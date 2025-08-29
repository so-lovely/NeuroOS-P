#ifndef _NEUROOS_P_KCONFIG_H
#define _NEUROOS_P_KCONFIG_H

#include "types.h"

/*
 * NeuroOS-P 커널 전역 설정 (Kernel Configuration)
 *
 * 이 파일은 OS의 핵심 동작과 리소스 할당을 결정하는 주요 파라미터들을 정의합니다.
 * 시스템의 요구사항이나 타겟 하드웨어에 맞게 이 값들을 튜닝할 수 있습니다.
 */

// 1. 태스크(Task) 및 스케줄링 설정
#define KCONF_MAX_TASKS         32      // 시스템에서 생성 가능한 최대 태스크 개수
#define KCONF_KERNEL_STACK_SIZE 4096    // 각 태스크의 커널 스택 크기 (bytes)
#define KCONF_MAX_TASK_PRIORITY 8       // 태스크 우선순위 레벨 (0: highest)

// 2. 이벤트(Event) 큐 설정
// 뉴로모픽 스파이크(Spike)와 같은 비동기 이벤트를 처리하기 위한 큐의 크기입니다.
#define KCONF_EVENT_QUEUE_SIZE  256     // 동시에 큐에 저장될 수 있는 최대 이벤트 개수

// 3. 메모리 설정 (Memory Configuration)
#define KCONF_BOOT_STACK_SIZE   (4 * PAGE_SIZE) // 초기 부팅 스택 크기 (16KB)

// 메모리 레이아웃은 링커 스크립트(linker.ld)에서 최종 결정되지만,
// 커널 코드에서 참조할 수 있는 기본 주소들을 여기에 정의합니다.
// (QEMU 'virt' 머신 기준 가상 주소)
#define KCONF_DRAM_BASE         0x80000000ULL // DRAM 시작 주소

// 4. 시간(Time) 및 시스템 타이머 설정
// 타겟 보드의 클럭 주파수에 따라 변경되어야 합니다.
#define KCONF_CPU_FREQUENCY     10000000UL // CPU 클럭 주파수 (Hz), QEMU 기본값: 10MHz
#define KCONF_SYSTICK_HZ        10UL       // 시스템 타이머 인터럽트 주기 (Hz), 10Hz = 100ms

#endif // _NEUROOS_P_KCONFIG_H
