#ifndef RECT_H
#define RECT_H

#include "CNFG.h"

typedef struct rect {
	RDPoint *points;
	int16_t num_points, radius, x, y, width, height;
	uint32_t colour;
} rect_t;

void InitRect(rect_t *r);
void DestroyRect(rect_t *r);
void DrawRect(rect_t *r);
uint8_t InRect(rect_t *r, int32_t x, int32_t y);

#endif