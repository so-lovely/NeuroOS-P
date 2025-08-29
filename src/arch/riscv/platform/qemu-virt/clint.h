#ifndef _NEUROOS_P_QEMU_VIRT_CLINT_H
#define _NEUROOS_P_QEMU_VIRT_CLINT_H

#include "kernel/types.h"

/**
 * @brief CLINT 타이머를 초기화하고 첫 번째 인터럽트를 설정합니다.
 *
 * 이 함수는 부팅 시 한번 호출되어 타이머 인터럽트를 활성화하고,
 * 이후에는 타이머 인터럽트 핸들러 내부에서 다음 인터럽트를 스케줄링하기 위해
 * 호출될 수 있습니다.
 */
void clint_timer_init(void);

#endif // _NEUROOS_P_QEMU_VIRT_CLINT_H
