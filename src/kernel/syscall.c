/**
 * @file syscall.c
 * @brief System call dispatcher for NeuroOS-P.
 * @author Gemini
 */

#include <kernel/printk.h>
#include <kernel/types.h>
#include "arch/riscv/common/trapframe.h"
#include <kernel/syscall.h>
#include <kernel/task.h>
#include <kernel/event.h>
#include "arch/riscv/platform/qemu-virt/uart.h" // For uart_putc

// External function and variable declarations
void schedule(void);
uint64_t time_get_ticks(void);
extern struct task *current_task;

/**
 * @brief Puts the current task to sleep for a specified number of ticks.
 * @param ticks The number of ticks to sleep.
 */
void sys_sleep(uint64_t ticks)
{
    uint64_t current_ticks = time_get_ticks();
    current_task->wakeup_tick = current_ticks + ticks;
    current_task->state = TASK_STATE_SLEEPING;
    schedule();
}

/**
 * @brief Writes a single character to the console.
 * @param c The character to write.
 */
void sys_putchar(char c)
{
    uart_putc(c);
}

/**
 * @brief Handles system calls.
 * @param tf The trapframe of the task that invoked the syscall.
 */
void syscall(struct trapframe *tf)
{
    // The syscall number is passed in the a7 register.
    uint64_t syscall_num = tf->a7;

    switch (syscall_num) {
    case SYS_sleep:
        // The number of ticks is passed in a0.
        sys_sleep(tf->a0);
        break;
    case SYS_event_wait:
        sys_event_wait((event_t)tf->a0);
        break;
    case SYS_event_signal:
        sys_event_signal((event_t)tf->a0);
        break;
    case SYS_putchar:
        sys_putchar((char)tf->a0);
        break;
    default:
        printk("Unknown system call: %d\n", syscall_num);
        break;
    }
}