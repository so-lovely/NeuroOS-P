/**
 * @file interrupt.c
 * @brief Interrupt subsystem initialization for NeuroOS-P.
 * @author Gemini
 */

#include <kernel/printk.h>
#include <kernel/interrupt.h>
#include "arch/riscv/common/riscv.h"
#include "arch/riscv/platform/qemu-virt/clint.h"

// Address of the assembly-level trap entry function.
extern void trap_entry(void);

/**
 * @brief Initializes the core interrupt handling mechanisms.
 * 
 * This function sets up the machine trap vector (mtvec) to point to the
 * low-level trap entry point and enables machine-level timer interrupts.
 * Global interrupts are enabled last.
 */
void interrupt_init(void)
{
    uint64_t trap_entry_addr = (uint64_t)trap_entry;
    printk("  trap_entry addr: 0x%x\n", trap_entry_addr);

    // 1. Set the trap handler address.
    printk("  1. writing mtvec...\n");
    w_mtvec(trap_entry_addr);

    // 2. Schedule the first timer interrupt (and clear any pending status).
    printk("  2. initializing clint timer...\n");
    clint_timer_init();

    // 3. Enable machine timer interrupts.
    printk("  3. enabling MTIE in mie...\n");
    w_mie(r_mie() | MIE_MTIE);

    // 4. Enable global interrupts (should be done last).
    printk("  4. enabling MIE in mstatus...\n");
    w_mstatus(r_mstatus() | MSTATUS_MIE);

    printk("  -> all interrupts initialized.\n");
}