#ifndef _LIBC_MINIMAL_STDIO_H
#define _LIBC_MINIMAL_STDIO_H

/**
 * @brief Prints a formatted string to the console.
 * 
 * This is a lightweight version of printf, supporting a limited set of format specifiers:
 * - %c: character
 * - %s: string
 * - %d: signed decimal integer
 * - %u: unsigned decimal integer
 * - %x: hexadecimal integer
 * - %p: pointer address
 * - %%: literal %
 *
 * @param format The format string.
 * @param ... The arguments for the format specifiers.
 * @return The number of characters printed.
 */
int printf(const char* format, ...);

/**
 * @brief Writes a character to the console.
 * @param c The character to write.
 * @return The character written.
 */
int putchar(int c);

#endif // _LIBC_MINIMAL_STDIO_H
