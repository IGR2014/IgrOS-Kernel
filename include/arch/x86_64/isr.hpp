////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	isr.hpp
//	Date:	16 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>

#include <arch/x86_64/types.hpp>
#include <arch/x86_64/register.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// IRQ offset in ISR list
	constexpr auto IRQ_OFFSET	= 32u;
	// ISR list size
	constexpr auto ISR_SIZE		= 256u;

	// Interrupt service routine handler type
	using isr_t = std::add_pointer_t<void(const register_t*)>;


	// Install interrupt service routine handler
	void isrHandlerInstall(const dword_t isrNumber, const isr_t isrHandler) noexcept;
	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const dword_t isrNumber) noexcept;


}	// namespace igros::x86_64

