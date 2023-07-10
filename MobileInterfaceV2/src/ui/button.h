#ifndef BUTTON_H
#define BUTTON_H

#include "rect.h"
#include <stdint.h>

typedef struct button {
    rect_t shape;
    int16_t textSize;
    uint32_t textColour;
    char* text;
    void (*onClick)(void);
} button_t;

void InitButton(button_t* b);
void DestroyButton(button_t* b);
void DrawButton(button_t* b);
void ButtonHandlePress(button_t* b);

#endif