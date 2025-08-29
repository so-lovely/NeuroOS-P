#
# NeuroOS-P Context Switching and Trap Entry
#

.section .text
.globl trap_entry
.globl swtch

#
# swtch(old_context_ptr, new_context_ptr):
# C에서 호출하는 문맥 교환 함수입니다.
# old_context_ptr (a0): 현재 태스크의 context 구조체 포인터 주소
# new_context_ptr (a1): 다음 태스크의 context 구조체 포인터 주소
#
# struct context의 오프셋:
# ra: 0, sp: 8, s0: 16, s1: 24, ..., s11: 104
#
swtch:
    # 1. 현재 문맥(Callee-saved registers)을 old_context에 저장합니다.
    sd ra, 0(a0)   # ra (return address)
    sd sp, 8(a0)   # sp (stack pointer)
    sd s0, 16(a0)  # s0 (x8)
    sd s1, 24(a0)  # s1 (x9)
    sd s2, 32(a0)  # s2 (x18)
    sd s3, 40(a0)  # s3 (x19)
    sd s4, 48(a0)  # s4 (x20)
    sd s5, 56(a0)  # s5 (x21)
    sd s6, 64(a0)  # s6 (x22)
    sd s7, 72(a0)  # s7 (x23)
    sd s8, 80(a0)  # s8 (x24)
    sd s9, 88(a0)  # s9 (x25)
    sd s10, 96(a0) # s10 (x26)
    sd s11, 104(a0)# s11 (x27)

    # 2. 다음 태스크의 문맥을 new_context로부터 복원합니다.
    ld ra, 0(a1)
    ld sp, 8(a1)
    ld s0, 16(a1)
    ld s1, 24(a1)
    ld s2, 32(a1)
    ld s3, 40(a1)
    ld s4, 48(a1)
    ld s5, 56(a1)
    ld s6, 64(a1)
    ld s7, 72(a1)
    ld s8, 80(a1)
    ld s9, 88(a1)
    ld s10, 96(a1)
    ld s11, 104(a1)

    # 3. 새로운 태스크가 인터럽트를 받을 수 있도록 전역 인터럽트를 활성화합니다.
    # MSTATUS_MIE 비트(3번 비트, 값 8)를 1로 설정합니다.
    csrsi mstatus, 8

    # 4. 복원된 ra 주소로 리턴합니다. (새로운 태스크의 실행 흐름으로 점프)
    ret


#
# trap_entry:
#
# 트랩(인터럽트/예외) 발생 시 mtvec이 가리키는 진입점입니다.
#
# 1. 모든 레지스터를 스택에 저장 (struct trapframe 생성)
# 2. trapframe의 주소(sp)를 a0에 담아 C 함수인 trap_handler()를 호출
# 3. 스택에서 모든 레지스터를 복원
# 4. mret 명령으로 트랩에서 복귀
#
.align 4
trap_entry:
    # 스택에 31개 레지스터(x1-x31)를 저장할 공간(248 bytes)을 할당합니다.
    addi sp, sp, -248

    # struct trapframe의 레이아웃에 맞춰 레지스터를 저장합니다.
    sd ra, 0(sp)   # x1
    # sp(x2)는 나중에 계산해서 저장
    sd gp, 16(sp)  # x3
    sd tp, 24(sp)  # x4
    sd t0, 32(sp)  # x5
    sd t1, 40(sp)  # x6
    sd t2, 48(sp)  # x7
    sd s0, 56(sp)  # x8
    sd s1, 64(sp)  # x9
    sd a0, 72(sp)  # x10
    sd a1, 80(sp)  # x11
    sd a2, 88(sp)  # x12
    sd a3, 96(sp)  # x13
    sd a4, 104(sp) # x14
    sd a5, 112(sp) # x15
    sd a6, 120(sp) # x16
    sd a7, 128(sp) # x17
    sd s2, 136(sp) # x18
    sd s3, 144(sp) # x19
    sd s4, 152(sp) # x20
    sd s5, 160(sp) # x21
    sd s6, 168(sp) # x22
    sd s7, 176(sp) # x23
    sd s8, 184(sp) # x24
    sd s9, 192(sp) # x25
    sd s10, 200(sp)# x26
    sd s11, 208(sp)# x27
    sd t3, 216(sp) # x28
    sd t4, 224(sp) # x29
    sd t5, 232(sp) # x30
    sd t6, 240(sp) # x31

    # 트랩 진입 전의 원래 sp 값을 계산하여 저장합니다.
    addi t0, sp, 248
    sd t0, 8(sp)

    # C 함수 호출 후에도 trapframe 시작 주소를 알 수 있도록
    # 현재 sp(trapframe의 시작 주소)를 callee-saved 레지스터 s0에 백업합니다.
    mv s0, sp

    # C 핸들러 호출. 첫번째 인자(a0)로 trapframe의 주소(sp)를 전달합니다.
    mv a0, sp
    call trap_handler

    # C 함수 호출로 sp가 변경되었을 수 있으므로, 백업해둔 s0로부터 sp를 복원합니다.
    mv sp, s0

    # 모든 레지스터를 복원합니다. sp는 가장 마지막에 복원해야 합니다.
    ld ra, 0(sp)
    ld gp, 16(sp)
    ld tp, 24(sp)
    ld t0, 32(sp)
    ld t1, 40(sp)
    ld t2, 48(sp)
    ld s0, 56(sp)
    ld s1, 64(sp)
    ld a0, 72(sp)
    ld a1, 80(sp)
    ld a2, 88(sp)
    ld a3, 96(sp)
    ld a4, 104(sp)
    ld a5, 112(sp)
    ld a6, 120(sp)
    ld a7, 128(sp)
    ld s2, 136(sp)
    ld s3, 144(sp)
    ld s4, 152(sp)
    ld s5, 160(sp)
    ld s6, 168(sp)
    ld s7, 176(sp)
    ld s8, 184(sp)
    ld s9, 192(sp)
    ld s10, 200(sp)
    ld s11, 208(sp)
    ld t3, 216(sp)
    ld t4, 224(sp)
    ld t5, 232(sp)
    ld t6, 240(sp)

    # 모든 레지스터 복원이 끝난 후, 마지막으로 sp를 복원합니다.
    ld sp, 8(sp)

    # Machine 모드 트랩에서 복귀합니다.
    mret
