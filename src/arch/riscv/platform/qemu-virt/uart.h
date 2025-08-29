#ifndef _NEUROOS_P_QEMU_VIRT_UART_H
#define _NEUROOS_P_QEMU_VIRT_UART_H

/**
 * @brief UART 하드웨어를 초기화합니다.
 */
void uart_init(void);

/**
 * @brief UART를 통해 한 글자를 전송합니다.
 *
 * @param c 전송할 문자
 */
void uart_putc(char c);

/**
 * @brief UART를 통해 NULL로 끝나는 문자열을 전송합니다.
 *
 * @param s 전송할 문자열
 */
void uart_puts(const char *s);

#endif // _NEUROOS_P_QEMU_VIRT_UART_H
