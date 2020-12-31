////////////////////////////////////////////////////////////////
//
//	Task switch registers data structure
//
//	File:	register.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/i386/types.hpp>


// i386 namespace
namespace igros::i386 {


#pragma pack(push, 1)

	// x86 task switch registers save
	struct register_t {

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

	};

#pragma pack(pop)


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read CS register
		[[nodiscard]] word_t volatile outCS() noexcept;
		// Read DS register
		[[nodiscard]] word_t volatile outDS() noexcept;
		// Read ES register
		[[nodiscard]] word_t volatile outES() noexcept;
		// Read FS register
		[[nodiscard]] word_t volatile outFS() noexcept;
		// Read GS register
		[[nodiscard]] word_t volatile outGS() noexcept;
		// Read SS register
		[[nodiscard]] word_t volatile outSS() noexcept;

		// Read EBP register
		[[nodiscard]] dword_t volatile outEBP() noexcept;
		// Read ESP register
		[[nodiscard]] dword_t volatile outESP() noexcept;

		// Read EFLAGS register
		[[nodiscard]] dword_t volatile outEFLAGS() noexcept;

		// Read EAX register
		[[nodiscard]] dword_t volatile outEAX() noexcept;
		// Read EBX register
		[[nodiscard]] dword_t volatile outEBX() noexcept;
		// Read ECX register
		[[nodiscard]] dword_t volatile outECX() noexcept;
		// Read EDX register
		[[nodiscard]] dword_t volatile outEDX() noexcept;
		// Read EDI register
		[[nodiscard]] dword_t volatile outEDI() noexcept;
		// Read ESI register
		[[nodiscard]] dword_t volatile outESI() noexcept;

		// Read EIP register
		[[nodiscard]] dword_t volatile outEAX() noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Print registers info
	void dumpRegisters() noexcept;


}	// namespace igros::i386

