/*----------------------------------------------------------------------------*\
|*                  JerrOS Input-Output Library (jerrio.h)                    *|
|*     This library provides functions for input and output operations.       *|
\*----------------------------------------------------------------------------*/

#ifndef JERRIO_H
#define JERRIO_H

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#ifndef KEYBOARD_PORT
    #define KEYBOARD_PORT 0x60
#endif
#ifndef KEYBOARD_STATUS_PORT
    #define KEYBOARD_STATUS_PORT 0x64
#endif

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern char* vga_buffer;
extern int cursor_pos;

void flush(); // Flush the VGA buffer
void itoa(int num, char *str, int base); // Convert integer to string

/// @brief Print a character to the VGA buffer
/// @param c The character to be printed
void print_char(char c);
/// @brief Print a string to the VGA buffer
/// @param message The string to be printed
void print_str(char *message);
/// @brief Print an integer to the VGA buffer
/// @param num The integer to be printed
void print_int(int num);

/// @brief A simple port I/O function (you may have this in your system already)
/// @param port The port to read from
uint8_t inb(uint16_t port);

void outb(uint16_t port, uint8_t val);


void wait_for_input(); // Wait for input from the keyboard

#endif