/**
 * @file kernel.c
 * @brief Main entry point for the NeuroOS-P kernel.
 * @author Gemini
 */

#include <kernel/printk.h>
#include <kernel/kconfig.h>
#include <kernel/types.h>
#include "arch/riscv/common/riscv.h"
#include "arch/riscv/platform/qemu-virt/clint.h"

#include <kernel/memory.h>
#include <kernel/interrupt.h>
#include <kernel/task.h>
#include <kernel/scheduler.h>
#include <kernel/event.h>

// --- External Declarations for Kernel-Internal API ---
extern uint64_t time_get_ticks(void);
extern struct task *current_task;

// --- Test Task Prototypes ---
void waiter_task(void);
void signaler_task(void);

#define TEST_EVENT 1

/**
 * @brief The C entry point for the NeuroOS-P kernel.
 */
void kmain(void)
{
    // Note: uart_init() is called by the boot assembly code.
    printk("\n[NeuroOS-P] Welcome!\n");

    printk("[Kernel] Initializing memory manager...\n");
    memory_init();
    printk("[Kernel] Memory manager initialized.\n");

    printk("[Kernel] Initializing task manager...\n");
    task_init();
    printk("[Kernel] Task manager initialized.\n");

    printk("[Kernel] Initializing scheduler...\n");
    scheduler_init();
    printk("[Kernel] Scheduler initialized.\n");

    printk("[Kernel] Creating event test tasks...\n");
    task_create(waiter_task);
    task_create(signaler_task);
    printk("[Kernel] Test tasks created.\n");

    printk("[Kernel] Initializing interrupt controller...\n");
    interrupt_init();
    printk("[Kernel] Interrupts enabled. Starting scheduler...\n");

    schedule();

    // This part of the code should never be reached if the scheduler works correctly.
    printk("[FATAL] Returned to kmain after scheduler start!\n");
    for(;;);
}

// --- Event Test Tasks (Simulating User-Space) ---
void waiter_task(void)
{
    printk("Waiter: Waiting for event %d...\n", TEST_EVENT);
    sys_event_wait(TEST_EVENT);
    printk("Waiter: Event %d received! Resuming.\n", TEST_EVENT);
    for (;;);
}

void signaler_task(void)
{
    printk("Signaler: Going to sleep for 2 seconds...\n");
    // Directly use kernel functionality instead of syscall wrapper
    uint64_t current_ticks = time_get_ticks();
    current_task->wakeup_tick = current_ticks + 20; // 20 ticks (2s @ 10Hz)
    current_task->state = TASK_STATE_SLEEPING;
    schedule();

    printk("Signaler: Woke up, signaling event %d.\n", TEST_EVENT);
    sys_event_signal(TEST_EVENT);

    // This task runs only once.
    for (;;);
}