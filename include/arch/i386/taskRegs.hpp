////////////////////////////////////////////////////////////////
//
//	Task switch registers data structure
//
//	File:	taskRegs.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef	IGROS_ARCH_TASK_REGS_HPP
#define	IGROS_ARCH_TASK_REGS_HPP


#include <types.hpp>


// Arch-dependent code zone
namespace arch {


#pragma pack(push, 1)

	// x86 task switch registers save
	struct taskRegs_t {

		// Segment registers pushed by us
		dword_t	gs;		// Extra3 segment register
		dword_t	fs;		// Extra2 segment register
		dword_t	es;		// Extra segment register
		dword_t	ds;		// Data segment register
		// "All" registers pushed by pushal instruction
		dword_t	edi;		// Destination index register
		dword_t	esi;		// Source index register
		dword_t	ebp;		// Stack base pointer register
		dword_t	esp;		// Stack pointer register
		dword_t	ebx;		// Base register
		dword_t	edx;		// Data register
		dword_t	ecx;		// Counter register
		dword_t	eax;		// Accumulator register
		// Exception number pushed by us
		dword_t	number;		// Exception number
		// Param pushed by processor or (0x00) by us
		dword_t	param;		// Exception param (or null if no param provided)
		// This pushed to stack by processor
		dword_t	eip;		// Instruction pointer register
		dword_t	cs;		// Code segment register
		dword_t	eflags;		// Flags register
		dword_t	userEsp;	// User stack pointer register
		dword_t	ss;		// Stack segment register

	};	// taskRegs

#pragma pack(pop)


}	// namespace arch


#endif	// IGROS_ARCH_TASK_REGS_HPP

