////////////////////////////////////////////////////////////////
//
//	Task state segment (i386)
//
//	File:	tss.hpp
//	Date:	22 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// i386 namespace
namespace igros::i386 {


	// Task state segments: kernel task (CPU saves interrupted state there on
	// task switch, ring 0 stack for future user mode) and double fault task
	// (reached through IDT task gate: known good stack and page directory
	// even if kernel stack overflowed)
	class tss final {

		// Copy c-tor
		tss(const tss &other) = delete;
		// Copy assignment
		auto	operator=(const tss &other) -> tss& = delete;


	public:

		// Double fault task stack size
		constexpr static auto	STACK_SIZE	{0x4000_usize};

#pragma pack(push, 1)

		// Task state segment layout (segment registers in low 16 bits)
		struct segment_t {
			igros_dword_t	link;		// Previous task selector
			igros_dword_t	esp0;		// Stack for CPL change to ring 0
			igros_dword_t	ss0;
			igros_dword_t	esp1;		// Stack for CPL change to ring 1
			igros_dword_t	ss1;
			igros_dword_t	esp2;		// Stack for CPL change to ring 2
			igros_dword_t	ss2;
			igros_dword_t	cr3;		// Page directory
			igros_dword_t	eip;
			igros_dword_t	eflags;
			igros_dword_t	eax;
			igros_dword_t	ecx;
			igros_dword_t	edx;
			igros_dword_t	ebx;
			igros_dword_t	esp;
			igros_dword_t	ebp;
			igros_dword_t	esi;
			igros_dword_t	edi;
			igros_dword_t	es;
			igros_dword_t	cs;
			igros_dword_t	ss;
			igros_dword_t	ds;
			igros_dword_t	fs;
			igros_dword_t	gs;
			igros_dword_t	ldt;		// LDT selector
			igros_word_t	trap;		// Debug trap on task switch
			igros_word_t	iomapBase;	// I/O permission bitmap offset
		};

#pragma pack(pop)

		static_assert(104_usize == sizeof(segment_t), "TSS layout must match CPU");

		// Default c-tor
		tss() noexcept = delete;

		// Set up task state segments (double fault task uses current page directory)
		static void	init(const igros_word_t codeSelector, const igros_word_t dataSelector) noexcept;

		// Kernel task state segment
		[[nodiscard]]
		static auto	kernel() noexcept -> segment_t&;
		// Double fault task state segment
		[[nodiscard]]
		static auto	doubleFault() noexcept -> segment_t&;


	};


}	// namespace igros::i386

