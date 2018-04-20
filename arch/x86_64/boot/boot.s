################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	20 Nov. 2017
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#


.code32

.section .text
.balign	4
.global	kernelStart			# Kernel main function
.extern	kernelFunc			# Extern kernel C-function

kernelStart:				# Kernel starts here
	cli				# Turn off interrupts
	movl	$stackTop, %esp		# Set stack
	call	kernelFunc		# Call main func

haltCPU:
	hlt				# Stop CPU
	jmp	haltCPU			# Hang CPU

.section .bss
stackBottom:				# End of stack
	.skip	16384			# Stack size of 16kB
stackTop:				# Stack pointer

