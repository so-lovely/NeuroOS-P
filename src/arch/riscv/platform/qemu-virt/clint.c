/**
 * @file clint.c
 * @brief RISC-V Core-Local Interruptor (CLINT) driver for QEMU.
 * @author Gemini
 */

#include <kernel/printk.h>
#include "clint.h"
#include <kernel/kconfig.h>

/*
 * In QEMU's 'virt' machine, the CLINT is mapped to address 0x2000000.
 * mtime: 64-bit counter representing the current time (read-only).
 * mtimecmp: Register to trigger an interrupt when mtime matches (read/write).
 */
#define CLINT_BASE   0x2000000L
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))
#define CLINT_MTIME            (CLINT_BASE + 0xBFF8)

// The interval for timer interrupts.
static const uint64_t TIMER_INTERVAL = KCONF_CPU_FREQUENCY / KCONF_SYSTICK_HZ;

void clint_timer_init(void)
{
    volatile uint64_t *mtime = (uint64_t *)CLINT_MTIME;
    volatile uint64_t *mtimecmp = (uint64_t *)CLINT_MTIMECMP(0);

    uint64_t current_time = *mtime;
    uint64_t next_interrupt_time = current_time + TIMER_INTERVAL;

    *mtimecmp = next_interrupt_time;

    // This print is for debugging and can be removed in production.
    // printk("    CLINT: interval=0x%x, mtime=0x%x, next_irq=0x%x, mtimecmp_readback=0x%x\n",
    //        TIMER_INTERVAL, current_time, next_interrupt_time, *mtimecmp);
}