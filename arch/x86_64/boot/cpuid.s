################################################################
#
#	CPUID instruction functions
#
#	File:	cpuid.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#



.set	CPUID_STACK_SHIFT,	0x00000020	# CPUID stack 32-bit shift for 64-bit push


.code64

.section .text
.balign 8

.global cpuCPUID				# Execute CPUID instruction with required params


# Execute CPUID with reauired flags
.type cpuCPUID, %function
cpuCPUID:

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

.size cpuCPUID, . - cpuCPUID

