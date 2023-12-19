////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	isr.hpp
//	Date:	19 Dec 2023
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
	using isr_t			= std::add_pointer_t<void (const register_t* const)>;

	// Install interrupt service routine handler
	void	isrHandlerInstall(const igros_usize_t number, const isr_t handle) noexcept;

	// Uninstall interrupt service routine handler
	void	isrHandlerUninstall(const igros_usize_t number) noexcept;


}	// namespace igros::i386

