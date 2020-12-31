////////////////////////////////////////////////////////////////
//
//	CPU operations
//
//	File:	cpu.hpp
//	Date:	21 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
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
	class tCPU final : public singleton<tCPU<T>> {

		// No copy construction
		tCPU(const tCPU &other) noexcept = delete;
		// No copy assignment
		tCPU& operator=(const tCPU &other) noexcept = delete;

		// No move construction
		tCPU(tCPU &&other) noexcept = delete;
		// No move assignment
		tCPU& operator=(tCPU &&other) noexcept = delete;


	public:

		// Default c-tor
		tCPU() noexcept = default;

		// Halt CPU
	inline	void halt() const noexcept;


	};


	// Halt CPU
	template<typename T>
	inline void tCPU<T>::halt() const noexcept {
		T::halt();
	}


#if	defined (IGROS_ARCH_i386)
	// CPU type
	using cpu = tCPU<i386::cpu>;
#elif	defined (IGROS_ARCH_x86_64)
	// CPU type
	using cpu = tCPU<x86_64::cpu>;
#else
	// CPU type
	using cpu = tCPU<void>;
	static_assert(false, u8"Unknown architecture!!!");
#endif


}	// namespace igros::arch

