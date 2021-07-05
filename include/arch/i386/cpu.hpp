////////////////////////////////////////////////////////////////
//
//	CPU operations
//
//	File:	cpu.hpp
//	Date:	08 Feb 2021
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
			u8"Registers dump:\r\n"
			u8"EAX=[%p] EBX=[%p] ECX=[%p] EDX=[%p]\r\n"
			u8"ESI=[%p] EDI=[%p]\r\n"
			u8"ESP=[%p] EBP=[%p]\r\n"
			u8"EIP=[%p]\r\n"
			u8"EFLAGS=[%p]\r\n"
			u8"Segments:\r\n"
			u8"CS=[%p]\r\n"
			u8"DS=[%p]\r\n"
			u8"SS=[%p]\r\n"
			u8"ES=[%p]\r\n"
			u8"FS=[%p]\r\n"
			u8"GS=[%p]\r\n"
			u8"\r\n",
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

