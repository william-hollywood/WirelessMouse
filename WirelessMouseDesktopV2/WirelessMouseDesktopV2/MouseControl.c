#include "MouseControl.h"

#include <windows.h>
#include <WinUser.h>
#include <limits.h>

#include "Encoding.h"

int32_t lastX;
int32_t lastY;
uint8_t holdPress = 0;
double SCALE = 2.5;

void handle_mouse(short dx, short dy) {
    if (dx == SHRT_MAX && dy == SHRT_MAX)  // left click
    {
        if (!holdPress) {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    } else if (dx == SHRT_MIN && dy == SHRT_MIN)  // right click
    {
        if (!holdPress) {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        }
    } else if (dx == SHRT_MAX && dy == SHRT_MIN)  // left down toggle
    {
        if (!holdPress)  // check this
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        holdPress = 1;
    } else if (dx == SHRT_MIN && dy == SHRT_MAX)  // left up toggle
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