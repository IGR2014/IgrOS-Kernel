////////////////////////////////////////////////////////////////
//
//	Task switch registers data structure
//
//	File:	register.hpp
//	Date:	09 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Read CS register
	[[nodiscard]]
	auto	outCS() noexcept -> igros::igros_word_t;
	// Read DS register
	[[nodiscard]]
	auto	outDS() noexcept -> igros::igros_word_t;
	// Read ES register
	[[nodiscard]]
	auto	outES() noexcept -> igros::igros_word_t;
	// Read FS register
	[[nodiscard]]
	auto	outFS() noexcept -> igros::igros_word_t;
	// Read GS register
	[[nodiscard]]
	auto	outGS() noexcept -> igros::igros_word_t;
	// Read SS register
	[[nodiscard]]
	auto	outSS() noexcept -> igros::igros_word_t;

	// Read EBP register
	[[nodiscard]]
	auto	outEBP() noexcept -> igros::igros_dword_t;
	// Read ESP register
	[[nodiscard]]
	auto	outESP() noexcept -> igros::igros_dword_t;

	// Read EFLAGS register
	[[nodiscard]]
	auto	outEFLAGS() noexcept -> igros::igros_dword_t;

	// Read EAX register
	[[nodiscard]]
	auto	outEAX() noexcept -> igros::igros_dword_t;
	// Read EBX register
	[[nodiscard]]
	auto	outEBX() noexcept -> igros::igros_dword_t;
	// Read ECX register
	[[nodiscard]]
	auto	outECX() noexcept -> igros::igros_dword_t;
	// Read EDX register
	[[nodiscard]]
	auto	outEDX() noexcept -> igros::igros_dword_t;
	// Read EDI register
	[[nodiscard]]
	auto	outEDI() noexcept -> igros::igros_dword_t;
	// Read ESI register
	[[nodiscard]]
	auto	outESI() noexcept -> igros::igros_dword_t;

	// Read EIP register
	[[nodiscard]]
	auto	outEAX() noexcept -> igros::igros_dword_t;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 namespace
namespace igros::i386 {


#pragma pack(push, 1)

	// x86 task switch registers save
	struct register_t {

		// Segment registers pushed by us
		igros_dword_t	gs;		// Extra3 segment register
		igros_dword_t	fs;		// Extra2 segment register
		igros_dword_t	es;		// Extra segment register
		igros_dword_t	ds;		// Data segment register
		// "All" registers pushed by pushal instruction
		igros_dword_t	edi;		// Destination index register
		igros_dword_t	esi;		// Source index register
		igros_dword_t	ebp;		// Stack base pointer register
		igros_dword_t	esp;		// Stack pointer register
		igros_dword_t	ebx;		// Base register
		igros_dword_t	edx;		// Data register
		igros_dword_t	ecx;		// Counter register
		igros_dword_t	eax;		// Accumulator register
		// Exception number pushed by us
		igros_dword_t	number;		// Exception number
		// Param pushed by processor or (0x00) by us
		igros_dword_t	param;		// Exception param (or null if no param provided)
		// This pushed to stack by processor
		igros_dword_t	eip;		// Instruction pointer register
		igros_dword_t	cs;		// Code segment register
		igros_dword_t	eflags;		// Flags register
		igros_dword_t	userEsp;	// User stack pointer register
		igros_dword_t	ss;		// Stack segment register

	};

#pragma pack(pop)


	// Print registers info
	void	dumpRegisters() noexcept;


}	// namespace igros::i386

