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

global _gdt_flush     
extern _gp            
_gdt_flush:
    lgdt [_gp]        
    mov ax, 0x10      
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   
flush2:
    ret               

.size _start, . - _start





