/**
 * @file trap.c
 * @brief C-level trap handler for RISC-V.
 * @author Gemini
 */

#include <kernel/printk.h>
#include <kernel/types.h>
#include "arch/riscv/common/riscv.h"
#include "arch/riscv/common/trapframe.h"
#include "arch/riscv/platform/qemu-virt/clint.h"
#include <kernel/syscall.h>
#include <kernel/task.h> // For struct task

// External function and variable declarations
void schedule(void);
void time_tick_handler(void);
extern struct task *current_task;

// RISC-V mcause code definitions
// ----------------------------------------------------------------------------
// If the top bit (XLEN-1) is 1, it's an interrupt. Otherwise, it's an exception.
#define MCAUSE_INTERRUPT_BIT (1UL << 63)

// Interrupt cause codes
#define MCAUSE_MACHINE_TIMER_INTERRUPT (MCAUSE_INTERRUPT_BIT | 7)

// Exception cause codes
#define MCAUSE_ECALL_FROM_M_MODE 11


/**
 * @brief The C function that all traps (interrupts, exceptions) eventually call.
 * 
 * This function is called from the assembly-level trap entry point (trap_entry).
 * It analyzes the cause of the trap (mcause) and calls the appropriate handler.
 */
void trap_handler(struct trapframe *tf)
{
    uint64_t mcause = r_mcause();

    if (mcause == MCAUSE_MACHINE_TIMER_INTERRUPT) {
        // --- Machine Timer Interrupt Handling ---
        time_tick_handler(); // Process the system tick (increment time, check sleeping tasks)
        clint_timer_init();  // Schedule the next timer interrupt.
        schedule();          // Call the scheduler for a forced context switch.
        return; 
    
    } else if (mcause == MCAUSE_ECALL_FROM_M_MODE) {
        // --- System Call Handling ---
        // The ecall instruction is 4 bytes, so increment mepc by 4
        // to continue execution at the instruction after ecall.
        w_mepc(r_mepc() + 4);
        
        // Call the system call dispatcher.
        syscall(tf);
        return;
    }

    // --- Unhandled Exception or Interrupt ---
    printk("\n--- UNHANDLED TRAP ---\n");
    printk(" mcause: 0x%x\n", mcause);
    printk(" mepc:   0x%x\n", r_mepc());
    printk("----------------------\n");

    // Halt the system.
    for(;;);
}