################################################################
#
#	CPU operations
#
#	File:	cpu.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign 4

.global cpuHalt			# halt CPU


# Halt CPU
.type cpuHalt, %function
cpuHalt:

1:
	hlt
	jmp 1b

.size cpuHalt, . - cpuHalt

