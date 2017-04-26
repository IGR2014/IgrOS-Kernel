# kernel.s

.code32

.set	EAXMAGIC,	0x2BADB002
.set	CPUIDBIT,	1<<21
.set	CPUIDMAGIC,	0x80000000
.set	CPUIDEXTENDED,	0x80000001
.set	LONGMODEBIT,	1<<29
.set	LONGMODEBITCR,	0xC0000080

.section .text
.balign	4
.global	kernelStart			# Kernel main function
.extern	kernelFunc			# Extern kernel C-function

kernelStart:				# Kernel starts here
	cli				# Turn off interrupts
	movl	$stackTop, %esp		# Set stack

	call	checkMultiboot
	call	checkCPUID
	call	checkLongMode

	call	pageTableSetup
	call	enablePaging

	lgdt	(GDTPointer)

	call	kernelFunc		# Call main func

.haltLoop:
	hlt				# Stop CPU
	jmp	.haltLoop		# Hang CPU

checkMultiboot:
	cmp	$EAXMAGIC, %eax
	jne	.haltLoop
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
	je	.haltLoop
	ret

checkLongMode:
	movl	$CPUIDMAGIC, %eax
	cpuid
	cmp	$CPUIDEXTENDED, %eax
	jl	.haltLoop
	movl	$CPUIDEXTENDED, %eax
	cpuid
	test	$LONGMODEBIT, %edx
	jz	.haltLoop
	ret

pageTableSetup:
	movl	$PDPTable, %eax
	or	$0b11, %eax
	movl	%eax, (PML4Table)
	movl	$PDTable, %eax
	or	$0b11, %eax
	movl	%eax, (PDPTable)
	movl	$0, %ecx
.mapPDTable:
	movl	$0x200000, %eax
	mul	%ecx
	or	$0b10000011, %eax
	movl	%eax, PDTable(, %ecx, 8)
	inc	%ecx
	cmp	$512, %ecx
	jne	.mapPDTable
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

.section .rodata
GDT:
	.word	0
	.word	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
code:
	.word	0
	.word	0
	.byte	0
	.byte	0b10011010
	.byte	0b00100000
	.byte	0
GDTEnd:

GDTPointer:
	.word	GDTPointer - GDT - 1
	.long	GDT

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
stackBottom:				# End of stack
	.skip	16384			# Stack size of 16kB
stackTop:				# Stack pointer

