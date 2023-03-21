################################################################
#
#	Long mode low-level preparation operations
#
#	File:	longMode.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.set	EAXMAGIC,		0x2BADB002
.set	CPUIDBIT,		1<<21
.set	CPUIDMAGIC,		0x80000000
.set	CPUIDEXTENDED,		0x80000001
.set	LONGMODEBIT,		1<<29
.set	LONGMODEBITCR,		0xC0000080


.code32

.section .text
.balign	8

.global	checkCPUID
.global	checkLongMode


.type checkCPUID, %function
checkCPUID:

	pushfl
	pop	%eax
	movl	%eax, %ecx
	xor	$CPUIDBIT, %eax
	push	%eax
	popfl
	pushfl
	pop	%eax
	push	%ecx
	popfl
	cmp	%ecx, %eax
	je	1f
	movb	$0x01, %al
	ret

1:
	movb	$0x00, %al
	ret

.size checkCPUID, . - checkCPUID


.type checkLongMode, %function
checkLongMode:

	movl	$CPUIDMAGIC, %eax
	cpuid
	cmp	$CPUIDEXTENDED, %eax
	jl	1f
	movl	$CPUIDEXTENDED, %eax
	cpuid
	test	$LONGMODEBIT, %edx
	jz	1f
	movb	$0x01, %al
	ret

1:
	movb	$0x00, %al
	ret

.size checkLongMode, . - checkLongMode

