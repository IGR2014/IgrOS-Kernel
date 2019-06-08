################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	06 Jun 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	8
.global	kernelStart				# Kernel main function

.extern	checkMultiboot
.extern	checkCPUID
.extern checkLongMode

.extern	setupPageTables
.extern	enablePaging

.extern jumpToLongMode

kernelStart:					# Kernel starts here
	cli					# Turn off interrupts
	movl	$stackTop, %esp			# Set stack
	movl	%eax, multibootMagic		# Multiboot magic value
	movl	%ebx, multibootHeader		# Multiboot header address
	call	checkCPUID
	cmpb	$0x00, %al
	jz	1f
	call	checkLongMode
	cmpb	$0x00, %al
	jz	1f
	call	setupPageTables
	call	enablePaging

	call	jumpToLongMode

1:
	movl	$0x4f524f45, 0xB8000
	hlt				# Stop CPU
	jmp	1b			# Hang CPU


.section .data
.global	multibootMagic
.global	multibootHeader
multibootMagic:
	.skip	4
multibootHeader:
	.skip	8


.section .bss
stackBottom:				# End of stack
	.skip	32768			# Stack size of 16kB
stackTop:				# Stack pointer

