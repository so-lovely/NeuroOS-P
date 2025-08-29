#
# NeuroOS-P Toolchain Configuration
#
# This file defines the path to the RISC-V cross-compiler and build flags.
# It has been adapted for a user's environment (/opt/homebrew/bin/).
#

# 1. Toolchain Path & Prefix
# ----------------------------------------------------------------------------
# Assumes a toolchain installed via Homebrew (macOS) or a standard Linux path.

# Directory where the toolchain binaries are located.
TOOLCHAIN_PATH ?= /opt/homebrew/bin/

# Cross-compiler prefix.
CROSS_COMPILE  ?= riscv64-unknown-elf-


# 2. Compiler & Utilities
# ----------------------------------------------------------------------------
CC      := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)gcc
AS      := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)as
LD      := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)ld
OBJCOPY := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)objcopy
OBJDUMP := $(TOOLCHAIN_PATH)$(CROSS_COMPILE)objdump


# 3. Compiler Flags (CFLAGS)
# ----------------------------------------------------------------------------
# -march=rv64gc: 64-bit RISC-V with General-purpose extensions and Compressed instructions.
# -mabi=lp64d: 64-bit long/pointer, Double-precision floating point ABI.
# -nostdlib: Do not link the standard system libraries.
# -ffreestanding: Generate code that does not depend on a host OS.
# -fno-builtin: Do not use built-in functions like printf.
# -g: Include debugging symbols.
# -Wall, -Wextra: Enable all reasonable warnings.

CFLAGS := -march=rv64gc -mabi=lp64d \
          -mcmodel=medany \
          -O2 -g \
          -Wall -Wextra \
          -nostdlib -ffreestanding -fno-builtin \
          -fno-omit-frame-pointer

# Include paths for the compiler.
CFLAGS += -Isrc
CFLAGS += -Isrc/include
CFLAGS += -Ilibs/libc-minimal/include


# 4. Assembler Flags (AFLAGS)
# ----------------------------------------------------------------------------
AFLAGS := -march=rv64gc -mabi=lp64d -Isrc/include


# 5. Linker Flags (LDFLAGS)
# ----------------------------------------------------------------------------
# The linker script (-T) is specified in the main Makefile for each platform.
LDFLAGS := -nostdlib
