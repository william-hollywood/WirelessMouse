#include "rect.h"

#include <math.h>
#include <stdlib.h>

#define RAD (double)(r->radius)
#define LX (double)(r->x)
#define RX (LX + (double)(r->width))
#define LXi (LX + RAD)
#define RXi (RX - RAD)

#define TY (double)(r->y)
#define BY (TY + (double)(r->height))
#define TYi (TY + RAD)
#define BYi (BY - RAD)

#define ADD_POINT(x, y) \
    r->points[point_count++] = (RDPoint) { (short)(x), (short)(y) }
#define GET_ANGLE(angle_var, iter_var) angle_var = (M_PI / 2.0) * ((double)(iter_var) + 1.0) / ((double)(r->roundness) + 1.0)

void InitRect(rect_t* r)
{
    int32_t point_count = 0;
    if (RAD == 0) {
        r->num_points = 4;
        r->points = (RDPoint*)calloc(r->num_points, sizeof(RDPoint));
        ADD_POINT(LX, TY);
        ADD_POINT(RX, TY);
        ADD_POINT(RX, BY);
        ADD_POINT(LX, BY);
        return;
    }
    r->num_points = 8; // start + end of each side + 4 * number of corner points
    r->points = (RDPoint*)calloc(r->num_points, sizeof(RDPoint));
    double angle;
    // Top Line
    ADD_POINT(LXi, TY); // TL
    ADD_POINT(RXi, TY); // TR
    // Right Line
    ADD_POINT(RX, TYi); // RT
    ADD_POINT(RX, BYi); // RB
    // Bottom Line
    ADD_POINT(RXi, BY); // BR
    ADD_POINT(LXi, BY); // BL
    // Left Line
    ADD_POINT(LX, BYi); // LB
    ADD_POINT(LX, TYi); // LT
}

void DestroyRect(rect_t* r)
{
    free(r->points);
}

void DrawRect(rect_t* r)
{
    CNFGColor(r->colour);
    CNFGTackPoly(r->points, r->num_points);
}

uint8_t InRect(rect_t* r, int32_t x, int32_t y)
{
    return (x >= r->x && x <= r->x + r->width && y >= r->y && y <= r->y + r->height);
}