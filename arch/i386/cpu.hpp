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
		// Print regs (32-bit values)
		klib::kprintf(
R"registers(Registers dump:
	EAX=[%08x] EBX=[%08x] ECX=[%08x] EDX=[%08x]
	ESI=[%08x] EDI=[%08x] ESP=[%08x] EBP=[%08x]
	EIP=[%08x] EFLAGS=[%08x]
Segments:
	CS=[%08x]
	DS=[%08x]
	SS=[%08x]
	ES=[%08x]
	FS=[%08x]
	GS=[%08x])registers",
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

