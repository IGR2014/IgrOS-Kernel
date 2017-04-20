bits 32
section .text
	; multyboot specific
	;align 4
	;dd	0x1BADB002		; magic number
	;dd	0x00			; flags
	;dd	-(0x1BADB002 + 0x00)	; checksum
header_start:
	dd 0xe85250d6                ; magic number (multiboot 2)
	dd 0                         ; architecture 0 (protected mode i386)
	dd header_end - header_start ; header length
	; checksum
	dd -(0xe85250d6 + 0 + (header_end - header_start))

	; insert optional multiboot tags here

	; required end tag
	dw 0    ; type
	dw 0    ; flags
	dd 8    ; size
header_end:


global start
extern kernelFunc

start:
	cli				; turn off interrupts
	mov	ESP, stackTop		; set stack
	call	kernelFunc		; call main func
	hlt				; stop

section .bss
	resb 8192			; 8 kB stack
	stackTop:

