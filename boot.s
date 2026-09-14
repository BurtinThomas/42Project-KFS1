.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
.skip 16384
stack_top:

.section .text
.global boot_entry

boot_entry:
    mov $stack_top, %esp
    call kernel_entry

    cli

halt_loop:
    hlt
    jmp halt_loop
