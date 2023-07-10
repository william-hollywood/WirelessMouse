#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/keycodes.h>
#include <android/log.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CNFGAndroid.h"
#include "os_generic.h"

// #define CNFG3D
#define CNFG_IMPLEMENTATION
#include "CNFG.h"
#include "app.h"

uint8_t runtimeDebug = 0;
#define DGB_INT(var)                       \
    if (runtimeDebug) {                    \
        char buf[1024];                    \
        sprintf(buf, "%s: %d", #var, var); \
        CNFGDrawText(buf, 8);              \
        CNFGPenY += 40;                    \
    }

uint32_t frames = 0;
uint64_t iFrameNo = 0;
volatile int32_t suspended;

int16_t ScreenX, ScreenY;
int32_t PressX = 0;
int32_t PressY = 0;
int32_t PressDown = 0;
int32_t NewPress = 0;
int32_t MoveX = 0;
int32_t MoveY = 0;
int32_t NumTouches = 0;
int32_t MoveMask = 0;
int32_t LastKey, LastKeyDown;
int8_t KeyboardUp;
int8_t DoHideKeyboard = 0;

extern struct android_app* gapp;

#define GENLINEWIDTH 89
#define GENLINES 67
int GenLineLen = 0;
char GenLog[(GENLINEWIDTH + 1) * (GENLINES + 1) + 2] = "log";
int GenLogLen;
int GenLogLines;
int FirstNewLine = -1;

void LogFunction(int readSize, char* buf)
{
    static og_mutex_t* mt;
    if (!mt)
        mt = OGCreateMutex();
    OGLockMutex(mt);
    int i, k;
    for (i = 0; (readSize >= 0) ? (i <= readSize) : buf[i]; i++) {
        char c = buf[i];
        if (c == '\0')
            c = '\n';
        if ((c != '\n' && GenLineLen >= GENLINEWIDTH) || c == '\n') {
            GenLogLines++;
            if (GenLogLines >= GENLINES) {
                GenLogLen -= FirstNewLine + 1;
                int offset = FirstNewLine;
                FirstNewLine = -1;

                for (k = 0; k < GenLogLen; k++) {
                    if ((GenLog[k] = GenLog[k + offset + 1]) == '\n' && FirstNewLine < 0) {
                        FirstNewLine = k;
                    }
                }
                GenLog[k] = 0;
                GenLogLines--;
            }
            GenLineLen = 0;
            if (c != '\n') {
                GenLog[GenLogLen + 1] = 0;
                GenLog[GenLogLen++] = '\n';
            }
            if (FirstNewLine < 0)
                FirstNewLine = GenLogLen;
        }
        GenLog[GenLogLen + 1] = 0;
        GenLog[GenLogLen++] = c;
        if (c != '\n')
            GenLineLen++;
    }

    OGUnlockMutex(mt);
}

// Handle physical keys (volume), and back/home, etc
void HandleKey(int32_t keycode, int32_t bDown)
{
    LastKey = keycode;
    LastKeyDown = bDown;
    if (keycode == AKEYCODE_BACK) {
        AndroidSendToBack(1);
    }
}

// Handle presses on the screen
void HandleButton(int32_t x, int32_t y, int32_t button, int32_t bDown)
{
    PressDown = NewPress = bDown;
    if (bDown)
        DoHideKeyboard = 1;
    NumTouches = bDown ? button + 1 : 0;
    if (NumTouches == 5)
        runtimeDebug = !runtimeDebug;
    PressX = x;
    PressY = y;
}

// Handle dragging on the screen
void HandleMotion(int32_t x, int32_t y, int32_t mask)
{
    MoveMask = mask;
    MoveX = x;
    MoveY = y;
}

void HandleDestroy(void) { }

void HandleSuspend(void) { suspended = 1; }

void HandleResume(void) { suspended = 0; }

int32_t main()
{
    double thisTime;
    double lastFrameTime = OGGetAbsoluteTime();
    CNFGSetupFullscreen("Wireless Interface V2", 0);
    CNFGGetDimensions(&ScreenX, &ScreenY);

    AppInit();

    while (1) {
        iFrameNo++;

        CNFGHandleInput();

        if (suspended) {
            usleep(50000);
            continue;
        }

        CNFGClearFrame();
        CNFGColor(0xffffffff);
        CNFGGetDimensions(&ScreenX, &ScreenY);
        uint8_t events = 0;
        events |= (NewPress) ? 1 : 0;

        AppTick(events);
        AppDraw();
        CNFGColor(0xffffffff);
        CNFGPenX = 5;
        CNFGPenY = 5;
        DGB_INT(PressX);
        DGB_INT(PressY);
        DGB_INT(PressDown);
        DGB_INT(NewPress);
        DGB_INT(MoveX);
        DGB_INT(MoveY);
        DGB_INT(NumTouches);

        if (NumTouches == 4) {
            CNFGPenX = 5;
            CNFGPenY = 600;
            CNFGDrawText(GenLog, 4);
        }

        NewPress = 0;
        if (DoHideKeyboard) {
            AndroidDisplayKeyboard(0);
            DoHideKeyboard = 0;
        }

        frames++;
        CNFGSwapBuffers();

        thisTime = OGGetAbsoluteTime();
        if (thisTime > lastFrameTime + 1) {
            frames = 0;
            lastFrameTime += 1;
        }
    }

    return (0);
}
