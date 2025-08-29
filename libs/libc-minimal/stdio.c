/**
 * @file stdio.c
 * @brief Minimal C library stdio functions.
 * @author Gemini
 */

#include <stdio.h>
#include <kernel/syscall.h>

// This is a trick to use the registers for syscall arguments.
// We define a function that will be inlined, and the compiler will
// hopefully place the arguments in the correct registers (a0, a7).
static inline long syscall1(long n, long arg0)
{
    register long a0 asm("a0") = arg0;
    register long a7 asm("a7") = n;
    asm volatile("ecall"
                 : "+r"(a0)
                 : "r"(a7)
                 : "memory");
    return a0;
}

int putchar(int c)
{
    syscall1(SYS_putchar, c);
    return c;
}

// A simple integer to ascii conversion function.
static void itoa(int n, char* buffer, int base)
{
    if (base < 2 || base > 16) {
        return;
    }

    int i = 0;
    int is_negative = 0;

    if (n == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    if (n < 0 && base == 10) {
        is_negative = 1;
        n = -n;
    }

    while (n != 0) {
        int rem = n % base;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n = n / base;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

// A simple unsigned integer to ascii conversion function for hex.
static void utoa_hex(unsigned long n, char* buffer)
{
    int i = 0;
    if (n == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    while (n != 0) {
        int rem = n % 16;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n = n / 16;
    }
    buffer[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

// Minimal printf implementation
int printf(const char* format, ...)
{
    // Note: This is a very basic implementation of printf.
    // It uses a fixed-size buffer and does not support all format specifiers.
    // This is intentional to keep the code size small and deterministic for the
    // neuromorphic photonics environment.

    // va_list, va_start, va_arg, va_end are macros to handle variable arguments.
    __builtin_va_list args;
    __builtin_va_start(args, format);

    int written = 0;
    char buffer[32];

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'c': {
                    char c = (char)__builtin_va_arg(args, int);
                    putchar(c);
                    written++;
                    break;
                }
                case 's': {
                    const char* s = __builtin_va_arg(args, const char*);
                    while (*s) {
                        putchar(*s++);
                        written++;
                    }
                    break;
                }
                case 'd': {
                    int n = __builtin_va_arg(args, int);
                    itoa(n, buffer, 10);
                    for (int j = 0; buffer[j] != '\0'; j++) {
                        putchar(buffer[j]);
                        written++;
                    }
                    break;
                }
                case 'x':
                case 'p': { // Treat pointers as hex for simplicity
                    unsigned long n = __builtin_va_arg(args, unsigned long);
                    utoa_hex(n, buffer);
                    for (int j = 0; buffer[j] != '\0'; j++) {
                        putchar(buffer[j]);
                        written++;
                    }
                    break;
                }
                case '%': {
                    putchar('%');
                    written++;
                    break;
                }
                default: {
                    // Unsupported format specifier, just print it literally.
                    putchar('%');
                    putchar(format[i]);
                    written += 2;
                    break;
                }
            }
        } else {
            putchar(format[i]);
            written++;
        }
    }

    __builtin_va_end(args);
    return written;
}
