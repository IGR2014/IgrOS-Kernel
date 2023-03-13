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


	// Dump registers
	inline void cpu::dumpRegisters(const register_t* const regs) noexcept {
		// Print regs
		klib::kprintf(
R"registers(
Registers dump:
RAX=[%p] RBX=[%p] RCX=[%p] RDX=[%p]
R8 =[%p] R9 =[%p] R10=[%p] R11=[%p]
R12=[%p] R13=[%p] R14=[%p] R15=[%p]
RSI=[%p] RDI=[%p]
RSP=[%p] RBP=[%p]
RIP=[%p]
RFLAGS=[%p]
Segments:
CS=[%p]
DS=[%p]
SS=[%p]
ES=[%p]
FS=[%p]
GS=[%p]
)registers",
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
			0_u64,		//regs->ds,
			regs->ss,
			0_u64,		//regs->es,
			0_u64,		//regs->fs,
			0_u64		//regs->gs
		);
	}


}	// namespace igros::x86_64

