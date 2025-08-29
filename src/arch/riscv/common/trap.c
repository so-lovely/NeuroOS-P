/*
 * RISC-V C-level Trap Handler
 * @author Gemini
 */

#include "kernel/types.h"
#include "arch/riscv/common/riscv.h"
#include "arch/riscv/common/trapframe.h"
#include "arch/riscv/platform/qemu-virt/clint.h"
#include "kernel/syscall.h"
#include "kernel/task.h" // struct task를 사용하기 위해 추가

// 외부 함수 및 변수 선언
void kputs(const char *s);
void kputhex(uint64_t h);
void schedule(void);
void time_tick_handler(void);
extern struct task *current_task;

// RISC-V에서 사용하는 mcause 코드 정의
// ----------------------------------------------------------------------------
// 최상위 비트(XLEN-1)가 1이면 인터럽트, 0이면 예외입니다.
#define MCAUSE_INTERRUPT_BIT (1UL << 63)

// 인터럽트 원인 코드
#define MCAUSE_MACHINE_TIMER_INTERRUPT (MCAUSE_INTERRUPT_BIT | 7)

// 예외 원인 코드
#define MCAUSE_ECALL_FROM_M_MODE 11


/**
 * @brief 모든 종류의 트랩(인터럽트, 예외)이 최종적으로 호출하는 C 함수입니다.
 * 
 * 이 함수는 어셈블리 레벨의 트랩 진입점(trap_entry)에서 호출됩니다.
 * 트랩의 원인(mcause)을 분석하여 적절한 핸들러를 호출합니다.
 */
void trap_handler(struct trapframe *tf)
{
    uint64_t mcause = r_mcause();

    if (mcause == MCAUSE_MACHINE_TIMER_INTERRUPT) {
        // --- 머신 타이머 인터럽트 처리 ---
        time_tick_handler(); // 시스템 틱을 처리 (시간 증가, 슬립 태스크 확인)
        clint_timer_init();  // 다음 타이머 인터럽트를 예약합니다.
        schedule();          // 스케줄러를 호출하여 강제 문맥 교환
        return; 
    
    } else if (mcause == MCAUSE_ECALL_FROM_M_MODE) {
        // --- 시스템 콜 처리 ---
        // ecall 명령어는 4바이트이므로, mepc를 4 증가시켜
        // ecall 다음 명령어부터 실행되도록 합니다.
        w_mepc(r_mepc() + 4);
        
        // 시스템 콜 디스패처를 호출합니다.
        syscall(tf);
        return;
    }

    // --- 처리되지 않은 예외 또는 인터럽트 ---
    kputs("\n--- UNHANDLED TRAP ---");
    kputs("\n mcause: "); kputhex(mcause);
    kputs("\n mepc:   "); kputhex(r_mepc());
    kputs("\n----------------------\n");

    // 시스템을 멈춥니다.
    for(;;);
}
