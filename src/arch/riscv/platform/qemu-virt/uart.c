/*
 * NS16550A UART Driver for QEMU virt machine
 * @author Gemini
 */

#include <kernel/types.h>
#include "uart.h"

/*
 * QEMU 'virt' 머신에서 NS16550A UART는 0x10000000 주소에 매핑되어 있습니다.
 * 각 레지스터는 이 기본 주소로부터의 오프셋으로 접근합니다.
 */
#define UART_BASE 0x10000000L

// Transmitter Holding Buffer (쓰기 전용)
#define UART_THR (volatile uint8_t *)(UART_BASE + 0x00)
// Line Status Register (읽기 전용)
#define UART_LSR (volatile uint8_t *)(UART_BASE + 0x05)

// LSR의 5번째 비트: Transmitter Holding Register Empty (THRE)
// 이 비트가 1이면 UART가 다음 문자를 전송할 준비가 되었음을 의미합니다.
#define LSR_THRE (1 << 5)

void uart_init(void)
{
    // QEMU의 UART는 별도의 초기화 없이 기본 설정으로 바로 사용할 수 있습니다.
    // 복잡한 설정(Baud rate, Parity 등)이 필요할 경우 여기에 코드를 추가합니다.
}

void uart_putc(char c)
{
    // LSR의 THRE 비트가 1이 될 때까지 대기합니다.
    // 즉, UART 전송 버퍼가 빌 때까지 기다립니다.
    while ((*UART_LSR & LSR_THRE) == 0);

    // 버퍼가 비었으면, 전송할 문자를 THR 레지스터에 씁니다.
    *UART_THR = c;
}

void uart_puts(const char *s)
{
    if (s == NULL) return;
    while (*s != '\0') {
        uart_putc(*s);
        s++;
    }
}