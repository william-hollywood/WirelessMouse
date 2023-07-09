#include "app.h"

#include "ui/ui.h"

eState app_state = IDLE;

uint8_t buttonCount = 0;
button_t buttons[8];
extern int8_t keyboard_up;

void CreateButton(int16_t x, int16_t y, int16_t w, int16_t h, char *text, void (*on_click)(void)) {
    buttons[buttonCount++] = (button_t){
        .x = x,
        .y = y,
        .width = w,
        .height = h,
        .colour = GRAYSCALE(0x80),
        .text_colour = GRAYSCALE(0xFF),
        .text = text,
        .text_size = 8,
        .on_click = on_click,
        .radius = 0,
        .roundness = 0};
}

extern uint8_t pressDown;

void showKeyboard(void) {
    if (pressDown) {
        AndroidDisplayKeyboard(1);
    }
}

void AppInit() {
    CreateButton(30, 30, 500, 150, "KEYBOARD", &showKeyboard);
    buttons[0].radius = 20;
    for (int i = 0; i < buttonCount; i++) {
        init_button(&buttons[i]);
    }
}

void AppTick(uint8_t events) {
    if (events != 0) {
        for (int i = 0; i < buttonCount; i++) {
            if (did_touch_button(&buttons[i])) {
                buttons[i].on_click();
            }
        }
    }
}

void AppDraw() {
    for (int i = 0; i < buttonCount; i++) {
        draw_button(&buttons[i]);
    }
}