#include <jerrio.h>
#include <curses.h>
#include <keyboard.h>
#include <string.h>
#include <fs.h>

// Function to remove trailing whitespaces from a string
void trim_trailing_whitespace(char* str) {
    int length = strlen(str);
    while (length > 0 && (str[length - 1] == ' ' || str[length - 1] == '\t' || str[length - 1] == '\n')) {
        str[--length] = '\0'; // Replace trailing whitespace with null terminator
    }
}
typedef struct ManPage {
    char command[64];
    char description[256];
} ManPage;

ManPage pages[] = {
    {"exit", "Exit jsh.\nUsage: exit"},
    {"quit", "Exit jsh.\nUsage: quit"},
    {"echo", "Print text to the screen.\nUsage: echo [text]"},
    {"help", "Display available commands. Parentheses indicate aliases.\nUsage: help"},
    {"h", "Display available commands. Parentheses indicate aliases.\nUsage: h"},
    {"clear", "Clear the screen.\nUsage: clear"},
    {"ls", "List files in the filesystem.\nUsage: ls"}
};

char *lastCmd;

void man(const char *cmd) {
    for (int i = 0; i < sizeof(pages)/sizeof(ManPage); i++) {
        if (strcmp(pages[i].command, cmd) == 0) {
            print_str("Manual entry for ");
            print_str(cmd);
            print_char('\n');
            print_char('\n');
            print_str(pages[i].description);
            print_char('\n');
            return;
        }
    }
    print_str("No manual entry for ");
    print_str(cmd);
    print_char('\n');
}


void jsh() {
    char command[64];
    init_fs();
    flush();
    clear_screen(COLOR_BLACK);
    print_str("JerrOS Shell (jsh)\n\n");
    print_str("jsh> ");
    int idx = 0;

    while (1) {
        char c = getchar();
        if (c > 0) { // Only process valid characters
            // Check for Backspace (ASCII 0x08)
            if (c == '\b') {
                if (idx >= 0) {
                    command[idx] = (char)0; // Null-terminate the command
                    idx--;
                    flush();
                    clear_screen(COLOR_BLACK);
                    print_str("jsh> ");
                    print_str(command);
                }
            } else if (c == 'H') {
                strcpy(command, lastCmd);
                print_str(command);
                idx = strlen(command);
            } else {
                print_char(c); // Echo valid character
                command[idx++] = c;
                if (c == '\n') {
                    lastCmd = command;
                    command[idx] = '\0'; // Null-terminate the command

                    trim_trailing_whitespace(command);

                    flush();
                    clear_screen(COLOR_BLACK);
                    print_str("\n");
                    if (strcmp(command, "help") == 0 || strcmp(command, "h") == 0) {
                        print_str("Commands: help (h), echo, exit (quit)\n");
                    } else if (strncmp(command, "echo", 4) == 0) {
                        print_str(command + 5);
                        print_char('\n');
                    } else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
                        print_str("Goodbye!\n");
                        break;
                    } else if (strcmp(command, "clear") == 0) {
                        clear_screen(COLOR_BLACK);
                    } else if (strncmp(command, "man", 3) == 0) {
                        man(command + 4);
                    } else if (strcmp(command, "ls") == 0) {
                        list_files();
                    } else if (strncmp(command, "touch", 5) == 0) {
                        create_file(command + 6);
                    } else if (strncmp(command, "rm", 2) == 0) {
                        if (strlen(command) == 2) {
                            print_str("Usage: rm [file]\n");
                        } else {
                            delete_file(command + 3);
                        }
                    } else if (strncmp(command, "mkdir", 5) == 0) {
                        if (strlen(command) == 5) {
                            print_str("Usage: mkdir [directory]\n");
                        } else {
                            create_directory(command + 6);
                        }
                    } else if (strncmp(command, "cd", 2) == 0) {
                        if (strlen(command) == 2) {
                            change_directory(".");
                        } else {
                            change_directory(command + 3);
                        }
                    } else if (strcmp(command, "pwd") == 0) {
                        print_working_directory();
                    } /*else if (strcmp(command, "testelf") == 0) {
                        run_elf(elfTestBinary); NOT FUNCTIONAL!!!
                    }*/ else {
                        print_str("Unknown command\n");
                    }
                    idx = 0;
                    print_str("jsh> ");
                }
            }
        }   
    }
    __asm__("call kernel_main");
}

