#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void draw_header();
void set_x_pos(int new_x);
int width_of_string(const char *str);
void draw_char(int ch);

#ifdef __cplusplus
}
#endif