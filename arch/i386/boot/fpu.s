################################################################
#
#	FPU operations
#
#	File:	fpu.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.set	FPU_CONTROL_WORD_MASK,	0x103F		# FPU control word mask
.set	FPU_CONTROL_WORD,	0x003F		# FPU control word


.code32

.section .text
.balign 4

.global fpuCheck				# Check FPU


# Check FPU
.type fpuCheck, %function
fpuCheck:

	fninit					# Initialize FPU
	movl $FPU_STATUS_WORD_TEST, %eax	# Load status test value to AX
	fnstsw 2(%eax)				# Store FPU status word
	cmpw $0, %ax				# Compare FPU status word with 0
	jne 1f					# Not equal - no FPU

	movl $FPU_STATUS_WORD_TEST, %eax	# Load status test value to AX
	fnstcw 2(%eax)				# Store FPU control word
	andw $FPU_CONTROL_WORD_MASK, %ax	# Apply mask to FPU control word
	cmpw $FPU_CONTROL_WORD, %ax		# Compare FPU control word with 0
	jne 1f					# Not equal - no FPU

	movl $1, %eax				# FPU - ok
	retl					# Exit

1:
	movl $0, %eax				# FPU - error
	retl					# Exit

.size fpuCheck, . - fpuCheck


.section .rodata
.balign	4096

# FPU status word test
FPU_STATUS_WORD_TEST:
	.word	0xDEAD

