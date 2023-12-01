#include <lz4.h>
#include <lz4hc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Compile with: gcc lz4hc.c -Wall -Os -s -llz4 -o lz4hc

int main(int argc, char **argv)
{
    uint8_t compress = 0;
    if (argc == 1) {
        compress = 1;
    }
    else if (argc != 2 || strcmp(argv[1], "-d"))
    {
        fprintf(stderr, "Usage: lz4hc [-d] < in_file > out_file\n");
        return 1;
    }

    char in_buffer[1024 * 1024];
    char out_buffer[1024 * 1024];
    size_t total_bytes_read = 0;

    for (;;)
    {
        ssize_t bytes_read = read(STDIN_FILENO, in_buffer, sizeof in_buffer - total_bytes_read);
        if (bytes_read < 0)
            return 1;

        if (bytes_read == 0)
            break;
        total_bytes_read += bytes_read;
    }

    int out_size;
    if (compress)
        out_size = LZ4_compress_HC(in_buffer, out_buffer, total_bytes_read, sizeof out_buffer, LZ4HC_CLEVEL_MAX);
    else
        out_size = LZ4_decompress_safe(in_buffer, out_buffer, total_bytes_read, sizeof out_buffer);

    if (out_size == 0)
        return 1;

    ssize_t bytes_written = write(STDOUT_FILENO, out_buffer, out_size);
    if (bytes_written != out_size)
        return -1;

    return 0;
}
