################################################################
#
#	MSR in/out operations
#
#	File:   msr.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign 8

.global inMSR				# Write MSR register function
.global outMSR				# Read MSR register function


# Write MSR register
.type inMSR, %function
inMSR:

	cld				# Clear direction flag
	movq	%rdi, %rcx
        movq    %rsi, %rax
        wrmsr
	retq

.size inMSR, . - inMSR


# Read MSR register
.type outMSR, %function
outMSR:

	cld				# Clear direction flag
	movq	%rdi, %rcx
        rdmsr
	retq

.size outMSR, . - outMSR

