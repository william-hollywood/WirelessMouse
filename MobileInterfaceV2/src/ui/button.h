#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "CNFG.h"

#define BUTTON_FLAG_BORDER (1 << 0)

typedef struct button
{
    RDPoint *points;
    int16_t radius, roundness, num_points, x, y, width, height, text_size;
    uint32_t colour, text_colour;
    char *text;
    void (*on_click)(void);
} button_t;

void init_button(button_t *b);
void destroy_button(button_t *b);
void draw_button(button_t *b);
int8_t tick_button(button_t *b);
uint8_t did_touch_button(button_t *b);

#endif