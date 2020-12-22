////////////////////////////////////////////////////////////////
//
//	CPU operations
//
//	File:	cpu.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/x86_64/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read CR0 register
		inline void cpuHalt() noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// CPU representation
	struct cpu final {

		// Default c-tor
		cpu() noexcept = default;

		// Copy c-tor
		cpu(const cpu &other) = delete;
		// Copy assignment
		cpu& operator=(const cpu &other) = delete;

		// Move c-tor
		cpu(cpu &&other) = delete;
		// Move assignment
		cpu& operator=(cpu &&other) = delete;

		// Halt CPU
		inline static void	halt() noexcept;


	};


	// Halt CPU
	inline void cpu::halt() noexcept {
		cpuHalt();
	}


}	// namespace igros::x86_64

