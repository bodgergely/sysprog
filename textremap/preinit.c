
#include "remap.h"
#include <stdio.h>
#include <stdint.h>

extern const uint8_t __start_text;
extern const uint8_t __end_text;

void init()
{
	printf("preinit code at: %p\n", init);
	remap_text_section(&__start_text, &__end_text);
}

void (* const preinits[])(void) __attribute__((section(".preinit_array"),visibility("hidden")))  =
{
		&init
};


