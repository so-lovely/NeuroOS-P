/*
 * NeuroOS-P Global Type Definitions
 * @author Gemini
 */

#ifndef _NEUROOS_P_TYPES_H
#define _NEUROOS_P_TYPES_H

// 기본 데이터 타입 정의
// ----------------------------------------------------------------------------
typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef uint64_t size_t;

// NULL 포인터 정의
#define NULL ((void *)0)

// 페이지 크기 정의 (RISC-V 기본 페이지 크기)
#define PAGE_SIZE 4096

// 이벤트 ID 타입
typedef uint32_t event_t;

// 태스크 상태 (Task State)
// ----------------------------------------------------------------------------
typedef enum {
    TASK_STATE_UNUSED,   // 사용되지 않는 TCB 슬롯
    TASK_STATE_READY,    // 실행 대기 중인 상태
    TASK_STATE_RUNNING,  // 현재 실행 중인 상태
    TASK_STATE_SLEEPING, // 시간 지연으로 잠든 상태 (타이머 대기)
    TASK_STATE_WAITING,  // 특정 이벤트를 기다리는 상태
    TASK_STATE_ZOMBIE,   // 종료되었지만, 아직 자원이 정리되지 않은 상태
} task_state_t;


#endif // _NEUROOS_P_TYPES_H