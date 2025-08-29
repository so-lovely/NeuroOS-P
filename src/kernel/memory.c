/**
 * @file memory.c
 * @brief Physical page frame allocator for NeuroOS-P.
 * @author Gemini
 */

#include <kernel/printk.h>
#include <string.h>
#include <kernel/memory.h>
#include <kernel/kconfig.h>

// --- Internal Data Structures and Variables ---

typedef struct free_page_node {
    struct free_page_node *next;
} free_page_node_t;

static free_page_node_t *free_list_head;

// Symbols defined in the linker script.
extern char _end[];
extern char __stack_top[];


// --- Page Allocator Implementation ---

/**
 * @brief Frees a physical page, adding it to the free list.
 * @param pa The physical address of the page to free.
 */
void pfree(void *pa)
{
    // Basic sanity checks.
    if (((uint64_t)pa % PAGE_SIZE) != 0 || (char *)pa < _end) {
        return;
    }
    free_page_node_t *node = (free_page_node_t *)pa;
    node->next = free_list_head;
    free_list_head = node;
}

/**
 * @brief Initializes the physical memory manager.
 * 
 * This function builds a free list of all available physical pages,
 * starting from the end of the kernel image up to the start of the
 * initial boot stack.
 */
void memory_init(void)
{
    // Align the start of the heap to the next page boundary.
    uint64_t pa_start = ((uint64_t)_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    
    // Reserve space at the top of DRAM for the boot stack to prevent collision.
    uint64_t pa_end = (uint64_t)__stack_top - KCONF_BOOT_STACK_SIZE;

    printk("  _end addr:      0x%x\n", (uint64_t)_end);
    printk("  pa_start addr:  0x%x\n", pa_start);
    printk("  pa_end addr:    0x%x (Stack guard applied)\n", pa_end);

    free_list_head = NULL;
    for (uint64_t pa = pa_start; pa + PAGE_SIZE <= pa_end; pa += PAGE_SIZE) {
        pfree((void *)pa);
    }
}

/**
 * @brief Allocates a single physical page.
 * @return A pointer to the allocated page, or NULL if no memory is available.
 */
void *palloc(void)
{
    if (!free_list_head) {
        return NULL;
    }
    free_page_node_t *allocated_node = free_list_head;
    free_list_head = free_list_head->next;
    
    // Zero out the page before returning it.
    return memset(allocated_node, 0, PAGE_SIZE);
}