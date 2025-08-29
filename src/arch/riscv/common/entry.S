/*
 * NeuroOS-P RISC-V 64-bit Entry Point
 * @author Gemini
 */

.section .text.entry
.globl _start

_start:
    # 1. 트랩 핸들러(trap_entry) 주소를 mtvec 레지스터에 설정합니다.
    la t0, trap_entry
    csrw mtvec, t0

    # 2. 초기 스택 포인터(sp) 설정
    la sp, __stack_top

    # 3. .bss 섹션 초기화 (0으로 채우기)
    la a0, __bss_start
    la a1, __bss_end
    bge a0, a1, .L_bss_done

.L_bss_loop:
    sd zero, (a0)
    addi a0, a0, 8
    blt a0, a1, .L_bss_loop

.L_bss_done:

    # 4. C 커널(kmain) 호출
    call kmain

.L_hang:
    j .L_hang
