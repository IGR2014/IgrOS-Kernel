////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Architecture interrupts type deduction
///	@date		21 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <concepts>
// IgrOS-Kernel arch
#include <arch/register.hpp>

#if	defined (IGROS_ARCH_i386) || defined (IGROS_ARCH_x86_64)

// IgrOS-Kernel arch x86
#include <arch/x86/irq.hpp>

#else

#error "Unknown architecture!"

#endif


// Arch namespace
namespace igros::arch {


	/// @brief Interrupts control every architecture must provide
	///
	/// (plus "install<line, handler>()" / "uninstall<line>()" templates)
	template<class T, class L>
	concept Interrupts = requires(const L line) {
		{ T::enable() }		-> std::same_as<void>;
		{ T::disable() }	-> std::same_as<void>;
		{ T::mask(line) }	-> std::same_as<void>;
		{ T::unmask(line) }	-> std::same_as<void>;
		{ T::eoi(line) }	-> std::same_as<void>;
		// Disables interrupts in scope, restores previous state on exit
		typename T::guard;
	};


#if	defined (IGROS_ARCH_i386) || defined (IGROS_ARCH_x86_64)

	/// @brief IRQ line type
	using x86::irq_t;
	/// @brief IRQ handler type
	using x86::isr_t;
	/// @brief IRQ type
	using x86::irq;

#endif


	static_assert(Interrupts<irq, irq_t>, "Architecture IRQ doesn't provide interrupts interface!");


}	// namespace igros::arch

