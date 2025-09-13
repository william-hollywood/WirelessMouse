#include "app.h"

#include "CNFGAndroid.h"
#include "ui/ui.h"
#include <stdio.h>
#include <string.h>

#define BORDER_SIZE 30

eState AppState = IDLE;

uint8_t ButtonCount = 0;
button_t Buttons[8];
dragger_t Dragger;

void CreateButton(int16_t x, int16_t y, int16_t w, int16_t h, char* text, void (*onClick)(void))
{
    Buttons[ButtonCount++] = (button_t) {
        .shape = (rect_t) {
            .x = x,
            .y = y,
            .width = w,
            .height = h,
            .colour = GRAYSCALE(0x7F),
            .radius = 20,
        },
        .textColour = GRAYSCALE(0xFF),
        .text = text,
        .textSize = 15,
        .onClick = onClick
    };
}

extern uint8_t PressDown;
extern uint8_t DoHideKeyboard;
void ShowKeyboard(void)
{
    if (PressDown) {
        AndroidDisplayKeyboard(1);
        DoHideKeyboard = 0;
    }
}

extern int16_t ScreenX, ScreenY;

void AppInit(void)
{
    CreateButton(BORDER_SIZE, BORDER_SIZE, (ScreenX - 100) / 2, 150, "KEYBOARD", ShowKeyboard);
    CreateButton(ScreenX - BORDER_SIZE - ((ScreenX - 100) / 2), BORDER_SIZE, (ScreenX - 100) / 2, 150, "ADDRESS", NULL);
    for (int i = 0; i < ButtonCount; i++) {
        InitButton(&Buttons[i]);
    }
    Dragger = (dragger_t) {
        .shape = (rect_t) {
            .x = BORDER_SIZE,
            .y = 210,
            .width = ScreenX - (2 * BORDER_SIZE),
            .height = ScreenY - 150 - (4 * BORDER_SIZE),
            .colour = RGB(30, 132, 210),
            .radius = 20,
        },
    };
    InitDragger(&Dragger);
}

void AppTick(uint8_t events)
{
    if (events != 0) {
        if (events & EVENT_PRESS) {
            for (int i = 0; i < ButtonCount; i++) {
                ButtonHandlePress(&Buttons[i]);
            }
            PressDragger(&Dragger);
        }
        if (events & EVENT_DRAG) {
            DragDragger(&Dragger);
        }
        if (events & EVENT_RELEASE) {
            ReleaseDragger(&Dragger);
        }
    }
}

void AppDraw(void)
{
    for (int i = 0; i < ButtonCount; i++) {
        DrawButton(&Buttons[i]);
    }
    DrawDragger(&Dragger);
}