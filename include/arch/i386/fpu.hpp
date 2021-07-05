////////////////////////////////////////////////////////////////
//
//	FPU operations
//
//	File:	fpu.hpp
//	Date:	05 Jul 2021
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


	// Check FPU
	inline bool	fpuCheck() noexcept;


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
		static bool	check() noexcept;


	};


	// Check FPU
	inline bool fpu::check() noexcept {
		return ::fpuCheck();
	}


}	// namespace igros::i386

