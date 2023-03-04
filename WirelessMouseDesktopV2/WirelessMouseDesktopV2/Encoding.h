#ifndef _ENCODING
#define _ENCODING

#include <stdint.h>

uint32_t type;
int16_t dx;
int16_t dy;
uint32_t key;

uint32_t get_encoding(char* password);

void decode(uint8_t* bytes);
void decode_mouse(uint8_t* bytes);
void decode_key(uint8_t* bytes);


#endif // !_ENCODING
