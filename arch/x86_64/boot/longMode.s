################################################################
#
#	Long mode low-level preparation operations
#
#	File:	longMode.s
#	Date:	20 Nov. 2017
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#


.code32

.set	EAXMAGIC,	0x2BADB002
.set	CPUIDBIT,	1<<21
.set	CPUIDMAGIC,	0x80000000
.set	CPUIDEXTENDED,	0x80000001
.set	LONGMODEBIT,	1<<29
.set	LONGMODEBITCR,	0xC0000080

.section .text
.balign	4
.global	checkMultiboot
.global	checkCPUID
.global	checkLongMode

.global	setupPageTables
.global	enablePaging

checkMultiboot:
	cmp	$EAXMAGIC, %eax
	jne	1f
	movb	$0x01, %al
	ret
1:
	movb	$0x00, %al
	ret

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

setupPageTables:
	movl	$PDPTable, %eax
	or	$0b11, %eax
	movl	%eax, (PML4Table)
	movl	$PDTable, %eax
	or	$0b11, %eax
	movl	%eax, (PDPTable)
	movl	$0, %ecx
1:
	movl	$0x200000, %eax
	mul	%ecx
	or	$0b10000011, %eax
	movl	%eax, PDTable(, %ecx, 8)
	inc	%ecx
	cmp	$512, %ecx
	jne	1b
	ret

enablePaging:
	movl	$PML4Table, %eax
	movl	%eax, %cr3
	movl	%cr4, %eax
	or	$1<<5, %eax
	movl	%eax, %cr4
	movl	$LONGMODEBITCR, %ecx
	rdmsr
	or	$1<<8, %eax
	wrmsr
	movl	%cr0, %eax
	or	$1<<31, %eax
	movl	%eax, %cr0
	ret

.section .bss
	.balign	4096
PML4Table:
	.skip	4096
PDPTable:
	.skip	4096
PDTable:
	.skip	4096
PTable:
	.skip	4096

