################################################################
#
#	Kernel-space memset function implementation for x86
#
#	File:	memset.s
#	Date:	08 Jun. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


.code32

.set	ALIGNWORD,	0x00000001
.set	ALIGNDWORD,	0x00000003

.set	SIZEWORD,	ALIGNWORD + 1
.set	SIZEDWORD,	ALIGNDWORD + 1

.set	LIMITSTOSB,	0xFFFFFF80
.set	LIMITSTOSW,	0xFFFFFFC0
.set	LIMITSTOSL,	0xFFFFFFE0


.section .text
.balign 4
.global	kmemset8			# Memset function for bytes
.global	kmemset16			# Memset function for words
.global	kmemset32			# Memset function for double words


#########################################
#
#	Memset for bytes function
#
kmemset8:
	pushl	%ebp			# Function prologue
	movl	%esp, %ebp		# ---//---
	pushl	%edi			# Save EDI register

	movl	12(%esp), %edi		# Get pointer to the target memory
	movl	16(%esp), %ecx		# Get size of target memory (in bytes)
	movb	20(%esp), %al		# Get value to be set in target memory

	test	%edi, %edi		# Null pointer test
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
	movb	%al, (%edi)		# Move byte from AL to memory address from EDI
	inc	%edi			# Move to next memory address
	dec	%ecx			# Decrement bytes counter
	jnz	1b			# Repeat if byte counter is not zero

# Exit from function
2:
	movl	8(%esp), %eax		# Return pointer to string
	popl	%edi			# Restore EDI register
	leave				# Function epilogue
	retl


#########################################
#
#	Memset for words function
#
kmemset16:
	pushl	%ebp			# Function prologue
	movl	%esp, %ebp		# ---//---
	pushl	%edi			# Save EDI register

	movl	12(%esp), %edi		# Get pointer to the target memory
	movl	16(%esp), %ecx		# Get size of target memory (in words)
	movw	20(%esp), %ax		# Get value to be set in target memory

	test	%edi, %edi		# Null pointer test
	jz	3f			# Exit if so
	test	%ecx, %ecx		# Null size test
	jz	3f			# Exit if so

	test	$ALIGNWORD, %edi	# Check if target address is WORD-aligned
	jz	1f			# Go to STOSW if so

# Deal with unaligned target address
	dec	%ecx			# Decrement words counter
	movb	%al, (%edi, %ecx, 2)	# Copy last byte to the target
	movb	%ah, (%edi)		# Move byte from AL to memory address from EDI
	addl	$SIZEWORD, %edi		# Move to next target address

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
	movw	%ax, (%edi)		# Move word from AX to memory address from EDI
	addl	$SIZEWORD, %edi		# Move to next memory address
	dec	%ecx			# Decrement words counter
	jnz	2b			# Repeat if word counter is not zero

# Exit from function
3:
	movl	8(%esp), %eax		# Return pointer to string
	popl	%edi			# Restore EDI register
	leave				# Function epilogue
	retl


#########################################
#
#	Memset for double words function
#
kmemset32:
	pushl	%ebp			# Function prologue
	movl	%esp, %ebp		# ---//---
	pushl	%edi			# Save EDI register

	movl	12(%esp), %edi		# Get pointer to the target memory
	movl	16(%esp), %ecx		# Get size of target memory (in double words)
	movl	20(%esp), %eax		# Get value to be set in target memory

	test	%edi, %edi		# Null pointer test
	jz	5f			# Exit if so
	test	%ecx, %ecx		# Null size test
	jz	5f			# Exit if so

	test	$ALIGNDWORD, %edi	# Check if target address is DWORD-aligned
	jz	3f			# Go to STOSL if so
	dec	%ecx
	test	$ALIGNWORD, %edi	# Check if target address is WORD-aligned
	jz	2f			# Go to MOVW if so
	test	$SIZEWORD, %edi		# Check if target address is BYTE-aligned
	jz	1f			# Go to MOVB if so

# Deal with first unaligned byte and word
	movb	%al, (%edi, %ecx, 4)	# Copy last byte to the target
	shrl	$8, %eax
	movb	%al, (%edi)		# Move byte from high word of EAX to memory address from EDI
	shrl	$8, %eax
	movw	%ax, (%edi)
	movl	20(%esp), %eax
	jmp	3f

# Deal with first unaligned byte
1:
	shrl	$8, %eax
	movw	%ax, (%edi, %ecx, 4)
	shrl	$16, %eax
	movb	%al, (%edi, %ecx, 4)	# Copy last byte to the target
	movl	20(%esp), %eax
	movb	%al, (%edi)		# Move byte from high word of EAX to memory address from EDI
	jmp	3f

# Deal with first unaligned word
2:
	movw	%ax, (%edi, %ecx, 4)	# Copy last word to the target
	shrl	$8, %eax
	movw	%ax, (%edi)		# Move word from high word of EAX to memory address from EDI
	movl	20(%esp), %eax

# Case for DWORD-aligned target address
3:
	addl	$SIZEDWORD, %edi	# Move to next target address
	test	$LIMITSTOSL, %ecx	# Check if filled size is more than 128 bytes 
	jz	4f			# Jump to MOVL method if not

# STOSL method for size > 127
	cld				# Clear direction flag for ECX to count down
	rep	stosl			# Store double words "string"
	jmp	5f			# Jump to last word, byte or both check

# MOVL method for size = [1 .. 127]
4:
	movl	%eax, (%edi)		# Move double word from EAX to memory address from EDI
	addl	$SIZEDWORD, %edi	# Move to next memory address
	dec	%ecx			# Decrement double words counter
	jnz	4b			# Repeat if double word counter is not zero

# Exit from function
5:
	movl	8(%esp), %eax		# Return pointer to string
	popl	%edi			# Restore EDI register
	leave				# Function epilogue
	retl


