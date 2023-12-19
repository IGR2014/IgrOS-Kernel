////////////////////////////////////////////////////////////////
//
//	CPU operations
//
//	File:	cpu.hpp
//	Date:	13 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/register.hpp>
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Halt CPU
	[[noreturn]]
	void	cpuHalt() noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 platform namespace
namespace igros::i386 {


	// CPU representation
	class cpu final {

		// Copy c-tor
		cpu(const cpu &other) = delete;
		// Copy assignment
		cpu& operator=(const cpu &other) = delete;

		// Move c-tor
		cpu(cpu &&other) = delete;
		// Move assignment
		cpu& operator=(cpu &&other) = delete;


	public:

		// Default c-tor
		cpu() noexcept = default;

		// Halt CPU
		[[noreturn]]
		static void	halt() noexcept;

		// Dump CPU registers
		static void	dumpRegisters(const register_t* const regs) noexcept;


	};


	// Halt CPU
	[[noreturn]]
	inline void cpu::halt() noexcept {
		::cpuHalt();
	}


	// Dump CPU registers
	inline void cpu::dumpRegisters(const register_t* const regs) noexcept {
		// Print regs
		klib::kprintf(
R"registers(Registers dump:
	EAX=[%p] EBX=[%p] ECX=[%p] EDX=[%p]
	ESI=[%p] EDI=[%p] ESP=[%p] EBP=[%p]
	EIP=[%p] EFLAGS=[%p]
Segments:
	CS=[%p]
	DS=[%p]
	SS=[%p]
	ES=[%p]
	FS=[%p]
	GS=[%p])registers",
			regs->eax, regs->ebx, regs->ecx, regs->edx,
			regs->esi, regs->edi, regs->esp, regs->ebp,
			regs->eip, regs->eflags,
			regs->cs,
			regs->ds,
			regs->ss,
			regs->es,
			regs->fs,
			regs->gs
		);
	}


}	// namespace igros::i386

