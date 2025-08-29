/*
 * RISC-V Core-Local Interruptor (CLINT) Driver for QEMU
 * @author Gemini
 */

#include "clint.h"
#include "kernel/kconfig.h"

// 디버깅용
void kputs(const char *s);
void kputhex(uint64_t h);

/*
 * QEMU의 'virt' 머신에서 CLINT는 0x2000000 주소에 매핑되어 있습니다.
 * mtime: 현재 시간을 나타내는 64비트 카운터 (읽기 전용)
 * mtimecmp: mtime과 비교하여 인터럽트를 발생시키는 레지스터 (읽기/쓰기)
 */
#define CLINT_BASE   0x2000000L
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))
#define CLINT_MTIME            (CLINT_BASE + 0xBFF8)

// 타이머 인터럽트 간격 설정
static const uint64_t TIMER_INTERVAL = KCONF_CPU_FREQUENCY / KCONF_SYSTICK_HZ;

void clint_timer_init(void)
{
    volatile uint64_t *mtime = (uint64_t *)CLINT_MTIME;
    volatile uint64_t *mtimecmp = (uint64_t *)CLINT_MTIMECMP(0);

    kputs("    CLINT: interval="); kputhex(TIMER_INTERVAL);
    uint64_t current_time = *mtime;
    kputs(", mtime="); kputhex(current_time);
    uint64_t next_interrupt_time = current_time + TIMER_INTERVAL;
    kputs(", next_irq="); kputhex(next_interrupt_time);

    *mtimecmp = next_interrupt_time;

    uint64_t mtimecmp_val = *mtimecmp;
    kputs(", mtimecmp_readback="); kputhex(mtimecmp_val);
    kputs("\n");
}
