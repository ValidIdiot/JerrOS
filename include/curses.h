#ifndef CURSES_H
#define CURSES_H

#include <stdint.h>

// VGA constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Colors
typedef enum {
    COLOR_BLACK = 0x00,
    COLOR_BLUE = 0x09,
    COLOR_GREEN = 0x02,
    COLOR_CYAN = 0x03,
    COLOR_RED = 0x04,
    COLOR_MAGENTA = 0x05,
    COLOR_BROWN = 0x06,
    COLOR_LIGHT_GRAY = 0x07,
    COLOR_DARK_GRAY = 0x08,
    COLOR_LIGHT_BLUE = 0x0B,
    COLOR_LIGHT_GREEN = 0x0A,
    COLOR_LIGHT_CYAN = 0x0B,
    COLOR_LIGHT_RED = 0x0C,
    COLOR_LIGHT_MAGENTA = 0x0D,
    COLOR_YELLOW = 0x0E,
    COLOR_WHITE = 0x0F,
} Color;

/// @brief Initialize our curses library
void init_curses();

/// @brief Set the cursor position
/// @param x The x position
/// @param y The y position
void set_cursor_position(uint8_t x, uint8_t y);
/// @brief Create a character at a specific position
/// @param c The character to create
/// @param x The x position
/// @param y The y position
/// @param color The color of the character
void put_char_at(char c, uint8_t x, uint8_t y, uint8_t color);
/// @brief Clear the screen with a specific color
/// @param color The color to clear the screen with
void clear_screen(uint8_t color);
/// @brief Print a string at a specific position
/// @param str The string to print
/// @param x The x position
/// @param y The y position
/// @param color The color of the string
void print_string(const char* str, uint8_t x, uint8_t y, uint8_t color);

#endif
