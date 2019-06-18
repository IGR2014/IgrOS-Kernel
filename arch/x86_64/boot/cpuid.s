################################################################
#
#	CPUID instruction functions
#
#	File:	cpuid.s
#	Date:	18 Jun 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#



.set	CPUID_STACK_SHIFT,	0x00000020	# CPUID stack 32-bit shift for 64-bit push


.code64

.section .text
.balign 8

.global cpuid					# Execute CPUID instruction with required params

# Execute CPUID with reauired flags
cpuid:
	cld					# Clear direction flag
	movl	%edi, %eax			# Put flag to EAX register
	cpuid					# Execute CPUID
	shlq	$CPUID_STACK_SHIFT, %rax
	orq	%rbx, %rax
	pushq	%rax
	shlq	$CPUID_STACK_SHIFT, %rcx
	orq	%rdx, %rcx
	pushq	%rcx
	retq					# Return structure of CPUID results

