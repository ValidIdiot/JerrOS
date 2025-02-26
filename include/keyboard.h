#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>
#define KB_BUFFER_SIZE 256

extern const char keymap[128];
/// @brief Initialize the keyboard
void init_keyboard();
/// @brief Get the scancode of the next key press
uint8_t get_scancode();
/// @brief Convert the scancode to an ASCII character (on the keymap)
char scancode_to_ascii(uint8_t scancode);
void keyboard_handler();
int read_line(char *buffer, int max_len);
char getchar();

#endif