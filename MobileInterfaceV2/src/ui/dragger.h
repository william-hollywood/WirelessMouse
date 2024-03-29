#ifndef DRAGGER_H
#define DRAGGER_H

#include "CNFG.h"
#include "rect.h"
#include <stdint.h>

typedef struct dragger {
    rect_t shape;
} dragger_t;

void InitDragger(dragger_t* d);
void DestroyDragger(dragger_t* d);
void DrawDragger(dragger_t* d);
void PressDragger(dragger_t* b);
void ReleaseDragger(dragger_t* d);
void DragDragger(dragger_t* b);

#endif