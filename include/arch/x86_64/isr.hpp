////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	isr.hpp
//	Date:	05 Feb 2021
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
	constexpr auto IRQ_OFFSET	= 32U;
	// ISR list size
	constexpr auto ISR_SIZE		= 256U;

	// Interrupt service routine handler type
	using isrx86_64_t		= std::add_pointer_t<void(const registerx86_64_t*)>;


	// Install interrupt service routine handler
	void isrHandlerInstall(const dword_t isrNumber, const isrx86_64_t isrHandler) noexcept;
	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const dword_t isrNumber) noexcept;


}	// namespace igros::x86_64

