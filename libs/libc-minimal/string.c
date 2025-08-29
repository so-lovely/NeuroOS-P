/**
 * @file string.c
 * @brief Minimal C library string functions.
 * @author Gemini
 *
 * This implementation is designed to be simple, portable, and suitable for
 * a lightweight RTOS environment. It avoids any complex or non-deterministic
 * operations.
 */

#include <string.h>

/**
 * Copies n bytes from memory area src to memory area dest.
 * The memory areas must not overlap.
 */
void* memcpy(void* dest, const void* src, size_t n)
{
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

/**
 * Fills the first n bytes of the memory area pointed to by s
 * with the constant byte c.
 */
void* memset(void* s, int c, size_t n)
{
    unsigned char* p = (unsigned char*)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (unsigned char)c;
    }
    return s;
}

/**
 * Calculates the length of the string pointed to by s,
 * excluding the terminating null byte ('\0').
 */
size_t strlen(const char* s)
{
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}
