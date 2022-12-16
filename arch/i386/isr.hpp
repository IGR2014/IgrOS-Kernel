////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	isr.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
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
	using isri386_t			= std::add_pointer_t<void(const register_t*)>;

	// Install interrupt service routine handler
	void isrHandlerInstall(const igros_dword_t isrNumber, const isri386_t isrHandler) noexcept;
	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const igros_dword_t isrNumber) noexcept;


}	// namespace igros::i386

