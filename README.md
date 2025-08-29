# NeuroOS-P: An RTOS for Neuromorphic Photonics

**NeuroOS-P** is a lightweight, event-driven Real-Time Operating System (RTOS) designed from the ground up for the unique challenges of Neuromorphic Photonics hardware. It aims to provide a standardized software foundation to control this next-generation hardware, allowing developers to focus on applications rather than low-level complexity.

This project is currently in the early stages of development.

---

## ✨ Core Features

The kernel is written in C and RISC-V Assembly for the `rv64gc` architecture, with the QEMU `virt` machine as the primary development target.

- **Monolithic Kernel:** A single-image kernel containing all core OS services.
- **Preemptive Scheduler:** A simple, tick-based, round-robin scheduler that ensures fair CPU time distribution among tasks.
- **Task Management:** Support for creating multiple tasks, each with its own context and kernel stack.
- **Inter-Process Communication (IPC):** A basic event signaling system (`event_wait`/`event_signal`) allows tasks to coordinate and synchronize.
- **System Calls:** A basic syscall interface for tasks to request kernel services, including `sleep()` for timed blocking.
- **Memory Management:** A physical page frame allocator manages DRAM from the end of the kernel image.
- **Interrupt Handling:** A basic trap handler processes hardware interrupts and exceptions, currently handling a periodic timer interrupt from the RISC-V CLINT.

##  STATUS

This project is a **work in progress** and should be considered a functional prototype. The core scheduling and event-signaling systems are operational within the QEMU simulation environment. It serves as a foundation for future development towards hardware targets.

## 🛠️ Getting Started

### Prerequisites

You need a RISC-V cross-compiler toolchain installed and available in your `PATH`. The project is configured for `riscv64-unknown-elf`.

- **macOS (Homebrew):**
  ```bash
  brew tap riscv/riscv
  brew install riscv-gnu-toolchain
  ```
- **Linux (Debian/Ubuntu):**
  ```bash
  sudo apt-get install build-essential gcc-riscv64-unknown-elf
  ```

### Building and Running

The project uses a standard `Makefile`.

1.  **Configure Toolchain:**
    - Copy or rename `toolchain.mk.example` to `toolchain.mk` (if it exists).
    - Edit `toolchain.mk` to ensure the `TOOLCHAIN_PATH` and `CROSS_COMPILE` prefix match your environment. The default is set for a Homebrew installation on macOS (`/opt/homebrew/bin/`).

2.  **Build the kernel:**
    ```bash
    make
    ```

3.  **Run in the QEMU simulator:**
    ```bash
    make qemu
    ```

4.  **Clean build artifacts:**
    ```bash
    make clean
    ```

## 🏗️ Architecture Overview

NeuroOS-P is built around a simple monolithic kernel. The system boots via an assembly entry point (`entry.S`) which sets up the machine-mode trap handler and initial stack before calling `kmain`. 

The C-based kernel then initializes its core subsystems:
- **Memory Manager:** Manages physical page frames.
- **Task Manager:** Manages the lifecycle of tasks.
- **Scheduler:** A round-robin algorithm switches between `READY` tasks on each system tick.
- **Event System:** Allows tasks to block waiting for signals from other tasks.

All tasks currently run in machine mode (M-mode).

## 📁 Directory Structure

```
NeuroOS-P/
├── src/                # Source code
│   ├── arch/           # Architecture-specific code (RISC-V)
│   ├── hal/            # Hardware Abstraction Layer interface
│   ├── include/        # User-facing API headers
│   └── kernel/         # Core kernel code (scheduler, tasks, IPC, etc.)
├── build/              # Build artifacts (ignored by git)
├── examples/           # Example applications
├── docs/               # Documentation
├── libs/               # Third-party libraries
├── Makefile            # Main makefile
├── toolchain.mk        # Toolchain configuration
└── linker.ld           # Linker script for memory layout
```

## 📄 License

This project is released under the **MIT License**. Please see the `LICENSE` file for details. (Note: The `LICENSE` file is currently empty and should be populated with the full MIT License text).