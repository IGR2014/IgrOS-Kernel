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


// i386 platform namespace
namespace igros::i386 {


	/// @brief CPU representation
	class cpu final {

		/// @brief Copy c-tor
		cpu(const cpu &other) = delete;
		/// @brief Copy assignment
		cpu& operator=(const cpu &other) = delete;

		/// @brief Move c-tor
		cpu(cpu &&other) = delete;
		/// @brief Move assignment
		cpu& operator=(cpu &&other) = delete;


	public:

		/// @brief Default c-tor
		cpu() noexcept = default;
		/// @brief D-tor
		~cpu() noexcept = default;

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


	/// @brief Dump CPU registers
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

