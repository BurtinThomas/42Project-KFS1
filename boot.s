#
# @file boot.s
# @brief Multiboot-compliant entry point that sets up the stack and jumps
#        into the C kernel.
#

# @brief Multiboot header flags: align loaded modules and request memory info.
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
# @brief Multiboot magic number identifying this binary to the bootloader.
.set MAGIC,    0x1BADB002
# @brief Checksum verifying the magic number and flags fields.
.set CHECKSUM, -(MAGIC + FLAGS)

#
# @brief Multiboot header, read by the bootloader (e.g. GRUB) before
#        control is handed to boot_entry.
#
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

#
# @brief Reserve 16 KiB of uninitialized memory to use as the kernel stack.
#
.section .bss
.align 16
.skip 16384
stack_top:

.section .text
.global boot_entry

#
# @brief Kernel entry point called by the bootloader.
#
# Sets up the stack pointer, calls into the C kernel's main(), and
# halts the CPU in an infinite loop if main() ever returns.
#
boot_entry:
    mov $stack_top, %esp
    call main

    cli

# @brief Infinite halt loop; keeps the CPU idle after the kernel returns.
halt_loop:
    hlt
    jmp halt_loop
