////////////////////////////////////////////////////////////////
//
//	CPU operations
//
//	File:	cpu.hpp
//	Date:	28 Jan 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/i386/types.hpp>


// i386 platform namespace
namespace igros::i386 {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Halt CPU
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


}	// namespace igros::i386

