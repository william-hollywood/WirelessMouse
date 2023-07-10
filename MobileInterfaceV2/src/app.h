#ifndef APP_H
#define APP_H

#include <stdint.h>

typedef enum State {
    IDLE,
    MOUSE,
    KEYBOARD,
    SETTINGS
} eState;

void AppInit(void);
void AppTick(uint8_t events);
void AppDraw(void);

#endif // APP_H