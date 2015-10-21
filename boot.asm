;Declare Constants for multiboot header
MBALIGN     equ 1<<0                ;align loaded modules
MEMINFO     equ 1<<1                ;provides mem map
FLAGS       equ MBALIGN | MEMINFO   ;multiboot flag field
MAGIC       equ 0x1BADB002          ;magic number bootloader looks for
CHECKSUM    equ -(MAGIC + FLAGS)    ;checksome of above, proves were multiboot

;Declare Header in multiboot standard
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

;Stack pointer can be pointing to anything so...
;Creating small temp stack
section .bootstrap_stack, nobits
align 4
stack_bottom:                       ;create symbol at bottom
resb 16384                          ;allocate 16384 bytes
stack_top:                          ;create symbol at top

;Linker specifies _start as the entry point to te kernel
section .text
global _start
_start:
    ;and we are successfully in the kernel

    ;set esp register to point to the stack top
    mov esp, stack_top

    ;now we can execute c code!!!
    extern kernel_main
    call kernel_main

    ;if this every returns (which it shouldnt) we halt everything
    cli
.hang:
    hlt
    jmp .hang


