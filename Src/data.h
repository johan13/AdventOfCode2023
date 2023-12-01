#pragma once
#include <stdint.h>

struct entry
{
    char name[16];
    int size;
    const uint8_t *data;
};

extern struct entry const data_entries[];
