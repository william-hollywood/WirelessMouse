#include "MouseControl.h"

#include <fcntl.h>
#include <limits.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

uint8_t holdPress = 0;
double SCALE = 0.5;
int fd = -1;

void emit(int fd, int type, int code, int val) {
	struct input_event ie;

	ie.type = type;
	ie.code = code;
	ie.value = val;
	/* timestamp values below are ignored */
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;

	write(fd, &ie, sizeof(ie));
}

void init_uinput(void) {
	struct uinput_setup usetup;
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	/* enable mouse button left and relative events */
	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

	ioctl(fd, UI_SET_EVBIT, EV_REL);
	ioctl(fd, UI_SET_RELBIT, REL_X);
	ioctl(fd, UI_SET_RELBIT, REL_Y);
	ioctl(fd, UI_SET_RELBIT, REL_WHEEL);
	ioctl(fd, UI_SET_RELBIT, REL_HWHEEL);
	ioctl(fd, UI_SET_RELBIT, REL_WHEEL_HI_RES);
	ioctl(fd, UI_SET_RELBIT, REL_HWHEEL_HI_RES);

	memset(&usetup, 0, sizeof(usetup));
	usetup.id.bustype = BUS_USB;
	usetup.id.vendor = 0x1234;	/* sample vendor */
	usetup.id.product = 0x5678; /* sample product */
	strcpy(usetup.name, "Example device");

	ioctl(fd, UI_DEV_SETUP, &usetup);
	ioctl(fd, UI_DEV_CREATE);
}

void mouse_click(int event, int val) {
	emit(fd, EV_KEY, event, val);
	emit(fd, EV_SYN, SYN_REPORT, 0);
}

void move_event(int relX, int relY) {
	emit(fd, EV_REL, REL_X, relX);
	emit(fd, EV_REL, REL_Y, relY);
	emit(fd, EV_SYN, SYN_REPORT, 0);
}

void handle_move(mouse_data_t *data) {
	int16_t dx = data->dx;
	int16_t dy = data->dy;
	if (dx == SHRT_MAX && dy == SHRT_MAX) // left click
	{
		if (!holdPress) {
			mouse_click(BTN_LEFT, 1);
			mouse_click(BTN_LEFT, 0);
		}
	} else if (dx == SHRT_MIN && dy == SHRT_MIN) // right click
	{
		if (!holdPress) {
			mouse_click(BTN_RIGHT, 1);
			mouse_click(BTN_RIGHT, 0);
		}
	} else if (dx == SHRT_MAX && dy == SHRT_MIN) // left down toggle
	{
		if (!holdPress) { // check this
			mouse_click(BTN_LEFT, 1);
		}
		holdPress = 1;
	} else if (dx == SHRT_MIN && dy == SHRT_MAX) // left up toggle
	{
		if (holdPress) { // check this
			mouse_click(BTN_LEFT, 0);
		}
		holdPress = 0;
	} else {
		move_event((int)(((double)dx * SCALE)), (int)(((double)dy * SCALE)));
	}
}

void handle_scroll(mouse_data_t *data) {
	int16_t dx = data->dx;
	int16_t dy = data->dy;
	int32_t event = 0;
	int32_t dir = 0;
	static const int32_t SCROLL_SCALE = 4;
	if (dy != 0) {
		event = REL_WHEEL_HI_RES;
		dir = dy * SCROLL_SCALE;
	} else if (dx != 0) {
		event = REL_HWHEEL_HI_RES;
		dir = dx * SCROLL_SCALE;
	}
	if (event != 0) {
		emit(fd, EV_REL, event, dir);
		emit(fd, EV_SYN, SYN_REPORT, 0);
	}
}

void HandleUDP(data_t *data) {
	switch (data->type) {
	case 0:
		handle_move(&(data->mouseData));
		break;
	case 1:
	case 2:
		handle_scroll(&(data->mouseData));
		break;
	}
}
