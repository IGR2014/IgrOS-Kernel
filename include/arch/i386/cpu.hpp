////////////////////////////////////////////////////////////////
//
//	CPU operations
//
//	File:	cpu.hpp
//	Date:	24 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/i386/types.hpp>

#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Halt CPU
	inline void	cpuHalt() noexcept;


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
		static void	halt() noexcept;

		// Dump CPU registers
		static void	dumpRegisters(const register_t* const regs) noexcept;


	};


	// Halt CPU
	inline void cpu::halt() noexcept {
		::cpuHalt();
	}


	// Dump CPU registers
	inline void cpu::dumpRegisters(const register_t* const regs) noexcept {
		// Print regs
		klib::kprintf(
			"Registers dump:\r\n"
			"EAX=[%p] EBX=[%p] ECX=[%p] EDX=[%p]\r\n"
			"ESI=[%p] EDI=[%p]\r\n"
			"ESP=[%p] EBP=[%p]\r\n"
			"EIP=[%p]\r\n"
			"EFLAGS=[%p]\r\n"
			"Segments:\r\n"
			"CS=[%p]\r\n"
			"DS=[%p]\r\n"
			"SS=[%p]\r\n"
			"ES=[%p]\r\n"
			"FS=[%p]\r\n"
			"GS=[%p]\r\n"
			"\r\n",
			regs->eax,
			regs->ebx,
			regs->ecx,
			regs->edx,
			regs->esi,
			regs->edi,
			regs->esp,
			regs->ebp,
			regs->eip,
			regs->eflags,
			regs->cs,
			regs->ds,
			regs->ss,
			regs->es,
			regs->fs,
			regs->gs
		);
	}


}	// namespace igros::i386

