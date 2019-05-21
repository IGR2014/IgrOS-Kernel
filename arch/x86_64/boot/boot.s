################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	22 May 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	4
.global	kernelStart			# Kernel main function

.extern	checkMultiboot
.extern	checkCPUID
.extern checkLongMode

.extern	setupPageTables
.extern	enablePaging

.extern	kernelBootstrap			# Extern kernel C-function
.extern jumpToLongMode

kernelStart:				# Kernel starts here
	cli				# Turn off interrupts
	movl	$stackTop, %esp		# Set stack
#	call	kernelFunc		# Call main func

#	movl	$0x4f524f45, 0xb8014
#	call	checkMultiboot
#	cmpb	$0x00, %al
#	jz	1f
	movl	$0x4f524f45, 0xb8000
	call	checkCPUID
	cmpb	$0x00, %al
	jz	1f
	movl	$0x4f524f45, 0xb8004
	call	checkLongMode
	cmpb	$0x00, %al
	jz	1f
	movl	$0x4f524f45, 0xb8008
	call	setupPageTables
	movl	$0x4f524f45, 0xb800c
	call	enablePaging
	movl	$0x4f524f45, 0xb8010

	call	jumpToLongMode

1:
	movl	$0x4f524f45, 0xb8010
	hlt				# Stop CPU
	jmp	1b			# Hang CPU

.section .bss
stackBottom:				# End of stack
	.skip	16384			# Stack size of 16kB
stackTop:				# Stack pointer

