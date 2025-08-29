/*
 * NeuroOS-P System Call Dispatcher
 * @author Gemini
 */

#include "kernel/types.h"
#include "arch/riscv/common/trapframe.h"
#include "kernel/syscall.h"
#include "kernel/task.h"
#include "kernel/event.h"

// 외부 참조
void kputs(const char *s);
void kputhex(uint64_t h);
void schedule(void);
uint64_t time_get_ticks(void);
extern struct task *current_task;

/**
 * @brief 현재 태스크를 지정된 틱 수만큼 잠들게 합니다.
 * @param ticks 잠들 틱의 수
 */
void sys_sleep(uint64_t ticks)
{
    uint64_t current_ticks = time_get_ticks();
    current_task->wakeup_tick = current_ticks + ticks;
    current_task->state = TASK_STATE_SLEEPING;
    schedule();
}

/**
 * @brief 시스템 콜을 처리합니다.
 * @param tf 시스템 콜을 호출한 태스크의 저장된 trapframe
 */
void syscall(struct trapframe *tf)
{
    // a7 레지스터에 저장된 시스템 콜 번호를 가져옵니다.
    uint64_t syscall_num = tf->a7;

    switch (syscall_num) {
    case SYS_kputs:
        // a0 레지스터에 저장된 문자열 포인터 인자를 가져와 kputs를 호출합니다.
        kputs((const char *)tf->a0);
        break;
    case SYS_sleep:
        // a0 레지스터에 저장된 틱 수를 인자로 가져와 sys_sleep을 호출합니다.
        sys_sleep(tf->a0);
        break;
    case SYS_event_wait:
        sys_event_wait((event_t)tf->a0);
        break;
    case SYS_event_signal:
        sys_event_signal((event_t)tf->a0);
        break;
    default:
        kputs("Unknown system call: ");
        kputhex(syscall_num);
        kputs("\n");
        break;
    }
}


