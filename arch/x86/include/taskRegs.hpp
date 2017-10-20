////////////////////////////////////////////////////////////////
//
//	Task switch registers data structure
//
//	File:	taskRegs.hpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef	IGROS_ARCH_TASK_REGS_HPP
#define	IGROS_ARCH_TASK_REGS_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


#pragma pack(push, 1)

	// x86 task switch registers save
	struct taskRegs {

		// Segment registers pushed by us
		t_u32	gs;		// Extra3 segment register
		t_u32	fs;		// Extra2 segment register
		t_u32	es;		// Extra segment register
		t_u32	ds;		// Data segment register
		// "All" registers pushed by pushal instruction
		t_u32	edi;		// Destination index register
		t_u32	esi;		// Source index register
		t_u32	ebp;		// Stack base pointer register
		t_u32	esp;		// Stack pointer register
		t_u32	ebx;		// Base register
		t_u32	edx;		// Data register
		t_u32	ecx;		// Counter register
		t_u32	eax;		// Accumulator register
		// Exception number pushed by us
		t_u32	number;		// Exception number
		// Param pushed by processor or (0x00) by us
		t_u32	param;		// Exception param (or null if no param provided)
		// This pushed to stack by processor
		t_u32	eip;		// Instruction pointer register
		t_u32	cs;		// Code segment register
		t_u32	eflags;		// Flags register
		t_u32	userEsp;	// User stack pointer register
		t_u32	ss;		// Stack segment register

	};	// taskRegs

#pragma pack(pop)


}	// namespace arch


#endif	// IGROS_ARCH_TASK_REGS_HPP

