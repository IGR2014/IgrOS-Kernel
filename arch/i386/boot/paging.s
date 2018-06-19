################################################################
#
#	Memory paging low-level operations
#
#	File:	paging.s
#	Date:	19 Jun. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


.code32

.set	PAGINGBIT,	1<<31

.section .text
.balign 4
.global pagingSetupPD			# Setup Page directory
.global pagingFlushPD			# Update page tables cache
.global pagingEnable			# Enable paging


pagingSetupPD:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%esp), %eax
	movl	%eax, %cr3
	leave
	ret

pagingFlushPD:
	movl	%cr3,%eax
	movl	%eax,%cr3
	ret

pagingEnable:
	movl	%cr0, %eax
	or	$PAGINGBIT, %eax
	movl	%eax, %cr0
	ret


.section .bss
.balign 4096
.global	pageDirectory
.global	pageTable


pageDirectory:
	.skip 4096
pageTable:
	.skip 4096

