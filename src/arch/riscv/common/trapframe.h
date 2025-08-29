/*
 * RISC-V Trap Frame
 * @author Gemini
 */

#ifndef _NEUROOS_P_TRAPFRAME_H
#define _NEUROOS_P_TRAPFRAME_H

#include "kernel/types.h"

// trap_entry에서 스택에 저장하는 모든 레지스터의 레이아웃과 일치해야 합니다.
struct trapframe {
    uint64_t ra;   // x1
    uint64_t sp;   // x2
    uint64_t gp;   // x3
    uint64_t tp;   // x4
    uint64_t t0;   // x5
    uint64_t t1;   // x6
    uint64_t t2;   // x7
    uint64_t s0;   // x8
    uint64_t s1;   // x9
    uint64_t a0;   // x10
    uint64_t a1;   // x11
    uint64_t a2;   // x12
    uint64_t a3;   // x13
    uint64_t a4;   // x14
    uint64_t a5;   // x15
    uint64_t a6;   // x16
    uint64_t a7;   // x17
    uint64_t s2;   // x18
    uint64_t s3;   // x19
    uint64_t s4;   // x20
    uint64_t s5;   // x21
    uint64_t s6;   // x22
    uint64_t s7;   // x23
    uint64_t s8;   // x24
    uint64_t s9;   // x25
    uint64_t s10;  // x26
    uint64_t s11;  // x27
    uint64_t t3;   // x28
    uint64_t t4;   // x29
    uint64_t t5;   // x30
    uint64_t t6;   // x31
};

#endif // _NEUROOS_P_TRAPFRAME_H
