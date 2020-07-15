////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	isr.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
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
	constexpr auto IRQ_OFFSET	= 32u;
	// ISR list size
	constexpr auto ISR_SIZE		= 256u;

	// Interrupt service routine handler type
	using isr_t = std::add_pointer_t<void(const register_t*)>;


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Interrupts handler function
		void	isrHandler(const register_t* regs) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Install interrupt service routine handler
	void isrHandlerInstall(const dword_t isrNumber, const isr_t isrHandler) noexcept;
	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const dword_t isrNumber) noexcept;


}	// namespace igros::i386

