/**
 * @file printk.h
 * @brief Kernel-internal, non-syscall-based printing functions.
 * @author Gemini
 */

#ifndef _NEUROOS_P_KERNEL_PRINTK_H
#define _NEUROOS_P_KERNEL_PRINTK_H

/**
 * @brief Prints a formatted string directly to the console, for kernel-internal use.
 * 
 * This function is safe to call from any kernel context, including trap handlers,
 * as it does not use any system calls.
 * It supports a limited set of format specifiers:
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
 */
void printk(const char* format, ...);

#endif // _NEUROOS_P_KERNEL_PRINTK_H
