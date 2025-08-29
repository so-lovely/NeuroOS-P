/*
 * NeuroOS-P Physical Page Frame Allocator
 * @author Gemini
 */

#include "kernel/memory.h"
#include "kernel/kconfig.h"

// 메모리 관리를 위한 내부 자료구조 및 변수
// ----------------------------------------------------------------------------

typedef struct free_page_node {
    struct free_page_node *next;
} free_page_node_t;

static free_page_node_t *free_list_head;

extern char _end[];
extern char __stack_top[];

/**
 * @brief 메모리의 특정 영역을 주어진 값으로 채웁니다. (64-bit 최적화 버전)
 */
static void *memset(void *s, int c, size_t n)
{
    // 64비트 아키텍처에 맞춰 8바이트 단위로 쓰기를 시도합니다.
    uint64_t *p64 = (uint64_t *)s;
    size_t num_qwords = n / 8;

    // 8바이트 패턴 생성
    uint64_t c64 = (unsigned char)c;
    c64 |= c64 << 8;
    c64 |= c64 << 16;
    c64 |= c64 << 32;

    for (size_t i = 0; i < num_qwords; i++) {
        p64[i] = c64;
    }

    // 8바이트로 나누어 떨어지지 않는 나머지 부분을 처리합니다.
    unsigned char *p8 = (unsigned char *)(p64 + num_qwords);
    for (size_t i = 0; i < (n % 8); i++) {
        p8[i] = (unsigned char)c;
    }

    return s;
}

// 페이지 할당자 구현
// ----------------------------------------------------------------------------

void pfree(void *pa)
{
    if (((uint64_t)pa % PAGE_SIZE) != 0 || (char *)pa < _end) {
        return;
    }
    free_page_node_t *node = (free_page_node_t *)pa;
    node->next = free_list_head;
    free_list_head = node;
}

void kputs(const char *s);
void kputhex(uint64_t h);

/**
 * @brief 물리 메모리 관리자를 초기화합니다.
 */
void memory_init(void)
{
    uint64_t pa_start = ((uint64_t)_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    // 스택과의 충돌을 막기 위해 DRAM 최상단에 KCONF_BOOT_STACK_SIZE 만큼의 공간을 예약합니다.
    uint64_t pa_end = (uint64_t)__stack_top - KCONF_BOOT_STACK_SIZE;

    kputs("  _end addr:      "); kputhex((uint64_t)_end); kputs("\n");
    kputs("  pa_start addr:  "); kputhex(pa_start); kputs("\n");
    kputs("  pa_end addr:    "); kputhex(pa_end); kputs(" (Stack guard applied)\n");

    free_list_head = NULL;
    for (uint64_t pa = pa_start; pa + PAGE_SIZE <= pa_end; pa += PAGE_SIZE) {
        pfree((void *)pa);
    }
}

void *palloc(void)
{
    if (!free_list_head) {
        return NULL;
    }
    free_page_node_t *allocated_node = free_list_head;
    free_list_head = free_list_head->next;
    return memset(allocated_node, 0, PAGE_SIZE);
}