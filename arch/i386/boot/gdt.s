################################################################
#
#	Global descriptor table low-level operations
#
#	File:	gdt.s
#	Date:	17 Apr. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


.code32

.set	LONGMODEBITCR,	0xC0000080

.section .text
.balign 4
.global	gdtLoad				# Load GDT
.global setupPageTables			# PageTablesSetup
.global enablePaging			# Enable paging

gdtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lgdt	(%eax)			# Load GDT from pointer
	ljmp	$0x08, $1f
1:
	movw	$0x10, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
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
PML4Table:
	.skip	4096
PDPTable:
	.skip	4096
PDTable:
	.skip	4096
PTable:
	.skip	4096

