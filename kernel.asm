; kernel.asm

; lets assmble x86 code
bits 32

; Some constants which are reqired
; by multiboot specification
MAGIC		equ	0x1BADB002
MBALIGN		equ	1<<0
MEMINFO		equ	1<<1
FLAGS		equ	MBALIGN | MEMINFO
CHECKSUM	equ	-(MAGIC + FLAGS)

section .boot
	; Multyboot header
	align	4
	dd	MAGIC				; Magic number
	dd	FLAGS				; Flags
	dd	CHECKSUM			; Checksum

section .text
align	4
global	kernelStart				; Kernel main function
global	portWrite				; Read data from port
global	portRead				; Write data to port
extern	kernelFunc				; Extern kernel C-function

kernelStart:
	cli					; Turn off interrupts
	mov	ESP, stackTop			; Set stack
	call	kernelFunc			; Call main func
haltLoop:
	hlt					; Stop CPU
	jmp	haltLoop			; Hang CPU

portWrite:
	mov	EDX, [ESP + 4]
	mov	AL, [ESP + 8]
	out	DX, AL
	ret

portRead:
	mov	EDX, [ESP + 4]
	in	AL, DX
	ret

section .bss
	align	4
	stackBottom:				; End of stack
	resb	16384				; 16 kB stack
	stackTop:				; Stack pointer
