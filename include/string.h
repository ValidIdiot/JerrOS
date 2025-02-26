#ifndef STRING_H
#define STRING_H

#include <stddef.h>

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
int strlen(const char *s);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *ptr, int value, size_t num);

#endif