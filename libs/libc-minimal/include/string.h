#ifndef _LIBC_MINIMAL_STRING_H
#define _LIBC_MINIMAL_STRING_H

#include <kernel/types.h> // For size_t

/**
 * @brief Copies n bytes from memory area src to memory area dest.
 * @param dest The pointer to the destination array where the content is to be copied.
 * @param src The pointer to the source of data to be copied.
 * @param n The number of bytes to copy.
 * @return A pointer to the destination, which is dest.
 */
void* memcpy(void* dest, const void* src, size_t n);

/**
 * @brief Fills the first n bytes of the memory area pointed to by s with the constant byte c.
 * @param s The pointer to the memory area to be filled.
 * @param c The character to fill the memory area with.
 * @param n The number of bytes to be set to the value.
 * @return A pointer to the memory area s.
 */
void* memset(void* s, int c, size_t n);

/**
 * @brief Calculates the length of the string pointed to by s, excluding the terminating null byte.
 * @param s The string to be measured.
 * @return The number of characters in the string.
 */
size_t strlen(const char* s);

#endif // _LIBC_MINIMAL_STRING_H
