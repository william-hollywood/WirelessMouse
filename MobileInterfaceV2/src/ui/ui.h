#ifndef UI_H
#define UI_H

#include "button.h"
#include "dragger.h"

#define RGBA(r, g, b, a) ((((uint32_t)(r) << 24) | ((uint32_t)(g) << 16) | ((uint32_t)(b) << 8) | ((uint32_t)(a))))
#define RGB(r, g, b) ((((uint32_t)(r) << 24) | ((uint32_t)(g) << 16) | ((uint32_t)(b) << 8) | 0xFF))
#define GRAYSCALEA(val, a) ((((uint32_t)(val) << 24) | ((uint32_t)(val) << 16) | ((uint32_t)(val) << 8) | ((uint32_t)(a))))
#define GRAYSCALE(val) ((((uint32_t)(val) << 24) | ((uint32_t)(val) << 16) | ((uint32_t)(val) << 8) | 0xFF))

#endif