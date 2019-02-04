################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	18 Jun. 2018
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .boot
.balign	4
.global	kernelStart			# Kernel main function
.extern	kernelFunc			# Extern kernel C-function


# Kernel starts here
kernelStart:
	cli				# Turn off interrupts
	movl	$stackTop, %esp		# Set stack
	pushl	%eax			# Multiboot info
	pushl	%ebx			# ---//---
	call	kernelFunc		# Call main func

1:
	hlt				# Stop CPU
	jmp	1b			# Hang CPU


.section .bss
.balign 4096
stackBottom:				# End of stack
	.skip	16384			# Stack size of 16kB
stackTop:				# Stack pointer

