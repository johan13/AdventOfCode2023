#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "decompress.h"
#include "gfx.h"

// We only support one open file at a time. (fd=3) file_length=-1 indicates no file open.
static int file_offset;
static int file_length = -1;

int _open(const char *path, int flags, ...)
{
    if (file_length != -1)
    {
        errno = ENFILE;
        return -1;
    }

    file_length = decompress(path, DECOMPRESS_BUF, DECOMPRESS_BUF_SIZE);
    if (file_length == -1)
        return -1;

    file_offset = 0;

    return 3;
}

int _close(int fd)
{
    if (fd != 3 || file_length == -1)
    {
        errno = EBADF;
        return -1;
    }

    file_length = -1;
    return 0;
}

int _read(int fd, char *ptr, int len)
{
    if (fd != 3 || file_length == -1)
    {
        errno = EBADF;
        return -1;
    }

    int bytes_available = file_length - file_offset;
    int bytes_read = len <= bytes_available ? len : bytes_available;
    memcpy(ptr, DECOMPRESS_BUF + file_offset, bytes_read);
    file_offset += bytes_read;
    return bytes_read;
}

int _write(int file, char *ptr, int len)
{
    switch (file)
    {
    case STDOUT_FILENO: // stdout prints to the text overlay
        for (int i = 0; i < len; i++)
            draw_char(ptr[i]);
        break;

    case STDERR_FILENO: // stderr prints to SWO
        for (int i = 0; i < len; i++)
            ITM_SendChar(ptr[i]);
        break;

    default:
        errno = EBADF;
        return -1;
    }

    return len;
}
