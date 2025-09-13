#include "button.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

void InitButton(button_t* b)
{
    InitRect(&b->shape);
}

void DestroyButton(button_t* b)
{
    DestroyRect(&b->shape);
}

void DrawButton(button_t* b)
{
    DrawRect(&b->shape);
    CNFGColor(b->textColour);
    CNFGPenY = b->shape.y + (b->shape.height / 2) - (4 * b->textSize / 2);
    CNFGPenX = b->shape.x + (b->shape.width / 2) - (strlen(b->text) * 3 * b->textSize / 2);
    CNFGDrawText(b->text, b->textSize);
}

extern int32_t PressX;
extern int32_t PressY;

void ButtonHandlePress(button_t* b)
{
    if (b->onClick != NULL)
        if (InRect(&b->shape, PressX, PressY))
            b->onClick();
}