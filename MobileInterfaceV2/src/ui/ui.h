#ifndef UI_H
#define UI_H

#include "button.h"

#define RGBA(r, g, b, a) ((uint32_t)(((uint8_t)(r) << 24) | ((uint8_t)(g) << 16) | ((uint8_t)(b) << 8) | ((uint8_t)(a))))
#define RGB(r, g, b) ((uint32_t)(((uint8_t)(r) << 24) | ((uint8_t)(g) << 16) | ((uint8_t)(b) << 8) | 0xFF))
#define GRAYSCALEA(val, a) ((uint32_t)(((uint8_t)(val) << 24) | ((uint8_t)(val) << 16) | ((uint8_t)(val) << 8) | ((uint8_t)(a))))
#define GRAYSCALE(val) ((uint32_t)(((uint8_t)(val) << 24) | ((uint8_t)(val) << 16) | ((uint8_t)(val) << 8) | 0xFF))

#endif