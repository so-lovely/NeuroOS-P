/*
 * NeuroOS-P Kernel Main Entry
 * 
 * @author Gemini
 */

#include "kernel/kconfig.h"
#include "kernel/types.h"
#include "arch/riscv/common/riscv.h"
#include "arch/riscv/platform/qemu-virt/uart.h"
#include "arch/riscv/platform/qemu-virt/clint.h"

#include "kernel/memory.h"
#include "kernel/interrupt.h"
#include "kernel/task.h"
#include "kernel/scheduler.h"
#include "include/user.h" // sys_kputs를 사용하기 위해 추가

// 디버그 출력 함수를 UART 출력으로 연결
void kputhex(uint64_t h);
void kputs(const char *h);

// 테스트용 태스크 함수 선언
void waiter_task(void);
void signaler_task(void);

#define TEST_EVENT 1

/**
 * @brief NeuroOS-P 커널의 C 코드 진입점 (Entry Point)
 */
void kmain(void)
{
    uart_init();
    kputs("\n[NeuroOS-P] Welcome!\n");

    kputs("[Kernel] Initializing memory manager...\n");
    memory_init();
    kputs("[Kernel] Memory manager initialized.\n");

    kputs("[Kernel] Initializing task manager...\n");
    task_init();
    kputs("[Kernel] Task manager initialized.\n");

    kputs("[Kernel] Initializing scheduler...\n");
    scheduler_init();
    kputs("[Kernel] Scheduler initialized.\n");

    kputs("[Kernel] Creating event test tasks...\n");
    task_create(waiter_task);
    task_create(signaler_task);
    kputs("[Kernel] Test tasks created.\n");

    kputs("[Kernel] Initializing interrupt controller...\n");
    // 모든 서브시스템이 준비된 후, 마지막에 인터럽트를 활성화합니다.
    w_mstatus(r_mstatus() | MSTATUS_MIE); // 전역 인터럽트 활성화
    w_mie(r_mie() | MIE_MTIE);         // 머신 타이머 인터럽트 활성화
    clint_timer_init();                  // 첫 타이머 인터럽트 예약
    kputs("[Kernel] Interrupts enabled. Starting scheduler...\n");

    schedule();

    // 이 아래 코드는 스케줄러가 정상 동작하면 절대 실행되지 않아야 합니다.
    kputs("[FATAL] Returned to kmain after scheduler start!\n");
    for(;;);
}

// --- 이벤트 테스트 태스크 ---
void waiter_task(void)
{
    kputs("Waiter: Waiting for event 1...\n");
    event_wait(TEST_EVENT);
    kputs("Waiter: Event 1 received! Resuming.\n");
    for (;;);
}

void signaler_task(void)
{
    kputs("Signaler: Going to sleep for 2 seconds...\n");
    sleep(20); // 2초 대기

    kputs("Signaler: Woke up, signaling event 1.\n");
    event_signal(TEST_EVENT);

    // 한번만 실행하고 종료
    for (;;);
}


void kputs(const char *s)
{
    uart_puts(s);
}

// 64비트 정수를 16진수 문자열로 출력하는 헬퍼 함수
void kputhex(uint64_t h)
{
    char *hex = "0123456789abcdef";
    char buf[17];
    buf[16] = '\0';
    for (int i = 15; i >= 0; i--) {
        buf[i] = hex[h & 0xF];
        h >>= 4;
    }
    kputs("0x");
    kputs(buf);
}
