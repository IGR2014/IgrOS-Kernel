////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	isr.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_INTERRUPT_SERVICE_ROUTINES_HPP
#define IGROS_ARCH_INTERRUPT_SERVICE_ROUTINES_HPP


#include <type_traits>

#include <types.hpp>


// Arch-dependent code zone
namespace arch {


	// IRQ offset in ISR list
	constexpr static dword_t IRQ_OFFSET	= 32u;
	// ISR list size
	constexpr static dword_t ISR_SIZE	= 256u;

	// Forward declaration
	struct taskRegs_t;
	// Interrupt service routine handler type
	using isrHandler_t = std::add_pointer_t<void(const taskRegs_t*)>;

	// Interrupt handlers
	extern isrHandler_t isrList[ISR_SIZE];


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Interrupts handler function
		void	isrHandler(const taskRegs_t* regs) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Install interrupt service routine handler
	constexpr void isrHandlerInstall(const dword_t isrNumber, const isrHandler_t isrHandler) noexcept{
		// Put interrupt service routine handler in ISRs list
		isrList[isrNumber] = isrHandler;
	}

	// Uninstall interrupt service routine handler
	constexpr void isrHandlerUninstall(const dword_t isrNumber) noexcept {
		// Remove interrupt service routine handler from ISRs list
		isrList[isrNumber] = nullptr;
	}


}	// namespace arch


#endif	// IGROS_ARCH_INTERRUPT_SERVICE_ROUTINES_HPP

