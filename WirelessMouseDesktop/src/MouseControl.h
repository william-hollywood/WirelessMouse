#ifndef _MOUSE_CONTROL
#define _MOUSE_CONTROL

#include <stdint.h>

typedef struct mouse_data {
	int16_t dx;
	int16_t dy;
} __attribute__((packed)) mouse_data_t;

typedef struct data {
	uint8_t type;
	union {
		mouse_data_t mouseData;
		uint32_t keyData;
	};
} __attribute__((packed)) data_t;

void HandleUDP(data_t *data);
void init_uinput(void);

#endif // !_MOUSE_CONTROL
