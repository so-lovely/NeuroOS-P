/*
 * NeuroOS-P Linker Script for QEMU 'virt' RISC-V machine
 *
 * 이 스크립트는 커널 이미지의 메모리 레이아웃을 정의합니다.
 */

/* 1. 출력 형식 및 아키텍처 설정 */
OUTPUT_ARCH(riscv)

/* 2. 커널의 최초 진입점(Entry Point) 심볼 지정 */
/* 이 심볼은 src/arch/riscv/common/entry.S 파일에 정의될 예정입니다. */
ENTRY(_start)

/* 3. 메모리 맵 정의 */
/* QEMU의 'virt' 머신은 기본적으로 0x80000000에서 시작하는 128MB DRAM을 가집니다. */
MEMORY
{
  DRAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

/* 4. 섹션 배치 정의 */
SECTIONS
{
    /* 현재 위치 카운터(.)를 DRAM의 시작 주소로 설정 */
    . = ORIGIN(DRAM);

    /* .text 섹션: 실행 가능한 모든 코드가 여기에 위치합니다. */
    .text : {
        /* entry.S의 코드가 가장 먼저 오도록 명시적으로 배치합니다. */
        KEEP(*(.text.entry))
        /* 나머지 모든 코드들을 배치합니다. */
        *(.text*)
    } > DRAM

    /* .rodata 섹션: 읽기 전용 데이터 (상수 등) */
    .rodata : {
        . = ALIGN(8); /* 8바이트 정렬 */
        *(.rodata*)
    } > DRAM

    /* .data 섹션: 초기화된 전역/정적 변수 */
    .data : {
        . = ALIGN(8);
        *(.data*)
    } > DRAM

    /* .bss 섹션: 초기화되지 않은 전역/정적 변수 */
    /* 커널 시작 시 이 영역을 0으로 채워야 합니다. */
    .bss : {
        . = ALIGN(8);
        __bss_start = .; /* BSS 섹션 시작 주소 심볼 */
        *(.bss*)
        *(.sbss*) /* 스몰 BSS 섹션 */
        __bss_end = .;   /* BSS 섹션 끝 주소 심볼 */
    } > DRAM

    /* 커널 이미지의 끝을 나타내는 심볼 */
    /* 이 주소 이후부터 동적 메모리 할당 영역(heap)으로 사용할 수 있습니다. */
    _end = .;

    /* 초기 스택 포인터 위치를 DRAM의 최상단으로 정의 */
    /* 스택은 높은 주소에서 낮은 주소로 자랍니다. */
    __stack_top = ORIGIN(DRAM) + LENGTH(DRAM);
}
