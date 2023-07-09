#include "button.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define RAD (double)(b->radius)
#define LX (double)(b->x)
#define RX (LX + (double)(b->width))
#define LXi (LX + RAD)
#define RXi (RX - RAD)

#define TY (double)(b->y)
#define BY (TY + (double)(b->height))
#define TYi (TY + RAD)
#define BYi (BY - RAD)

#define ADD_POINT(x, y) b->points[point_count++] = (RDPoint) { (short)(x), (short)(y) }
#define GET_ANGLE(angle_var, iter_var) angle_var = (M_PI / 2.0) * ((double)(iter_var) + 1.0) / ((double)(b->roundness) + 1.0)
void init_button(button_t *b) {
    int32_t point_count = 0;
    if (RAD == 0) {
        b->num_points = 4;
        b->points = (RDPoint *)calloc(b->num_points, sizeof(RDPoint));
        ADD_POINT(LX, TY);
        ADD_POINT(RX, TY);
        ADD_POINT(RX, BY);
        ADD_POINT(LX, BY);
        return;
    }
    b->num_points = 8 + (4 * b->roundness);  // start + end of each side + 4 * number of corner points
    b->points = (RDPoint *)calloc(b->num_points, sizeof(RDPoint));
    double angle;
    // Top Line
    ADD_POINT(LXi, TY);  // TL
    ADD_POINT(RXi, TY);  // TR
    // Top Right Corner
    for (int i = 0; i < b->roundness; i++) {
        GET_ANGLE(angle, i);
        ADD_POINT(RXi + (RAD * cos(angle)), TYi - (RAD * sin(angle)));
    }
    // Right Line
    ADD_POINT(RX, TYi);  // RT
    ADD_POINT(RX, BYi);  // RB
    // Bottom Right Corner
    for (int i = 0; i < b->roundness; i++) {
        GET_ANGLE(angle, i);
        ADD_POINT(RXi + (RAD * cos(angle)), BYi + (RAD * sin(angle)));
    }
    // Bottom Line
    ADD_POINT(RXi, BY);  // BR
    ADD_POINT(LXi, BY);  // BL
    // Bottom Left Corner
    for (int i = 0; i < b->roundness; i++) {
        GET_ANGLE(angle, i);
        ADD_POINT(LXi - (RAD * cos(angle)), BYi + (RAD * sin(angle)));
    }
    // Left Line
    ADD_POINT(LX, BYi);  // LB
    ADD_POINT(LX, TYi);  // LT
    // Top Left Corner
    for (int i = 0; i < b->roundness; i++) {
        GET_ANGLE(angle, i);
        ADD_POINT(LXi - (RAD * cos(angle)), TYi - (RAD * sin(angle)));
    }
}

void destroy_button(button_t *b) {
    free(b->points);
}

void draw_button(button_t *b) {
    CNFGColor(b->colour);
    CNFGTackPoly(b->points, b->num_points);
    CNFGColor(b->text_colour);
    CNFGPenY = TY + (b->height / 2) - (3 * b->text_size / 2);
    CNFGPenX = LX + (b->width / 2) - (strlen(b->text) * 3 * b->text_size / 2);
    CNFGDrawText(b->text, b->text_size);
}

int8_t tick_button(button_t *b) {
    return 0;
}

extern int32_t pressX;
extern int32_t pressY;

uint8_t did_touch_button(button_t *b)
{
    if (pressX >= b->x && pressX <= b->x + b->width && pressY >= b->y && pressY <= b->y + b->height)
    {
        return 1;
    }
    return 0;
}