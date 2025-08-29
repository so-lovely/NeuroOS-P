#
# NeuroOS-P Main Makefile
#

# 1. 기본 설정 (Default Goal & Platform)
# ----------------------------------------------------------------------------
.PHONY: all clean qemu

# `make` 실행 시 기본적으로 `all` 타겟을 빌드합니다.
all: kernel

# 기본 타겟 플랫폼을 `qemu-virt`로 설정합니다.
# 다른 플랫폼으로 빌드하려면 `make PLATFORM=fpga_photonics_soc`와 같이 실행합니다.
PLATFORM ?= qemu-virt


# 2. 툴체인 및 빌드 플래그 포함 (Include Toolchain)
# ----------------------------------------------------------------------------
# `toolchain.mk` 파일이 없으면 에러 메시지를 출력하고 중지합니다.
ifeq ($(wildcard toolchain.mk),)
$(error "toolchain.mk not found. Please create it based on your environment.")
endif
include toolchain.mk


# 3. 빌드 디렉토리 설정 (Build Directory)
# ----------------------------------------------------------------------------
BUILD_DIR := build/$(PLATFORM)


# 4. 소스 파일 정의 (Source Files)
# ----------------------------------------------------------------------------
# `wildcard` 대신 빌드 순서를 보장하기 위해 모든 소스 파일을 명시적으로 나열합니다.

# 아키텍처 공통 어셈블리 파일
S_SOURCES = src/arch/riscv/common/entry.S \
            src/arch/riscv/common/context.S

# 커널 및 아키텍처 C 소스 파일
SRC_C_SOURCES = src/arch/riscv/platform/qemu-virt/uart.c \
                src/arch/riscv/platform/qemu-virt/clint.c \
                src/arch/riscv/platform/qemu-virt/platform.c \
                src/arch/riscv/platform/qemu-virt/hal_impl.c \
                src/arch/riscv/common/trap.c \
                src/kernel/memory.c \
                src/kernel/interrupt.c \
                src/kernel/time.c \
                src/kernel/event.c \
                src/kernel/task.c \
                src/kernel/syscall.c \
                src/kernel/user.c \
                src/kernel/scheduler.c \
                src/kernel/printk.c \
                src/kernel/kernel.c

# 최소 라이브러리 C 소스 파일
LIBS_C_SOURCES = libs/libc-minimal/string.c \
                 libs/libc-minimal/stdio.c


# 5. 오브젝트 파일 정의 (Object Files)
# ----------------------------------------------------------------------------
# 각 소스 파일에 대응하는 오브젝트 파일 경로를 `build` 디렉토리 하위로 지정합니다.
SRC_OBJS := $(patsubst src/%.c, $(BUILD_DIR)/src/%.o, $(SRC_C_SOURCES))
SRC_OBJS += $(patsubst src/%.S, $(BUILD_DIR)/src/%.o, $(S_SOURCES))
LIBS_OBJS := $(patsubst libs/%.c, $(BUILD_DIR)/libs/%.o, $(LIBS_C_SOURCES))

OBJS := $(SRC_OBJS) $(LIBS_OBJS)


# 6. 최종 타겟 정의 (Final Target)
# ----------------------------------------------------------------------------
TARGET_NAME := neuroos-p-$(PLATFORM)
TARGET_ELF  := $(BUILD_DIR)/$(TARGET_NAME).elf
TARGET_BIN  := $(BUILD_DIR)/$(TARGET_NAME).bin


# 7. 링커 스크립트 설정 (Linker Script)
# ----------------------------------------------------------------------------
# 버그 수정을 위해 링커 스크립트 경로를 변수 없이 직접 지정합니다.
LDFLAGS  += -T src/arch/riscv/platform/qemu-virt/linker.ld


# 8. 빌드 규칙 (Build Rules)
# ----------------------------------------------------------------------------

# 최종 커널 ELF 파일을 빌드합니다.
kernel: $(TARGET_ELF)

$(TARGET_ELF): $(OBJS)
	@echo "  LD      $@"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

# src/ 디렉토리의 C 소스 파일을 컴파일하는 규칙
$(BUILD_DIR)/src/%.o: src/%.c
	@echo "  CC      $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# libs/ 디렉토리의 C 소스 파일을 컴파일하는 규칙
$(BUILD_DIR)/libs/%.o: libs/%.c
	@echo "  CC      $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# src/ 디렉토리의 Assembly 소스 파일을 컴파일하는 규칙
$(BUILD_DIR)/src/%.o: src/%.S
	@echo "  AS      $@"
	@mkdir -p $(dir $@)
	@$(CC) $(AFLAGS) -c $< -o $@


# 9. 부가 규칙 (Phony Targets)
# ----------------------------------------------------------------------------

# 빌드 결과물 삭제
clean:
	@echo "  CLEAN   $(BUILD_DIR)"
	@rm -rf $(BUILD_DIR)

# QEMU 실행
# `make qemu`를 실행하면 커널을 빌드하고 바로 QEMU에서 실행합니다.
qemu: $(TARGET_ELF)
	@echo "  QEMU    Running $(TARGET_ELF)"
	@qemu-system-riscv64 -machine virt -nographic \
	-bios none -kernel $(TARGET_ELF)

# QEMU를 GDB 서버 모드로 실행
# `make qemu-gdb`를 실행하면 QEMU가 1234번 포트에서 GDB 연결을 기다립니다.
qemu-gdb: $(TARGET_ELF)
	@echo "  QEMU    Running in GDB server mode on port 1234..."
	@qemu-system-riscv64 -machine virt -nographic \
	-bios none -kernel $(TARGET_ELF) -S -s
