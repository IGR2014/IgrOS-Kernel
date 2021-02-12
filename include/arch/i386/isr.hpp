////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	isr.hpp
//	Date:	12 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>

#include <arch/i386/types.hpp>
#include <arch/i386/register.hpp>


// i386 namespace
namespace igros::i386 {


	// IRQ offset in ISR list
	constexpr auto IRQ_OFFSET	= 32U;
	// ISR list size
	constexpr auto ISR_SIZE		= 256U;

	// Interrupt service routine handler type
	using isri386_t			= std::add_pointer_t<void(const registeri386_t*)>;

	// Install interrupt service routine handler
	void isrHandlerInstall(const dword_t isrNumber, const isri386_t isrHandler) noexcept;
	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const dword_t isrNumber) noexcept;


}	// namespace igros::i386

