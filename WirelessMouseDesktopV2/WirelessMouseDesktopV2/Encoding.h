#ifndef _ENCODING
#define _ENCODING

#include <stdint.h>

uint32_t type;
int16_t dx;
int16_t dy;
uint32_t key;

void process(uint8_t* bytes);

#endif // !_ENCODING
