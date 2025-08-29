/*
 * NeuroOS-P Time Management
 * @author Gemini
 */

#include "kernel/types.h"
#include "kernel/task.h"
#include "kernel/kconfig.h"

// 외부 TCB 배열 참조
extern struct task tasks[KCONF_MAX_TASKS];

// 시스템 부팅 후 발생한 총 틱(tick) 수
static uint64_t g_ticks = 0;

/**
 * @brief 현재 시스템 틱 카운트를 반환합니다.
 */
uint64_t time_get_ticks(void)
{
    return g_ticks;
}

/**
 * @brief 시스템 틱을 처리합니다. (타이머 인터럽트 핸들러에서 호출됨)
 * 
 * 1. 전역 틱 카운터를 증가시킵니다.
 * 2. 잠들어 있는(sleeping) 태스크들을 확인하여, 깨어날 시간이 되었다면
 *    상태를 READY로 변경합니다.
 */
void time_tick_handler(void)
{
    g_ticks++;

    // 모든 태스크를 순회하며 깨울 태스크가 있는지 확인합니다.
    for (int i = 0; i < KCONF_MAX_TASKS; i++) {
        if (tasks[i].state == TASK_STATE_SLEEPING) {
            if (g_ticks >= tasks[i].wakeup_tick) {
                // 깨어날 시간이 된 태스크를 READY 상태로 변경
                tasks[i].state = TASK_STATE_READY;
            }
        }
    }
}
