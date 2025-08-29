/*
 * NeuroOS-P Interrupt Subsystem
 * @author Gemini
 */

#include "kernel/interrupt.h"
#include "arch/riscv/common/riscv.h"
#include "arch/riscv/platform/qemu-virt/clint.h"

// 디버깅용
void kputs(const char *s);
void kputhex(uint64_t h);

// 어셈블리에 정의될 트랩 진입점 함수의 주소
extern void trap_entry(void);

void interrupt_init(void)
{
    uint64_t trap_entry_addr = (uint64_t)trap_entry;
    kputs("  trap_entry addr: "); kputhex(trap_entry_addr); kputs("\n");

    // 1. 트랩 핸들러 주소 설정
    kputs("  1. writing mtvec...\n");
    w_mtvec(trap_entry_addr);

    // 2. 첫 타이머 인터럽트 예약 (및 기존 상태 클리어)
    kputs("  2. initializing clint timer...\n");
    clint_timer_init();

    // 3. 머신 타이머 인터럽트 활성화
    kputs("  3. enabling MTIE in mie...\n");
    w_mie(r_mie() | MIE_MTIE);

    // 4. 전역 인터럽트 활성화 (가장 마지막에 수행)
    kputs("  4. enabling MIE in mstatus...\n");
    w_mstatus(r_mstatus() | MSTATUS_MIE);

    kputs("  -> all interrupts initialized.\n");
}

