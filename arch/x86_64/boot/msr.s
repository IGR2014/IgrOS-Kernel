################################################################
#
#	MSR in/out operations
#
#	File:   msr.s
#	Date:	18 Jun 2019
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign 8

.global inMSR			# Write MSR register
.global outMSR			# Read MSR register


# Write MSR register
inMSR:
	cld				# Clear direction flag
	movq	%rdi, %rcx
        movq    %rsi, %rax
        wrmsr
	retq


# Read MSR register
outMSR:
	cld				# Clear direction flag
	movq	%rdi, %rcx
        rdmsr
	retq

