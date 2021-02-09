////////////////////////////////////////////////////////////////
//
//	Task switch registers data structure
//
//	File:	register.hpp
//	Date:	05 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/x86_64/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#pragma pack(push, 1)

	// x86_64 task switch registers save
	struct registerx86_64_t {

		// "All" registers pushed by us
		quad_t	r15;		// Register r15
		quad_t	r14;		// Register r14
		quad_t	r13;		// Register r13
		quad_t	r12;		// Register r12
		quad_t	r11;		// Register r11
		quad_t	r10;		// Register r10
		quad_t	r9;		// Register r9
		quad_t	r8;		// Register r8
		quad_t	rdi;		// Destination index register
		quad_t	rsi;		// Source index register
		quad_t	rbp;		// Stack base pointer register
		quad_t	rbx;		// Base register
		quad_t	rdx;		// Data register
		quad_t	rcx;		// Counter register
		quad_t	rax;		// Accumulator register
		// Exception number pushed by us
		quad_t	number;		// Exception number
		// Param pushed by processor or (0x00) by us
		quad_t	param;		// Exception param (or null if no param provided)
		// This pushed to stack by processor
		quad_t	rip;		// Instruction pointer register
		quad_t	cs;		// Code segment register
		quad_t	rflags;		// Flags register
		quad_t	userRsp;	// User stack pointer register
		quad_t	ss;		// Stack segment register

	};

#pragma pack(pop)


}	// namespace igros::x86_64

