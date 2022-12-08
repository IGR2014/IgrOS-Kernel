////////////////////////////////////////////////////////////////
//
//	Long mode low-level preparation operations
//
//	File:	longMode.hpp
//	Date:	09 Dec 2022
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//
//


#pragma	once


#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Check if loaded with multiboot loader
	[[nodiscard]]
	auto	checkMultiboot() noexcept -> igros::igros_byte_t;
	// Check CPUID is extended
	[[nodiscard]]
	auto	checkCPUID() noexcept -> igros::igros_byte_t;
	// Check CPUID for long mode existance
	[[nodiscard]]
	auto	checkLongMode() noexcept -> igros::igros_byte_t;

	// Setup page tables
	void	setupPageTables() noexcept;
	// Enable paging
	void	enablePaging() noexcept;

	// Perform jum to long mode
	void	jumpToLongMode() noexcept;


#ifdef	__cplusplus

}

#endif	// __cplusplus

