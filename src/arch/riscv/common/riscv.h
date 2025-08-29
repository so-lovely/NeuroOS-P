#ifndef _NEUROOS_P_RISCV_H
#define _NEUROOS_P_RISCV_H

#include "kernel/types.h"

/*
 * RISC-V 제어 및 상태 레지스터(CSR) 접근을 위한 헬퍼 함수들
 */

// mstatus 레지스터의 MIE(Machine Interrupt Enable) 비트 (3번 비트)
#define MSTATUS_MIE (1L << 3)

// mie 레지스터의 MTIE(Machine Timer Interrupt Enable) 비트 (7번 비트)
#define MIE_MTIE (1UL << 7)

// mip 레지스터의 MTIP(Machine Timer Interrupt Pending) 비트 (7번 비트)
#define MIP_MTIP (1UL << 7)

// mstatus 레지스터 읽기
static inline uint64_t r_mstatus()
{
    uint64_t x;
    asm volatile("csrr %0, mstatus" : "=r" (x));
    return x;
}

// mstatus 레지스터 쓰기
static inline void w_mstatus(uint64_t x)
{
    asm volatile("csrw mstatus, %0" : : "r" (x));
}

// mtvec 레지스터 쓰기 (Machine Trap-Vector Base-Address Register)
// 트랩 핸들러의 주소를 저장합니다.
static inline void w_mtvec(uint64_t x)
{
    asm volatile("csrw mtvec, %0" : : "r" (x));
}

// mtvec 레지스터 읽기
static inline uint64_t r_mtvec()
{
    uint64_t x;
    asm volatile("csrr %0, mtvec" : "=r" (x));
    return x;
}

// mcause 레지스터 읽기 (Machine Cause Register)
// 트랩의 원인을 담고 있습니다.
static inline uint64_t r_mcause()
{
    uint64_t x;
    asm volatile("csrr %0, mcause" : "=r" (x));
    return x;
}

// mepc 레지스터 읽기 (Machine Exception Program Counter)
// 트랩 발생 시점의 명령어 주소를 담고 있습니다.
static inline uint64_t r_mepc()
{
    uint64_t x;
    asm volatile("csrr %0, mepc" : "=r" (x));
    return x;
}

// mepc 레지스터 쓰기
static inline void w_mepc(uint64_t x)
{
    asm volatile("csrw mepc, %0" : : "r" (x));
}

// mie 레지스터 읽기 (Machine Interrupt Enable)
static inline uint64_t r_mie()
{
    uint64_t x;
    asm volatile("csrr %0, mie" : "=r" (x));
    return x;
}

// mie 레지스터 쓰기
static inline void w_mie(uint64_t x)
{
    asm volatile("csrw mie, %0" : : "r" (x));
}

// mip 레지스터 읽기 (Machine Interrupt Pending)
static inline uint64_t r_mip()
{
    uint64_t x;
    asm volatile("csrr %0, mip" : "=r" (x));
    return x;
}

// --- Interrupt Control ---

// 전역 인터럽트를 비활성화합니다.
static inline void intr_off()
{
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);
}

// 전역 인터럽트를 활성화합니다.
static inline void intr_on()
{
    w_mstatus(r_mstatus() | MSTATUS_MIE);
}

#endif // _NEUROOS_P_RISCV_H
