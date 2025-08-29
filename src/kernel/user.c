/*
 * NeuroOS-P User-facing System Call Wrappers
 * @author Gemini
 */

#include "kernel/types.h"
#include "kernel/syscall.h"
#include "include/user.h"

void sys_kputs(const char *s)
{
    // RISC-V 시스템 콜 규약:
    // a7: 시스템 콜 번호
    // a0-a6: 인자
    asm volatile(
        "li a7, %0\n"
        "mv a0, %1\n"
        "ecall\n"
        : 
        : "i" (SYS_kputs), "r" (s)
        : "a7", "a0"
    );
}

void sleep(uint64_t ticks)
{
    asm volatile(
        "li a7, %0\n"
        "mv a0, %1\n"
        "ecall\n"
        : 
        : "i" (SYS_sleep), "r" (ticks)
        : "a7", "a0"
    );
}

void event_wait(event_t event)
{
    asm volatile(
        "li a7, %0\n"
        "mv a0, %1\n"
        "ecall\n"
        : 
        : "i" (SYS_event_wait), "r" (event)
        : "a7", "a0"
    );
}

void event_signal(event_t event)
{
    asm volatile(
        "li a7, %0\n"
        "mv a0, %1\n"
        "ecall\n"
        : 
        : "i" (SYS_event_signal), "r" (event)
        : "a7", "a0"
    );
}
// sleep 시스콜 새로만듬
