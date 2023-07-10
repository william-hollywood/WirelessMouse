#include "Encoding.h"

#include <stdio.h>
#include <string.h>


void process(uint8_t* bytes) {
    type = *((uint32_t*)&bytes[0]);
    switch (type) {
        case 0:
            dx = *((int16_t*)&bytes[0]);
            dy = *((int16_t*)&bytes[2]);
            key = 0;
            return;
        case 1:
            key = 0;
            dx = 0;
            dy = 0;
            return;
    }
    return;
}