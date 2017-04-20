; kernel.asm

bits 32

section .boot
	; multyboot specific
	align 4
	dd	0x1BADB002		; magic number
	dd	0x00			; flags
	dd	-(0x1BADB002 + 0x00)	; checksum

section .text

global	start
extern	kernelFunc

start:
	cli				; turn off interrupts
	mov	ESP, stackTop		; set stack
	call	kernelFunc		; call main func
	hlt				; stop

section .bss
	resb 8192			; 8 kB stack
	stackTop:

