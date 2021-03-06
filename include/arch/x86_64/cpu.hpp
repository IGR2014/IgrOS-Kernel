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


#include <arch/x86_64/types.hpp>

#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Halt CPU
	inline void	cpuHalt() noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {


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


	// Dump registers
	inline void cpu::dumpRegisters(const register_t* const regs) noexcept {
		// Print regs
		klib::kprintf(
			u8"Registers dump:\r\n"
			u8"RAX=[%p] RBX=[%p] RCX=[%p] RDX=[%p]\r\n"
			u8"R8 =[%p] R9 =[%p] R10=[%p] R11=[%p]\r\n"
			u8"R12=[%p] R13=[%p] R14=[%p] R15=[%p]\r\n"
			u8"RSI=[%p] RDI=[%p]\r\n"
			u8"RSP=[%p] RBP=[%p]\r\n"
			u8"RIP=[%p]\r\n"
			u8"RFLAGS=[%p]\r\n"
			u8"Segments:\r\n"
			u8"CS=[%p]\r\n"
			u8"DS=[%p]\r\n"
			u8"SS=[%p]\r\n"
			u8"ES=[%p]\r\n"
			u8"FS=[%p]\r\n"
			u8"GS=[%p]\r\n"
			u8"\r\n",
			regs->rax,
			regs->rbx,
			regs->rcx,
			regs->rdx,
			regs->r8,
			regs->r9,
			regs->r10,
			regs->r11,
			regs->r12,
			regs->r13,
			regs->r14,
			regs->r15,
			regs->rsi,
			regs->rdi,
			regs->userRsp,
			regs->rbp,
			regs->rip,
			regs->rflags,
			regs->cs,
			//regs->ds,
			0ULL,
			regs->ss,
			//regs->es,
			0ULL,
			//regs->fs,
			0ULL,
			//regs->gs
			0ULL
		);
	}


}	// namespace igros::x86_64

