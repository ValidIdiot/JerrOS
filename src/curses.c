#include "curses.h"

static uint16_t* const buf = (uint16_t*)VGA_MEMORY;
static uint8_t cursor_x = 0, cursor_y = 0;

void init_curses() {
    clear_screen(COLOR_BLACK);
    set_cursor_position(0, 0);
}

void set_cursor_position(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;

    uint16_t position = y * VGA_WIDTH + x;

    // Update VGA cursor ports
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

void put_char_at(char c, uint8_t x, uint8_t y, uint8_t color) {
    buf[y * VGA_WIDTH + x] = (uint16_t)c | (uint16_t)color << 8;
}

void clear_screen(uint8_t color) {
    uint16_t blank = (uint16_t)' ' | (uint16_t)color << 8;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        buf[i] = blank;
    }
    set_cursor_position(0, 0);
}

void print_string(const char* str, uint8_t x, uint8_t y, uint8_t color) {
    while (*str) {
        put_char_at(*str++, x++, y, color);
        if (x >= VGA_WIDTH) {
            x = 0;
            y++;
        }
    }
}
