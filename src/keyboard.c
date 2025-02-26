#include "keyboard.h"
#include "jerrio.h"

static char kb_buffer[KB_BUFFER_SIZE];
static int kb_buffer_pos = 0;

const char keymap[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, '?', 0, 0, 0, 0, 0, 0, 0, 0,
    0x48 /*Up Arrow goes to the left of this comment*/, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char scancode_to_ascii(uint8_t scancode) {
    return keymap[scancode];
}

void init_keyboard() {
    // Enable keyboard IRQ
    outb(0x21, inb(0x21) & ~0x02);
}
/*
char getchar() {
    uint8_t scancode = get_scancode();
    if (scancode & 0x80) {
        return 0; // Key release
    }
    return keymap[scancode];
}
*/

char getchar() {
    // Wait until there's input in the buffer
    wait_for_input();

    // Get the scancode of the pressed key
    uint8_t scancode = inb(KEYBOARD_PORT);

    // Check if the scancode indicates a key release (0x80 bit set)
    if (scancode & 0x80) {
        return 0; // Ignore key releases
    }

    // Handle special scancode for Backspace
    //if (scancode == 0x0E) {
    //    return '\b'; // Backspace
    //}

    // Convert the scancode to ASCII character
    char c = scancode_to_ascii(scancode);

    // Return the character if valid, otherwise return 0 (invalid character)
    
    return (c) ? c : (char)0;
}
uint8_t get_scancode() {
    wait_for_input();
    uint8_t sc = inb(KEYBOARD_PORT);
    if (sc & 0x80) {
        return 0;
    }
    return sc;
}
void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_PORT);

    // Check if the scancode is for a key press (not release)
    if (!(scancode & 0x80)) {
        char c = scancode_to_ascii(scancode);
        if (c != 0) {
            // If valid character, echo it and add it to the buffer
            if (kb_buffer_pos < KB_BUFFER_SIZE) {
                kb_buffer[kb_buffer_pos++] = c;
                print_char(c);  // Echo the valid character to the screen
            }
        }
    }
}

void read_input(char *buffer, int max_length) {
    int i = 0;
    char c;
    
    while (i < max_length - 1) {
        c = getchar();
        if (c == '\n') {
            buffer[i] = '\0';
            print_char('\n');
            return;
        } else if (c > 0) {
            buffer[i++] = c;
            print_char(c);
        }
    }
    buffer[i] = '\0';
}