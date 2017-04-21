; kernel.asm

; Some constants which are reqired
; by multiboot specification
MAGIC		equ	0x1BADB002
MBALIGN		equ	1<<0
MEMINFO		equ	1<<1
FLAGS		equ	MBALIGN | MEMINFO
CHECKSUM	equ	-(MAGIC + FLAGS)

bits 32

section .boot
	; Multyboot header
	align	4
	dd	MAGIC				; Magic number
	dd	FLAGS				; Flags
	dd	CHECKSUM			; Checksum

section .text
global	kernelStart
extern	kernelFunc				; Extern kernel C-function

kernelStart:
	cli					; Turn off interrupts
	mov	ESP, stackTop			; Set stack
	call	kernelFunc			; Call main func
haltLoop:
	hlt					; Stop CPU
	jmp	haltLoop			; Hang CPU

section .bss
	align	4
	stackBottom:				; End of stack
	resb	16384				; 16 kB stack
	stackTop:				; Stack pointer

