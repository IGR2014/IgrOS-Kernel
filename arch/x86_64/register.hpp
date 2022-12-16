////////////////////////////////////////////////////////////////
//
//	Task switch registers data structure
//
//	File:	register.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#pragma pack(push, 1)

	// x86_64 task switch registers save
	struct register_t {

		// "All" registers pushed by us
		igros_quad_t	r15;		// Register r15
		igros_quad_t	r14;		// Register r14
		igros_quad_t	r13;		// Register r13
		igros_quad_t	r12;		// Register r12
		igros_quad_t	r11;		// Register r11
		igros_quad_t	r10;		// Register r10
		igros_quad_t	r9;		// Register r9
		igros_quad_t	r8;		// Register r8
		igros_quad_t	rdi;		// Destination index register
		igros_quad_t	rsi;		// Source index register
		igros_quad_t	rbp;		// Stack base pointer register
		igros_quad_t	rbx;		// Base register
		igros_quad_t	rdx;		// Data register
		igros_quad_t	rcx;		// Counter register
		igros_quad_t	rax;		// Accumulator register
		// Exception number pushed by us
		igros_quad_t	number;		// Exception number
		// Param pushed by processor or (0x00) by us
		igros_quad_t	param;		// Exception param (or null if no param provided)
		// This pushed to stack by processor
		igros_quad_t	rip;		// Instruction pointer register
		igros_quad_t	cs;		// Code segment register
		igros_quad_t	rflags;		// Flags register
		igros_quad_t	userRsp;	// User stack pointer register
		igros_quad_t	ss;		// Stack segment register

	};

#pragma pack(pop)


}	// namespace igros::x86_64

