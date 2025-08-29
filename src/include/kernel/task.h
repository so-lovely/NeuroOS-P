/*
 * NeuroOS-P Task Management
 * @author Gemini
 */

#ifndef _NEUROOS_P_TASK_H
#define _NEUROOS_P_TASK_H

#include "kernel/types.h"
#include "kernel/kconfig.h"

// 문맥(Context) 구조체: 문맥 교환 시 저장되고 복원될 레지스터들
// ----------------------------------------------------------------------------
struct context {
    // 어셈블리 코드(context.S)에서 저장/복원하는 순서와 정확히 일치해야 합니다.
    uint64_t ra; // Return Address (x1)
    uint64_t sp; // Stack Pointer (x2)

    // Callee-saved registers (s0-s11)
    uint64_t s0; // x8
    uint64_t s1; // x9
    uint64_t s2; // x18
    uint64_t s3; // x19
    uint64_t s4; // x20
    uint64_t s5; // x21
    uint64_t s6; // x22
    uint64_t s7; // x23
    uint64_t s8; // x24
    uint64_t s9; // x25
    uint64_t s10; // x26
    uint64_t s11; // x27
};


// 태스크 제어 블록 (Task Control Block - TCB)
// ----------------------------------------------------------------------------
struct task {
    uint64_t id;                // 태스크 고유 ID
    task_state_t state;         // 현재 태스크 상태
    struct context context;     // 레지스터 문맥
    uint64_t *kstack;           // 태스크별 커널 스택 포인터
    uint64_t wakeup_tick;       // 태스크가 깨어나야 할 시스템 틱 카운트
    event_t waiting_event;      // 태스크가 기다리는 이벤트 ID
    // TODO: 우선순위, 부모/자식 관계 등 추가 필드 정의
};

// 태스크 관리 함수 선언
// ----------------------------------------------------------------------------

/**
 * @brief 태스크 서브시스템을 초기화합니다.
 */
void task_init(void);

/**
 * @brief 새로운 태스크를 생성합니다.
 * @param func 태스크가 실행할 함수의 포인터
 * @return 성공 시 생성된 태스크의 포인터, 실패 시 NULL
 */
struct task* task_create(void (*func)(void));


#endif // _NEUROOS_P_TASK_H
