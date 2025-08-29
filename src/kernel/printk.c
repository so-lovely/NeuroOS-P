/**
 * @file printk.c
 * @brief Kernel-internal, non-syscall-based printing functions.
 * @author Gemini
 */

#include <kernel/printk.h>
#include <kernel/types.h>
#include <arch/riscv/platform/qemu-virt/uart.h>

// Use __builtin_va_list for variable arguments, which is compiler-specific
// but avoids including a full stdarg.h from a potentially non-existent toolchain libc.
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end

// A simple integer to ascii conversion function.
static void itoa(long long n, char* buffer, int base)
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
        long long rem = n % base;
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


void printk(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[32];

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    uart_putc(c);
                    break;
                }
                case 's': {
                    const char* s = va_arg(args, const char*);
                    while (*s) {
                        uart_putc(*s++);
                    }
                    break;
                }
                case 'd': {
                    int n = va_arg(args, int);
                    itoa(n, buffer, 10);
                    for (int j = 0; buffer[j] != '\0'; j++) {
                        uart_putc(buffer[j]);
                    }
                    break;
                }
                case 'u': {
                    unsigned int n = va_arg(args, unsigned int);
                    itoa(n, buffer, 10);
                    for (int j = 0; buffer[j] != '\0'; j++) {
                        uart_putc(buffer[j]);
                    }
                    break;
                }
                case 'x':
                case 'p': { // Treat pointers as hex for simplicity
                    unsigned long long n = va_arg(args, unsigned long long);
                    itoa(n, buffer, 16);
                    for (int j = 0; buffer[j] != '\0'; j++) {
                        uart_putc(buffer[j]);
                    }
                    break;
                }
                case '%': {
                    uart_putc('%');
                    break;
                }
                default: {
                    uart_putc('%');
                    uart_putc(format[i]);
                    break;
                }
            }
        } else {
            uart_putc(format[i]);
        }
    }

    va_end(args);
}
