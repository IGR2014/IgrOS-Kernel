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
#ifndef IGROS_ARCH_INTERRUPTS_HPP
#define IGROS_ARCH_INTERRUPTS_HPP


#include <arch/types.hpp>
#include <arch/isr.hpp>


// Arch-dependent code zone
namespace arch {


	// Interrupts number enumeration
	enum class irqNumber_t : byte_t {

		PIT		= 0,
		KEYBOARD	= 1

	};


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Interrupt 0 handler
		void	irqHandler0();
		// Interrupt 1 handler
		void	irqHandler1();
		// Interrupt 2 handler
		void	irqHandler2();
		// Interrupt 3 handler
		void	irqHandler3();
		// Interrupt 4 handler
		void	irqHandler4();
		// Interrupt 5 handler
		void	irqHandler5();
		// Interrupt 6 handler
		void	irqHandler6();
		// Interrupt 7 handler
		void	irqHandler7();
		// Interrupt 8 handler
		void	irqHandler8();
		// Interrupt 9 handler
		void	irqHandler9();
		// Interrupt 10 handler
		void	irqHandlerA();
		// Interrupt 11 handler
		void	irqHandlerB();
		// Interrupt 12 handler
		void	irqHandlerC();
		// Interrupt 13 handler
		void	irqHandlerD();
		// Interrupt 14 handler
		void	irqHandlerE();
		// Interrupt 15 handler
		void	irqHandlerF();

		// Enable interrupts
		void	irqEnable();
		// Disable interrupts
		void	irqDisable();


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Init interrupts
	void	irqInit();

	// Mask interrupts
	void	irqMask(const irqNumber_t);

	// Set interrupts mask
	void	irqMaskSet(const word_t);
	// Get interrupts mask
	word_t	irqMaskGet();

	// Install handler
	void	irqHandlerInstall(irqNumber_t, isrHandler_t);
	// Uninstall handler
	void	irqHandlerUninstall(irqNumber_t);


}	// namespace arch


#endif	// IGROS_ARCH_INTERRUPTS_HPP

