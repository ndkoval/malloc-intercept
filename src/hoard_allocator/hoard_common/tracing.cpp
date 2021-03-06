#include "tracing.h"

#include <cstring>
#include <unistd.h>

void hoard::print_object(char const *message) {
	::write(2, message, strlen(message));
}

void hoard::print_object(void *px) {
	static char const *hexdigits = "0123456789abcdef";

	static char buffer[32];

	size_t n = (size_t) px;

	char *p = buffer;
	*p++ = '0';
	*p++ = 'x';
	int shift = sizeof(void *) * 8;

	do {
		shift -= 4;
		*p++ = hexdigits[(n >> shift) & 15];

	}
	while (shift != 0);

	*p = '\0';
	print_object(buffer);
}


void hoard::print_object(size_t n) {
	char buffer[32];

	size_t divisor = 1;

	while (divisor <= (n / 10))
		divisor *= 10;

	char *p = buffer;
	do {
		*p++ = (char) (((n / divisor) % 10) + '0');
		divisor /= 10;
	}
	while (divisor != 0);

	*p = '\0';

	print_object(buffer);
}


void hoard::print_object(long long l) {
	hoard::print_object((size_t) l);
}

void hoard::print_object(int i) {
	hoard::print_object((long long) i);
}

void hoard::print_object(bool b) {
	if (b) {
		hoard::print_object("true");
	} else {
		hoard::print_object("false");
	}
}

void hoard::print() {
}

static bool enabled = true;

bool hoard::trace_enabled() {
	static bool system_enabled = (getenv("MALLOC_INTERCEPT_NO_TRACE") == NULL);
	return system_enabled && enabled;
}

hoard::StopTraceGuard::StopTraceGuard() : initial_value_(enabled) {
  enabled = false;
}

hoard::StopTraceGuard::~StopTraceGuard() {
  enabled = initial_value_;
}
