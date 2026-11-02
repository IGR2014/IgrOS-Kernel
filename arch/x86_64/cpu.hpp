////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		CPU operations
///	@date		13 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/register.hpp>
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kPrint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	/// @brief Halt CPU
	[[noreturn]]
	void	cpuHalt() noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {


	/// @brief CPU representation
	class cpu final {

	public:

		/// @brief Default c-tor
		cpu() noexcept = default;
		/// @brief D-tor
		~cpu() noexcept = default;

		/// @brief Copy c-tor
		cpu(const cpu &other) = delete;
		/// @brief Copy assignment
		auto	operator=(const cpu &other) -> cpu& = delete;
		/// @brief Move c-tor
		cpu(cpu &&other) = delete;
		/// @brief Move assignment
		auto	operator=(cpu &&other) -> cpu& = delete;

		/// @brief Halt CPU
		[[noreturn]]
		static void	halt() noexcept;

		/// @brief Dump CPU registers
		/// @param regs Registers saved on interrupt
		static void	dumpRegisters(const register_t* const regs) noexcept;


	};


	/// @brief Halt CPU
	[[noreturn]]
	inline void cpu::halt() noexcept {
		::cpuHalt();
	}


	/// @brief Dump registers
	inline void cpu::dumpRegisters(const register_t* const regs) noexcept {
		// 64-bit register value as format "%llx" argument
		constexpr auto r = [](const igros_quad_t value) constexpr noexcept {
			return static_cast<unsigned long long>(value);
		};
		// Print regs
		klib::kprintf(
R"registers(
Registers dump:
RAX=[%016llx] RBX=[%016llx] RCX=[%016llx] RDX=[%016llx]
R8 =[%016llx] R9 =[%016llx] R10=[%016llx] R11=[%016llx]
R12=[%016llx] R13=[%016llx] R14=[%016llx] R15=[%016llx]
RSI=[%016llx] RDI=[%016llx]
RSP=[%016llx] RBP=[%016llx]
RIP=[%016llx]
RFLAGS=[%016llx]
Segments:
CS=[%016llx]
DS=[%016llx]
SS=[%016llx]
ES=[%016llx]
FS=[%016llx]
GS=[%016llx]
)registers",
			r(regs->rax),
			r(regs->rbx),
			r(regs->rcx),
			r(regs->rdx),
			r(regs->r8),
			r(regs->r9),
			r(regs->r10),
			r(regs->r11),
			r(regs->r12),
			r(regs->r13),
			r(regs->r14),
			r(regs->r15),
			r(regs->rsi),
			r(regs->rdi),
			r(regs->userRsp),
			r(regs->rbp),
			r(regs->rip),
			r(regs->rflags),
			r(regs->cs),
			r(0_u64),		//r(regs->ds),
			r(regs->ss),
			r(0_u64),		//r(regs->es),
			r(0_u64),		//r(regs->fs),
			r(0_u64)		//r(regs->gs)
		);
	}


}	// namespace igros::x86_64

