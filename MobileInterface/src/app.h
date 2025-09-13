#ifndef APP_H
#define APP_H

#include <stdint.h>

typedef enum State {
    IDLE,
    MOUSE,
    KEYBOARD,
    SETTINGS
} eState;

#define EVENT_PRESS 0x1
#define EVENT_RELEASE 0x2
#define EVENT_DRAG 0x4
#define EVENT_OTHER 0x8

void AppInit(void);
void AppTick(uint8_t events);
void AppDraw(void);

#endif // APP_H