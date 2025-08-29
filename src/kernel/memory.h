#ifndef _NEUROOS_P_MEMORY_H
#define _NEUROOS_P_MEMORY_H

#include "kernel/types.h"

// RISC-V에서 표준 페이지 크기는 4KB (4096 bytes) 입니다.
#define PAGE_SIZE 4096

/**
 * @brief 물리 페이지 프레임 할당자를 초기화합니다.
 *
 * 커널 부팅 시 한번만 호출되어야 하며, 커널 이미지의 끝부터 DRAM의 끝까지를
 * 관리 대상으로 삼습니다.
 */
void memory_init(void);

/**
 * @brief 물리 메모리 페이지 한 개를 할당합니다.
 *
 * 할당된 메모리는 0으로 초기화됩니다.
 *
 * @return 성공 시 할당된 4KB 페이지의 포인터, 실패(메모리 부족) 시 NULL을 반환합니다.
 */
void *palloc(void);

/**
 * @brief 이전에 palloc으로 할당했던 페이지를 해제합니다.
 *
 * @param pa 해제할 페이지의 포인터. palloc으로 할당된 주소여야 합니다.
 */
void pfree(void *pa);

#endif // _NEUROOS_P_MEMORY_H
