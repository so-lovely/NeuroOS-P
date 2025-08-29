#
# NeuroOS-P Toolchain Configuration
#
# 이 파일은 RISC-V 크로스 컴파일러의 경로와 빌드 플래그를 정의합니다.
# 사용자 환경 경로(/opt/homebrew/bin/)가 반영되었습니다.
#

# 1. 툴체인 경로 및 접두사 설정 (Toolchain Path & Prefix)
# ----------------------------------------------------------------------------
# Homebrew (macOS) 또는 일반적인 Linux 경로에 설치된 툴체인을 가정합니다.

# 툴체인 바이너리가 위치한 디렉토리
TOOLCHAIN_PATH ?= /opt/homebrew/bin/

# 크로스 컴파일러 접두사
CROSS_COMPILE  ?= riscv64-unknown-elf-


# 2. 컴파일러 및 유틸리티 정의 (Compiler & Utilities)
# ----------------------------------------------------------------------------
CC      := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)gcc
AS      := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)as
LD      := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)ld
OBJCOPY := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)objcopy
OBJDUMP := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)objdump


# 3. 컴파일러 플래그 (Compiler Flags)
# ----------------------------------------------------------------------------
# -march=rv64gc: 64-bit RISC-V with General-purpose extensions and Compressed instructions.
# -mabi=lp64d: 64-bit long/pointer, Double-precision floating point ABI.
# -nostdlib: 표준 시스템 라이브러리를 링크하지 않습니다.
# -ffreestanding: 호스트 OS에 의존하지 않는 코드를 생성합니다.
# -fno-builtin: printf와 같은 내장 함수를 사용하지 않습니다.
# -g: 디버깅 심볼을 포함합니다.
# -Wall, -Wextra: 가능한 모든 경고를 활성화합니다.

CFLAGS := -march=rv64gc -mabi=lp64d \
          -mcmodel=medany \
          -O2 -g \
          -Wall -Wextra \
          -nostdlib -ffreestanding -fno-builtin \
          -fno-omit-frame-pointer \
          -Iinclude # 최상위 include 디렉토리 추가

# CFLAGS에 src 디렉토리도 추가 (kernel/types.h 같은 참조를 위해)
CFLAGS += -Isrc


# 4. 어셈블러 플래그 (Assembler Flags)
# ----------------------------------------------------------------------------
AFLAGS := -march=rv64gc -mabi=lp64d -Iinclude


# 5. 링커 플래그 (Linker Flags)
# ----------------------------------------------------------------------------
# 링커 스크립트(-T)는 각 플랫폼별 Makefile에서 최종적으로 지정됩니다.
LDFLAGS := -nostdlib
