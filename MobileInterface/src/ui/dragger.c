#include "dragger.h"
#include "MouseControl.h"

// Remove pedantic warnings for system includes
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <arpa/inet.h>
#include <limits.h>
#include <netinet/in.h>
#include <os_generic.h>
#include <sys/socket.h>
#pragma GCC diagnostic pop

int SockFd;
struct sockaddr_in ServerAddr;

uint8_t IsHoldDrag = 0;
int32_t LastX, LastY;

data_t MouseData = {
    .type = 0,
    .mouseData = {
        .dx = 0,
        .dy = 0,
    },
};

void CreateSocket(void) {
	SockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (SockFd < 0) {
		perror("Error creating socket");
		return;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(8192);
	ServerAddr.sin_addr.s_addr = inet_addr("192.168.1.6");
}

void SendToSocket(uint8_t type, int32_t dx, int32_t dy) {
	MouseData.type = type;
	MouseData.mouseData.dx = dx;
	MouseData.mouseData.dy = dy;
	if (sendto(SockFd, (void *)&MouseData, sizeof(MouseData), 0, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) <
		0) {
		perror("Error sending data");
		return;
	}
}

void InitDragger(dragger_t *d) {
	InitRect(&d->shape);
	CreateSocket();
}

void DestroyDragger(dragger_t *d) { DestroyRect(&d->shape); }

void DrawDragger(dragger_t *d) { DrawRect(&d->shape); }

extern int32_t PressX, PressY, PressDown, NewRelease, MoveX, MoveY, NumTouches;
extern double PressDownTime;
void PressDragger(dragger_t *d) {
	if (InRect(&d->shape, PressX, PressY)) {
		switch (NumTouches) {
		case 1:
			SendToSocket(0, 0, 0);
			MoveX = LastX = PressX;
			MoveY = LastY = PressY;
			printf("Single Pressed dragger\n");
			break;

		default:
			printf("Unimplemented dragger press\n");
			break;
		}
	}
}

void ReleaseDragger(dragger_t *d) {
	if (InRect(&d->shape, PressX, PressY)) {
		if ((OGGetAbsoluteTime() - PressDownTime) < 0.2) { // short press, it's a click.
			SendToSocket(0, SHRT_MAX, SHRT_MAX);
			printf("Send Short L-Click\n");
		} else { // long press only send a release
			if (IsHoldDrag) {
				SendToSocket(0, SHRT_MIN, SHRT_MAX);
				printf("Send Release L-Click\n");
			}
		}
		LastX = LastY = -1;
	}
}

void DragDragger(dragger_t *d) {
	if (InRect(&d->shape, PressX, PressY) && MoveX != LastX && MoveY != LastY) {
		int32_t dx = MoveX - LastX;
		int32_t dy = MoveY - LastY;
		switch (NumTouches) {
		case 1:
			if (LastX != dx && LastY != dy) { // throttle by time as well?
				SendToSocket(0, dx, dy);
				printf("Dragged dragger %d, %d\n", dx, dy);
				LastX = MoveX;
				LastY = MoveY;
			}
			break;
		default:
			break;
		}
	}
}
