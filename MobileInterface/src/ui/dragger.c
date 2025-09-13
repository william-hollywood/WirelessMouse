#include "MouseControl.h"
#include "dragger.h"
#include "os_generic.h"

#include <arpa/inet.h>
#include <limits.h>
#include <math.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int SockFd;
struct sockaddr_in ServerAddr;

uint8_t IsHoldDrag = 0;
int32_t LastDX, LastDY;

data_t MouseData = {
    .type = 0,
    .mouseData = {
        .dx = 0,
        .dy = 0,
    },
};

void CreateSocket(void)
{
    SockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (SockFd < 0) {
        perror("Error creating socket");
        return;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(8192);
    ServerAddr.sin_addr.s_addr = inet_addr("192.168.1.6");
}

void SendToSocket(uint8_t type, int32_t dx, int32_t dy)
{
    MouseData.type = type;
    MouseData.mouseData.dx = dx;
    MouseData.mouseData.dy = dy;
    if (sendto(SockFd, (void*)&MouseData, sizeof(MouseData), 0, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) < 0) {
        perror("Error sending data");
        return;
    }
}

void InitDragger(dragger_t* d)
{
    InitRect(&d->shape);
    CreateSocket();
}

void DestroyDragger(dragger_t* d)
{
    DestroyRect(&d->shape);
}

void DrawDragger(dragger_t* d)
{
    DrawRect(&d->shape);
}

extern int32_t PressX, PressY, PressDown, NewRelease, MoveX, MoveY, NumTouches;
extern double PressDownTime;
void PressDragger(dragger_t* d)
{
    if (InRect(&d->shape, PressX, PressY)) {
        switch (NumTouches) {
        case 1:
            SendToSocket(0, 0, 0);
            MoveX = PressX;
            MoveY = PressY;
            printf("Single Pressed dragger\n");
            break;

        default:
            printf("Unimplemented dragger press\n");
            break;
        }
    }
}

void ReleaseDragger(dragger_t* d)
{
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
    }
}

void DragDragger(dragger_t* d)
{
    if (InRect(&d->shape, PressX, PressY)) {
        int32_t dx = PressX - MoveX;
        int32_t dy = PressY - MoveY;
        switch (NumTouches) {
        case 1:
            if (LastDX != dx && LastDY != dy) { // throttle by time as well?
                SendToSocket(0, dx, dy);
                printf("Dragged dragger %d, %d\n", dx, dy);
                LastDX = dx;
                LastDY = dy;
            }
            break;
        default:
            break;
        }
    }
}