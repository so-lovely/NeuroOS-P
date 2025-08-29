/*
 * NeuroOS-P System Call Interface
 * @author Gemini
 */

#ifndef _NEUROOS_P_SYSCALL_H
#define _NEUROOS_P_SYSCALL_H

// 시스템 콜 번호 정의
#define SYS_kputs 1
#define SYS_sleep 2
#define SYS_event_wait 3
#define SYS_event_signal 4

// 순환 참조를 피하기 위해 구조체 전방 선언
struct trapframe;

/**
 * @brief 시스템 콜을 처리하는 메인 함수입니다.
 * 
 * trap_handler에 의해 호출되며, 시스템 콜 번호에 따라
 * 적절한 커널 함수를 실행합니다.
 * @param tf 현재 태스크의 저장된 레지스터 값 (trapframe)
 */
void syscall(struct trapframe *tf);

#endif // _NEUROOS_P_SYSCALL_H
