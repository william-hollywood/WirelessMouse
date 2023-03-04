#include "Encoding.h"

#include <stdio.h>
#include <string.h>

uint32_t checkNum = 0x55555555;
char password[100] = "Default";

uint32_t get_encoding() {
    uint32_t encoding = checkNum;
    // pad the password to be an even number of characters
    if (strlen(password) % 2 != 0) {
        password[strlen(password)] = ' ';
        password[strlen(password) + 1] = '\0';
    }
    // just some bit shifting to act as a password endoding
    for (int i = 0; i < strlen(password); i += 2) {
        uint32_t ca = password[i];
        uint32_t cb = password[i + 1];
        uint32_t c1 = (cb >> 8) + (cb >> 4) + (ca << 4) + (ca << 8) + cb;
        uint32_t c2 = (ca >> 8) + (cb >> 4) + (ca << 4) + (cb << 8) + ca;

        uint32_t cc = (c1 << 16) + c2;
        encoding ^= cc;
    }

    return encoding;
}

void decode(uint8_t* bytes) {
    uint32_t encoding = get_encoding();
    uint32_t checksum = checkNum ^ encoding;

    uint32_t* inChecksum = (uint32_t*)&bytes[8];
    if (checksum == *inChecksum) {
        type = *((uint32_t*)&bytes[0]) ^ encoding;
        switch (type) {
            case 0:
                decode_mouse(&bytes[4]);
                key = 0;
                return;
            case 1:
                decode_key(&bytes[4]);
                dx = 0;
                dy = 0;
                return;
        }
    }
    type = 0;
    key = 0;
    dx = 0;
    dy = 0;
    return;
}

void decode_mouse(uint8_t* bytes) {
    uint32_t values = *((uint32_t*)&bytes[0]);
    values ^= get_encoding();

    dx = (int16_t)(values >> 16);
    dy = (int16_t)(values & 0xFFFF);
}

void decode_key(uint8_t* bytes) {
}