#ifndef _MOUSE_CONTROL
#define _MOUSE_CONTROL

#include <stdint.h>

typedef struct mouse_data {
    int16_t dx;
    int16_t dy;
} mouse_data_t;

#pragma pack(push,1)
typedef struct data {
    uint8_t type;
    union {
        mouse_data_t mouseData;
        uint32_t keyData;
    };
} data_t;
#pragma pack(pop)
void HandleUDP(data_t* data);

#endif // !_MOUSE_CONTROL
