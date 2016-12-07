#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void remap_text_section(const uint8_t* start_text, const uint8_t* end_text);
