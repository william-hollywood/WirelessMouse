#ifndef DRAGGER_H
#define DRAGGER_H

#include <stdint.h>
#include "CNFG.h"

#define BUTTON_FLAG_BORDER (1 << 0)

typedef struct dragger_t
{
    RDPoint *points;
    int16_t radius, roundness, num_points, x, y, width, height, text_size;
    uint32_t colour, text_colour;
    char *text;
    void (*on_click)(void);
} dragger_t;

void init_dragger(dragger_t *b);
void destroy_dragger(dragger_t *b);
void draw_dragger(dragger_t *b);
int8_t tick_dragger(dragger_t *b);

#endif