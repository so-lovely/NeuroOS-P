/*
 * NeuroOS-P Event Signaling
 * @author Gemini
 */

#include <kernel/event.h>
#include <kernel/task.h>
#include <kernel/scheduler.h>
#include "arch/riscv/common/riscv.h"

// 외부 TCB 배열 및 현재 태스크 포인터 참조
extern struct task tasks[KCONF_MAX_TASKS];
extern struct task *current_task;

/**
 * @brief Wait for an event to be signaled. (Kernel-side implementation)
 *
 * This function will block the calling task until another task
 * signals the specified event.
 *
 * @param event The event ID to wait for.
 */
void sys_event_wait(event_t event)
{
    // 인터럽트를 비활성화하여 임계 영역을 보호합니다.
    uint64_t old_mstatus = r_mstatus();
    w_mstatus(old_mstatus & ~MSTATUS_MIE);

    current_task->waiting_event = event;
    current_task->state = TASK_STATE_WAITING;

    // 스케줄러를 호출하여 다른 태스크에 제어권을 양보합니다.
    schedule();

    // 태스크가 다시 깨어난 후, 이전 인터럽트 상태를 복원합니다.
    w_mstatus(old_mstatus);
}

/**
 * @brief Signal an event. (Kernel-side implementation)
 *
 * This function will wake up all tasks that are currently waiting
 * for the specified event.
 *
 * @param event The event ID to signal.
 */
void sys_event_signal(event_t event)
{
    // 인터럽트를 비활성화하여 임계 영역을 보호합니다.
    uint64_t old_mstatus = r_mstatus();
    w_mstatus(old_mstatus & ~MSTATUS_MIE);

    for (int i = 0; i < KCONF_MAX_TASKS; i++) {
        if (tasks[i].state == TASK_STATE_WAITING && tasks[i].waiting_event == event) {
            tasks[i].state = TASK_STATE_READY;
        }
    }

    // 새로 깨어난 태스크가 즉시 실행될 수 있도록 스케줄러를 호출합니다.
    schedule();

    // 이전 인터럽트 상태를 복원합니다.
    w_mstatus(old_mstatus);
}