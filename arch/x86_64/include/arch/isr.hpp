////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.hpp
//	Date:	10 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_INTERRUPT_SERVICE_ROUTINES_HPP
#define IGROS_ARCH_INTERRUPT_SERVICE_ROUTINES_HPP


#include <type_traits>

#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


	// IRQ offset in ISR list
	static const dword_t IRQ_OFFSET = 32;


	// Forward declaration
	struct taskRegs_t;

	// Interrupt service routine handler type
	using isrHandler_t = std::add_pointer<void(const taskRegs_t* regs)>::type;


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Interrupts handler function
		void isrHandler(const taskRegs_t*);


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Install interrupt service routine handler
	void isrHandlerInstall(const dword_t, const isrHandler_t);
	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const dword_t);


}	// namespace arch


#endif	// IGROS_ARCH_INTERRUPT_SERVICE_ROUTINES_HPP

