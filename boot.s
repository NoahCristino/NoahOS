.set FLAGS,    0
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot

.long MAGIC
.long FLAGS
.long CHECKSUM

stackBottom:

.skip 4096


stackTop:

.section .text
.global _start
.type _start, @function


_start:
	mov $stackTop, %esp
	call kernel_entry
	cli


hltLoop:

	hlt
	jmp hltLoop

.global gdt_flush

gdt_flush:
   movl 4(%esp),%eax
   lgdt (%eax)

   movw $0x10, %ax
   movw %ax, %ds
   movw %ax, %es
   movw %ax, %fs
   movw %ax, %gs
   movw %ax, %ss
   jmp  $0x08,$flush

flush:
   ret

.size _start, . - _start
