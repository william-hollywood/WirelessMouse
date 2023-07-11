#include "dragger.h"

#include <arpa/inet.h>
#include <math.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
int SockFd;
struct sockaddr_in ServerAddr;

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

void SendToSocket(void)
{
    char buffer[512];
    if (sendto(SockFd, buffer, strlen(buffer), 0, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) < 0) {
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

void PressDragger(dragger_t* d)
{
}

void DragDragger(dragger_t* d)
{
}