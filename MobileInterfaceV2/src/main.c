// Copyright (c) 2011-2020 <>< Charles Lohr - Under the MIT/x11 or NewBSD License you choose.
//  NO WARRANTY! NO GUARANTEE OF SUPPORT! USE AT YOUR OWN RISK
//  Super basic test - see rawdrawandroid's thing for a more reasonable test.
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/keycodes.h>
#include <android/log.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "CNFGAndroid.h"
#include "os_generic.h"

// #define CNFG3D
#define CNFG_IMPLEMENTATION
#include "CNFG.h"
#include "app.h"

uint8_t runtimeDebug = 0;
#define DGB_INT(var)                   \
if (runtimeDebug)                      \
{                                      \
    char buf[1024];                    \
    sprintf(buf, "%s: %d", #var, var); \
    CNFGDrawText(buf, 8);              \
    CNFGPenY += 40;                    \
}

uint32_t frames = 0;
uint64_t iframeno = 0;
volatile int32_t suspended;

int16_t screen_x, screen_y;
int32_t pressX = 0;
int32_t pressY = 0;
int32_t pressDown = 0;
int32_t newPress = 0;
int32_t moveX = 0;
int32_t moveY = 0;
int32_t numTouches = 0;
int32_t moveMask = 0;
int32_t lastkey, lastkeydown;

int8_t keyboard_up;

extern struct android_app* gapp;

#define GENLINEWIDTH 89
#define GENLINES 67
int genlinelen = 0;
char genlog[(GENLINEWIDTH + 1) * (GENLINES + 1) + 2] = "log";
int genloglen;
int genloglines;
int firstnewline = -1;

void example_log_function(int readSize, char* buf) {
    static og_mutex_t* mt;
    if (!mt) mt = OGCreateMutex();
    OGLockMutex(mt);
    int i;
    for (i = 0; (readSize >= 0) ? (i <= readSize) : buf[i]; i++) {
        char c = buf[i];
        if (c == '\0') c = '\n';
        if ((c != '\n' && genlinelen >= GENLINEWIDTH) || c == '\n') {
            int k;
            genloglines++;
            if (genloglines >= GENLINES) {
                genloglen -= firstnewline + 1;
                int offset = firstnewline;
                firstnewline = -1;

                for (k = 0; k < genloglen; k++) {
                    if ((genlog[k] = genlog[k + offset + 1]) == '\n' && firstnewline < 0) {
                        firstnewline = k;
                    }
                }
                genlog[k] = 0;
                genloglines--;
            }
            genlinelen = 0;
            if (c != '\n') {
                genlog[genloglen + 1] = 0;
                genlog[genloglen++] = '\n';
            }
            if (firstnewline < 0) firstnewline = genloglen;
        }
        genlog[genloglen + 1] = 0;
        genlog[genloglen++] = c;
        if (c != '\n') genlinelen++;
    }

    OGUnlockMutex(mt);
}

// Handle physical keys (volume), and back/home, etc
void HandleKey(int32_t keycode, int32_t bDown) {
    lastkey = keycode;
    lastkeydown = bDown;
    if (keycode == AKEYCODE_BACK) {
        AndroidSendToBack(1);
    }
}

// Handle presses on the screen
void HandleButton(int32_t x, int32_t y, int32_t button, int32_t bDown) {
    pressDown = newPress = bDown;
    if (bDown)
        AndroidDisplayKeyboard(0);
    numTouches = bDown ? button + 1 : 0;
    if (numTouches == 4)
        runtimeDebug = !runtimeDebug;
    pressX = x;
    pressY = y;
}

// Handle dragging on the screen
void HandleMotion(int32_t x, int32_t y, int32_t mask) {
    moveMask = mask;
    moveX = x;
    moveY = y;
}

void HandleDestroy() {
}

void HandleSuspend() {
    suspended = 1;
}

void HandleResume() {
    suspended = 0;
}

int32_t main() {
    double ThisTime;
    double LastFraneTime = OGGetAbsoluteTime();
    CNFGSetupFullscreen("Wireless Interface V2", 0);

    AppInit();

    while (1) {
        iframeno++;

        CNFGHandleInput();

        if (suspended) {
            usleep(50000);
            continue;
        }

        CNFGClearFrame();
        CNFGColor(0xffffffff);
        CNFGGetDimensions(&screen_x, &screen_y);
        uint8_t events = 0;
        events |= (newPress) ? 1 : 0;

        AppTick(events);
        AppDraw();
        CNFGPenX = 5;
        CNFGPenY = 5;
        DGB_INT(pressX);
        DGB_INT(pressY);
        DGB_INT(pressDown);
        DGB_INT(newPress);
        DGB_INT(moveX);
        DGB_INT(moveY);
        DGB_INT(numTouches);

// #define DEBUG
#ifdef DEBUG
        CNFGPenX = 5;
        CNFGPenY = 600;
        CNFGDrawText(genlog, 4);
#endif
        newPress = 0;

        frames++;
        CNFGSwapBuffers();

        ThisTime = OGGetAbsoluteTime();
        if (ThisTime > LastFraneTime + 1) {
            frames = 0;
            LastFraneTime += 1;
        }
    }

    return (0);
}
