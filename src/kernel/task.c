/*
 * NeuroOS-P Task Management
 * @author Gemini
 */

#include "kernel/task.h"
#include "kernel/memory.h"

// 모든 태스크의 TCB를 저장하는 배열
struct task tasks[KCONF_MAX_TASKS];

// 다음에 할당할 태스크 ID
static uint64_t next_pid = 1;

/**
 * @brief 태스크 관리자 서브시스템을 초기화합니다.
 * 
 * 모든 TCB의 상태를 UNUSED로 설정합니다.
 * 이 함수는 kmain에서 커널 초기화 과정 중 한번만 호출되어야 합니다.
 */
void task_init(void)
{
    // tasks[0]은 scheduler_init에서 커널 자신을 위해 사용하므로 초기화하지 않습니다.
    for (int i = 1; i < KCONF_MAX_TASKS; i++) {
        tasks[i].state = TASK_STATE_UNUSED;
    }
}

/**
 * @brief 새로운 태스크를 생성합니다.
 * 
 * @param func 태스크가 실행을 시작할 함수의 포인터
 * @return 성공 시 생성된 태스크의 포인터, 실패 시 NULL
 */
struct task* task_create(void (*func)(void))
{
    struct task *t = NULL;

    // 1. 비어있는 TCB 슬롯을 찾습니다. (0번은 커널용이므로 1번부터)
    for (int i = 1; i < KCONF_MAX_TASKS; i++) {
        if (tasks[i].state == TASK_STATE_UNUSED) {
            t = &tasks[i];
            break;
        }
    }

    if (t == NULL) {
        return NULL; // 사용 가능한 TCB가 없음
    }

    // 2. 태스크를 위한 커널 스택을 할당합니다.
    t->kstack = palloc();
    if (t->kstack == NULL) {
        t->state = TASK_STATE_UNUSED; // 할당 실패 시 TCB 롤백
        return NULL;
    }

    // 3. TCB의 상태를 초기화합니다.
    t->id = next_pid++;
    t->state = TASK_STATE_READY;

    // 4. 태스크의 초기 문맥(context)을 설정합니다.
    // 스택은 높은 주소에서 낮은 주소로 자라므로, 스택의 최상단을 가리키도록 설정합니다.
    t->context.sp = (uint64_t)t->kstack + PAGE_SIZE;
    // 태스크가 처음 시작될 때 복귀할 주소(ra)는 태스크 함수 자체입니다.
    t->context.ra = (uint64_t)func;

    return t;
}
