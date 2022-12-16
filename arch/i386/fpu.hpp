////////////////////////////////////////////////////////////////
//
//	FPU operations
//
//	File:	fpu.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Check FPU
	[[nodiscard]]
	auto	fpuCheck() noexcept -> bool;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 platform namespace
namespace igros::i386 {


	// FPU representation
	class fpu final {

		// Copy c-tor
		fpu(const fpu &other) = delete;
		// Copy assignment
		fpu& operator=(const fpu &other) = delete;

		// Move c-tor
		fpu(fpu &&other) = delete;
		// Move assignment
		fpu& operator=(fpu &&other) = delete;


	public:

		// Default c-tor
		fpu() noexcept = default;

		// Check FPU
		[[nodiscard]]
		static auto	check() noexcept -> bool;


	};


	// Check FPU
	[[nodiscard]]
	inline auto fpu::check() noexcept -> bool {
		return ::fpuCheck();
	}


}	// namespace igros::i386

