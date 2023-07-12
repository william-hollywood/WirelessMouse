#include "MouseControl.h"
#include "dragger.h"

#include <arpa/inet.h>
#include <limits.h>
#include <math.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int SockFd;
struct sockaddr_in ServerAddr;

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

void SendToSocket(int32_t dx, int32_t dy)
{
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

extern int32_t PressX, PressY, MoveX, MoveY, NumTouches;

void PressDragger(dragger_t* d)
{
    if (InRect(&d->shape, PressX, PressY))
        switch (NumTouches) {
        case 1:
            SendToSocket(SHRT_MAX, SHRT_MAX); // change to schedule send if not released before time limit
            SendToSocket(0, 0);
            MoveX = PressX;
            MoveY = PressY;
            printf("Single Pressed dragger\n");
            break;

        default:
            break;
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
                SendToSocket(dx, dy);
                printf("Dragged dragger %d, %d\n", dx, dy);
                LastDX = dx;
                LastDY = dy;
            }
        default:
            break;
        }
    }
}