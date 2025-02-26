// kernel.c
#include <jerrio.h>
#include <jerrport.h>
#include <string.h>
#include <keyboard.h>
#include <curses.h>
#include <jerrmem.h>

/////////////////
// DEFINITIONS //
/////////////////
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_SIZE (VGA_WIDTH * VGA_HEIGHT)

char* vga_buffer = (uint16_t*)0xB8000;
int cursor_pos = 0;

void flush() {
    for (int i = 0; i < VGA_SIZE * 2; i++) {
        vga_buffer[i] = ' ';
    }
    cursor_pos = 0;
}
void itoa(int num, char *str, int base) {
    int i = 0;
    bool is_negative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0 && base == 10) {
        is_negative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
void print_char(char c) {
    if (c == '\n') {
        cursor_pos += 160 - (cursor_pos % 160); // Move to the beginning of the next line
    } else {
        vga_buffer[cursor_pos] = c;            // Character
        vga_buffer[cursor_pos + 1] = 0x07;     // Attribute (white on black)
        cursor_pos += 2;
    }
}
void print_str(char *message) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\n') {
            cursor_pos += 160 - (cursor_pos % 160); // Move to the beginning of the next line
        } else {
            vga_buffer[cursor_pos] = message[i];        // Character
            vga_buffer[cursor_pos + 1] = 0x07;         // Attribute (white on black)
            cursor_pos += 2;
        }
    }
}
void print_int(int num) {
    char str[32];
    itoa(num, str, 10);
    print_str(str);
}
uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "d"(port));
    return result;
}
void wait_for_input() {
    while ((inb(0x64) & 0x01) == 0) {
        // Wait 
    }
}


void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
int strncmp(const char *s1, const char *s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) {
        return 0;
    } else {
        return *(unsigned char *)s1 - *(unsigned char *)s2;
    }
}
char *strncpy(char *dest, const char *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n && (*d++ = *s++)) {
        n--;
    }
    while (n--) {
        *d++ = '\0';
    }
    return dest;
}
int strlen(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
char *strcpy(char *dest, const char *src) {
    char *original = dest;  // Save the starting position of dest
    while ((*dest++ = *src++) != '\0');  // Copy characters including null terminator
    return original;  // Return the destination pointer
}
char *strcat(char *dest, const char *src) {
    char *ptr = dest;
    while (*ptr) ptr++;  // Move to end of dest
    while ((*ptr++ = *src++));  // Copy src to dest
    return dest;
}
void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}
void *memset(void *ptr, int value, size_t num) {
    unsigned char *p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}
char *strncat(char *dest, const char *src, size_t n)
{
    char *ptr = dest;
    while (*ptr) ptr++;

    while (n-- && *src) {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return dest;
}

static char heap[HEAP_SIZE];
static size_t heap_index = 0;

void *malloc(size_t size) {
    if (heap_index + size > HEAP_SIZE) {
        return NULL;
    }
    void *ptr = &heap[heap_index];
    heap_index += size;
    return ptr;
}

void free(void *ptr) {
    if (ptr >= (void*) heap && ptr < (void*) (heap + HEAP_SIZE)) {
        size_t offset = (char*) ptr - heap;
        if (offset == heap_index - 1) {
            heap_index = offset;
        }
    }
}


void kernel_main() {
    flush();
    print_str("Hello, world, from JerrOS!");
    init_keyboard();
    interface();
    while (1) { __asm__("hlt"); }
}


void interface() {
    init_curses();

    print_string("Welcome to JerrOS!\n", 0, 0, COLOR_LIGHT_GREEN);
    print_string("Choose an option:\n", 0, 1, COLOR_LIGHT_GRAY);
    print_string("1. Run Shell (jsh)\n", 0, 2, COLOR_LIGHT_CYAN);
    print_string("2. Shutdown\n", 0, 3, COLOR_LIGHT_CYAN);
    // print_string("3. Test ELF Executable\n", 0, 4, COLOR_LIGHT_CYAN); NOT FUNCTIONAL!!!
    print_string("Enter your choice: ", 0, 4, COLOR_LIGHT_GRAY);

    // Get user input
    char choice = getchar();

    if (choice == '1') {
        clear_screen(COLOR_BLACK);
        print_string("Starting Shell...\n", 0, 0, COLOR_LIGHT_GREEN);
        __asm__("call jsh");
    } else if (choice == '2') {
        clear_screen(COLOR_BLACK);
        print_string("Shutting down...\n", 0, 0, COLOR_LIGHT_RED);
        __asm__("call shutdown");
    } /*else if (choice == '3') {
        run_elf(elfTestBinary); NOT FUNCTIONAL!!!
    }*/ else {
        clear_screen(COLOR_BLACK);
        print_string("Invalid choice. Restarting...\n", 0, 0, COLOR_LIGHT_RED);
        interface();
    }
}
