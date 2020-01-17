################################################################
#
#	Kernel-space memset function implementation for x86
#
#	File:	memset.s
#	Date:	06 Jun 2019
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


.code64

.set	ALIGNWORD,	0x0000000000000001
.set	ALIGNDWORD,	0x0000000000000003
.set	ALIGNQUAD,	0x0000000000000007

.set	SIZEWORD,	ALIGNWORD	+ 1
.set	SIZEDWORD,	ALIGNDWORD	+ 1
.set	SIZEQUAD,	ALIGNQUAD	+ 1

.set	LIMITSTOSB,	0xFFFFFFFFFFFFFF80
.set	LIMITSTOSW,	0xFFFFFFFFFFFFFFC0
.set	LIMITSTOSL,	0xFFFFFFFFFFFFFFE0
.set	LIMITSTOSQ,	0xFFFFFFFFFFFFFFF0


.section .text
.balign 8
.global	kmemset8			# Memset function for bytes
.global	kmemset16			# Memset function for words
.global	kmemset32			# Memset function for double words
.global	kmemset64			# Memset function for quad words


#########################################
#
#	Memset for bytes function
#
kmemset8:
	#movq	%rdi, %rdi		# Get pointer to the target memory
	movl	%esi, %ecx		# Get size of target memory (in bytes)
	movb	%dl, %al		# Get value to be set in target memory

	test	%rdi, %rdi		# Null pointer test
	jz	2f			# Exit if so
	test	%ecx, %ecx		# Null size test
	jz	2f			# Exit if so

	test	$LIMITSTOSB, %ecx	# Check if filled size is more than 128 bytes
	jz	1f			# If not so - jump to MOVB method

# STOSB for size > 127
	cld				# Clear direction flag for ECX to count down
	rep	stosb			# Store bytes "string"
	jmp	2f			# Jump to exit

# MOVSB for size = [1 .. 127] bytes
1:
	movb	%al, (%rdi)		# Move byte from AL to memory address from RDI
	incq	%rdi			# Move to next memory address
	dec	%ecx			# Decrement bytes counter
	jnz	1b			# Repeat if byte counter is not zero

# Exit from function
2:
	movq	%rdi, %rax		# Return pointer to string
	retq


#########################################
#
#	Memset for words function
#
kmemset16:
	#movq	%rdi, %rdi		# Get pointer to the target memory
	movl	%esi, %ecx		# Get size of target memory (in words)
	movw	%dx, %ax		# Get value to be set in target memory

	test	%rdi, %rdi		# Null pointer test
	jz	3f			# Exit if so
	test	%ecx, %ecx		# Null size test
	jz	3f			# Exit if so

	test	$ALIGNWORD, %rdi	# Check if target address is WORD-aligned
	jz	1f			# Go to STOSW if so

# Deal with unaligned target address
	dec	%ecx			# Decrement words counter
	movb	%al, (%rdi, %rcx, 2)	# Copy last byte to the target
	movb	%ah, (%rdi)		# Move byte from AL to memory address from RDI
	addq	$SIZEWORD, %rdi		# Move to next target address

# Case for WORD-aligned target address
1:
	test	$LIMITSTOSW, %ecx	# Check if filled size is more than 128 bytes 
	jz	2f			# Jump to MOVW method if not

# STOSW method for size > 127
	cld				# Clear direction flag for ECX to count down
	rep	stosw			# Store words "string"
	jmp	3f			# Jump to last byte check

# MOVW method for size = [1 .. 127]
2:
	movw	%ax, (%rdi)		# Move word from AX to memory address from RDI
	addq	$SIZEWORD, %rdi		# Move to next memory address
	dec	%ecx			# Decrement words counter
	jnz	2b			# Repeat if word counter is not zero

# Exit from function
3:
	movq	%rdi, %rax		# Return pointer to string
	retq


#########################################
#
#	Memset for double words function
#
kmemset32:
	#movq	%rdi, %rdi		# Get pointer to the target memory
	movl	%esi, %ecx		# Get size of target memory (in double words)
	movl	%edx, %eax		# Get value to be set in target memory

	test	%rdi, %rdi		# Null pointer test
	jz	5f			# Exit if so
	test	%ecx, %ecx		# Null size test
	jz	5f			# Exit if so

	test	$ALIGNDWORD, %rdi	# Check if target address is DWORD-aligned
	jz	3f			# Go to STOSL if so
	dec	%ecx
	test	$ALIGNWORD, %rdi	# Check if target address is WORD-aligned
	jz	2f			# Go to MOVW if so
	test	$SIZEWORD, %rdi		# Check if target address is BYTE-aligned
	jz	1f			# Go to MOVB if so

# Deal with first unaligned byte and word
	movb	%al, (%rdi, %rcx, 4)	# Copy last byte to the target
	shrq	$0x08, %rax
	movb	%al, (%rdi)		# Move byte from high word of RAX to memory address from RDI
	shrq	$0x08, %rax
	movw	%ax, (%rdi)
	movq	%rdx, %rax
	jmp	3f

# Deal with first unaligned byte
1:
	shrq	$0x08, %rax
	movw	%ax, (%rdi, %rcx, 4)
	shrq	$0x10, %rax
	movb	%al, (%rdi, %rcx, 4)	# Copy last byte to the target
	movq	%rdx, %rax
	movb	%al, (%rdi)		# Move byte from high word of RAX to memory address from RDI
	jmp	3f

# Deal with first unaligned word
2:
	movw	%ax, (%rdi, %rcx, 4)	# Copy last word to the target
	shrq	$0x08, %rax
	movw	%ax, (%rdi)		# Move word from high word of RAX to memory address from RDI
	movq	%rdx, %rax

# Case for DWORD-aligned target address
3:
	addq	$SIZEDWORD, %rdi	# Move to next target address
	test	$LIMITSTOSL, %ecx	# Check if filled size is more than 128 bytes 
	jz	4f			# Jump to MOVL method if not

# STOSL method for size > 127
	cld				# Clear direction flag for ECX to count down
	rep	stosl			# Store double words "string"
	jmp	5f			# Jump to last word, byte or both check

# MOVL method for size = [1 .. 127]
4:
	movl	%eax, (%rdi)		# Move double word from EAX to memory address from RDI
	addq	$SIZEDWORD, %rdi	# Move to next memory address
	dec	%ecx			# Decrement double words counter
	jnz	4b			# Repeat if double word counter is not zero

# Exit from function
5:
	movq	%rdi, %rax		# Return pointer to string
	retq


