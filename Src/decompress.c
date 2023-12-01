#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "decompress.h"
#include "data.h"

void lz4_decompress(const void *src, void *dst, uint32_t dst_size);

int decompress(const char *path, void *dest, int dest_size)
{
    for (int i = 0; data_entries[i].data != NULL; i++)
    {
        if (0 == strncmp(path, data_entries[i].name, sizeof data_entries->name)) {
            if (dest_size < data_entries[i].size)
            {
                errno = ENOBUFS;
                return -1;
            }

            lz4_decompress(data_entries[i].data, dest, data_entries[i].size);
            return data_entries[i].size;
        }
    }

    errno = ENOENT;
    return -1;
}