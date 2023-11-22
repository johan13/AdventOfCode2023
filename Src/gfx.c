#include <stdint.h>
#include "gfx_data.h"

extern uint8_t *overlay_frame_buf;

static void draw_index(int i);

static const uint16_t stride = 400;
static const uint16_t line_height = 20;

static uint16_t x = 0, y = 480;

void draw_header()
{
    for (uint16_t dy = 0; dy < 95; dy++)
    {
        uint8_t *out_ptr = &overlay_frame_buf[(y + dy) * stride];
        for (uint16_t i = 0; i < 50; i++)
        {
            uint32_t d = header[dy * 50 + i];

            *(out_ptr++) = ((d >> 24) & 0xf0) | 0xf;
            *(out_ptr++) = ((d >> 20) & 0xf0) | 0xf;
            *(out_ptr++) = ((d >> 16) & 0xf0) | 0xf;
            *(out_ptr++) = ((d >> 12) & 0xf0) | 0xf;
            *(out_ptr++) = ((d >> 8) & 0xf0) | 0xf;
            *(out_ptr++) = ((d >> 4) & 0xf0) | 0xf;
            *(out_ptr++) = ((d >> 0) & 0xf0) | 0xf;
            *(out_ptr++) = ((d << 4) & 0xf0) | 0xf;
        }
    }
    x = 0;
    y += 95;
}

void draw_char(int ch)
{
    if (ch == '\n')
    {
        x = 0;
        y += line_height;
        return;
    }

    if (ch == '-')
        draw_index(0);
    else if (ch == '.')
        draw_index(1);
    else if (ch >= '0' && ch <= '9')
        draw_index(ch - '0' + 2);
    else if (ch >= 'A' && ch <= 'Z')
        draw_index(ch - 'A' + 12);
    else if (ch >= 'a' && ch <= 'z')
        draw_index(ch - 'a' + 12);
    else
        x += 12;

    x += 2;
}

static void draw_index(int i)
{
    uint16_t max_width = 0;
    const uint16_t *ch = font[i];
    uint8_t *out_ptr = &overlay_frame_buf[y * stride + x];
    for (uint16_t dy = 0; dy < 16; dy++)
    {
        uint16_t ch_row = ch[dy];
        uint16_t dx = 0;
        for (;;)
        {
            out_ptr[dx] = ch_row & 0x8000 ? 0xff : 0;
            ch_row <<= 1;
            if (!ch_row)
                break;
            dx++;
        }
        max_width = dx > max_width ? dx : max_width;
        out_ptr += stride;
    }

    x += max_width;
}
