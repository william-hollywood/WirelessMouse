#ifndef APP_H
#define APP_H

#include <stdint.h>

typedef enum State {
    IDLE,
    MOUSE,
    KEYBOARD,
    SETTINGS
} eState;

void AppInit();
void AppTick(uint8_t events);
void AppDraw();

#endif // APP_H