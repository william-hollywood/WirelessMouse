#include "MouseControl.h"

#include <windows.h>
#include <WinUser.h>

#include "Encoding.h"

#define SHORT_MAXVALUE 32767
#define SHORT_MINVALUE -32768

int32_t lastX;
int32_t lastY;
uint8_t holdPress = 0;
double SCALE = 2.5;

void handle_mouse(short dx, short dy) {
    if (dx == SHORT_MAXVALUE && dy == SHORT_MAXVALUE)  // left click
    {
        if (!holdPress) {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    } else if (dx == SHORT_MINVALUE && dy == SHORT_MINVALUE)  // right click
    {
        if (!holdPress) {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        }
    } else if (dx == SHORT_MAXVALUE && dy == SHORT_MINVALUE)  // left down toggle
    {
        if (!holdPress)  // check this
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        holdPress = 1;
    } else if (dx == SHORT_MINVALUE && dy == SHORT_MAXVALUE)  // left up toggle
    {
        if (holdPress)  // check this
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        holdPress = 0;
    } else {
        if (dx == 0 && dy == 0) {
            POINT p;
            GetCursorPos(&p);
            lastX = p.y;  // ew
            lastY = p.x;
        }
        SetCursorPos((int)(lastY + ((double)dy * SCALE)), (int)(lastX + ((double)dx * SCALE)));
    }
}

void handle_UDP(uint8_t *bytes) {
    decode(bytes);
    switch (type) {
        case 0:
            handle_mouse(dy, dx);
            break;
        case 1:
            // HandleKey(key);
            break;
    }
}