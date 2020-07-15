////////////////////////////////////////////////////////////////
//
//	Long mode low-level preparation operations
//
//	File:	longMode.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//
//


#pragma	once


#include <arch/x86_64/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Check if loaded with multiboot loader
		[[nodiscard]] constexpr byte_t checkMultiboot() noexcept;
		// Check CPUID is extended
		[[nodiscard]] constexpr byte_t checkCPUID() noexcept;
		// Check CPUID for long mode existance
		[[nodiscard]] constexpr byte_t checkLongMode() noexcept;

		// Setup page tables
		constexpr void	setupPageTables() noexcept;
		// Enable paging
		constexpr void	enablePaging() noexcept;

		// Perform jum to long mode
		constexpr void	jumpToLongMode() noexcept;


#ifdef	__cplusplus

	}

#endif	// __cplusplus


}	// namespace igros::x86_64

