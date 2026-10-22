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


.set	CPUIDBIT,		1<<21			# EFLAGS.ID: CPUID available if it can be toggled
.set	CPUIDMAGIC,		0x80000000		# Max extended CPUID leaf
.set	CPUIDEXTENDED,		0x80000001		# Extended feature flags leaf
.set	LONGMODEBIT,		1<<29			# EDX.LM: long mode supported


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
	cmpl	$CPUIDEXTENDED, %eax			# Leaf numbers are unsigned
	jb	1f
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

