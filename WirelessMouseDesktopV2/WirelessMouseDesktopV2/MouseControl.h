#ifndef _MOUSE_CONTROL
#define _MOUSE_CONTROL

#include <stdint.h>

typedef struct mouse_data {
    int16_t dx;
    int16_t dy;
} mouse_data_t;

typedef struct data {
    uint8_t type;
    union {
        mouse_data_t mouseData;
        uint32_t keyData;
    };
} data_t;

void handle_UDP(data_t *data);

#endif // !_MOUSE_CONTROL
