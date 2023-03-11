////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	isr.hpp
//	Date:	12 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <array>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch i386
#include <arch/i386/register.hpp>


// i386 namespace
namespace igros::i386 {


	// IRQ offset in ISR list
	constexpr auto IRQ_OFFSET	{32_usize};
	// ISR list size
	constexpr auto ISR_SIZE		{256_usize};

	// Interrupt service routine handler type
	using isr_t			= std::add_pointer_t<void(const register_t*)>;

	// Interrupt handlers
	static auto isrList		{std::array<isr_t, ISR_SIZE> {}};

	// Install interrupt service routine handler
	template<igros_dword_t N, isr_t HANDLE>
	void isrHandlerInstall() noexcept {
		// Put interrupt service routine handler in ISRs list
		isrList[N] = HANDLE;
	}

	// Uninstall interrupt service routine handler
	template<igros_dword_t N>
	void isrHandlerUninstall() noexcept {
		// Remove interrupt service routine handler from ISRs list
		isrList[N] = nullptr;
	}


}	// namespace igros::i386

