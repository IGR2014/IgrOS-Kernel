////////////////////////////////////////////////////////////////
//
//	CPU operations
//
//	File:	cpu.hpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// Common headers
#include <singleton.hpp>

// i386
#include <arch/i386/cpu.hpp>
// x86_64
#include <arch/x86_64/cpu.hpp>


// Arch namespace
namespace igros::arch {


	// CPU description type
	template<typename T>
	class cpu_t final : public singleton<cpu_t<T>> {

		// No copy construction
		cpu_t(const cpu_t &other) noexcept = delete;
		// No copy assignment
		cpu_t& operator=(const cpu_t &other) noexcept = delete;

		// No move construction
		cpu_t(cpu_t &&other) noexcept = delete;
		// No move assignment
		cpu_t& operator=(cpu_t &&other) noexcept = delete;


	public:

		// Default c-tor
		cpu_t() noexcept = default;

		// Halt CPU
		void	halt() const noexcept;

		// Dump CPU registers
		void	dumpRegisters(const register_t* const regs) const noexcept;


	};


	// Halt CPU
	template<typename T>
	inline void cpu_t<T>::halt() const noexcept {
		T::halt();
	}


	// Dump CPU registers
	template<typename T>
	inline void cpu_t<T>::dumpRegisters(const register_t* const regs) const noexcept {
		T::dumpRegisters(regs);
	}


#if	defined (IGROS_ARCH_i386)

	// CPU type
	using cpu	= cpu_t<i386::cpu>;

#elif	defined (IGROS_ARCH_x86_64)

	// CPU type
	using cpu	= cpu_t<x86_64::cpu>;

#else

	static_assert(
		false,
		"Unknown architecture!"
	);

	// CPU type
	using cpu	= cpu_t<void>;

#endif


}	// namespace igros::arch

