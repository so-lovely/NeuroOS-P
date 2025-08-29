/*
 * NeuroOS-P Simple Round-Robin Scheduler
 * @author Gemini
 */

#include <kernel/scheduler.h>
#include <kernel/task.h>

// 외부에서 정의된 TCB 배열과 swtch 함수 참조
extern struct task tasks[KCONF_MAX_TASKS];
extern void swtch(struct context *old, struct context *new);

// 현재 실행 중인 태스크의 TCB 포인터 (전역 변수)
struct task *current_task = NULL;

/**
 * @brief 스케줄러를 초기화합니다.
 * 
 * tasks[0]을 커널 메인 프로세스를 위한 TCB로 설정하고, 현재 실행 태스크로 지정합니다.
 */
void scheduler_init(void)
{
    // tasks[0]은 kmain의 실행 흐름을 나타내는 초기 태스크로 사용합니다.
    current_task = &tasks[0];
    current_task->state = TASK_STATE_RUNNING;
    current_task->id = 0; // ID 0은 특별한 커널 태스크
    current_task->kstack = NULL; // 커널 스택은 이미 entry.S에서 설정됨
}

/**
 * @brief 스케줄러를 실행하여, 다음에 실행할 태스크를 찾아 문맥을 교환합니다.
 */
void schedule(void)
{
    struct task *next_task = NULL;
    int start_idx = 0;

    // 1. 현재 태스크의 배열 인덱스를 찾습니다.
    for (int i = 0; i < KCONF_MAX_TASKS; i++) {
        if (&tasks[i] == current_task) {
            start_idx = i;
            break;
        }
    }

    // 2. 현재 태스크의 다음 위치부터 순회하며 READY 상태의 태스크를 찾습니다.
    for (int i = 1; i < KCONF_MAX_TASKS; i++) {
        int idx = (start_idx + i) % KCONF_MAX_TASKS;
        if (idx == 0) continue; // 0번 커널 태스크는 건너뜁니다.

        if (tasks[idx].state == TASK_STATE_READY) {
            next_task = &tasks[idx];
            break;
        }
    }

    if (next_task == NULL) {
        // 실행할 태스크가 없으면 현재 태스크를 계속 실행합니다.
        return;
    }

    // 3. 문맥 교환을 준비하고 태스크 상태를 업데이트합니다.
    struct task *prev_task = current_task;
    current_task = next_task;
    
    if (prev_task->state == TASK_STATE_RUNNING) {
        prev_task->state = TASK_STATE_READY;
    }
    current_task->state = TASK_STATE_RUNNING;

    // 4. 문맥 교환!
    swtch(&prev_task->context, &current_task->context);
}