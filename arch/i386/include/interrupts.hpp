////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	interrupts.hpp
//	Date:	08 Aug. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_INTERRUPTS_HPP
#define IGROS_ARCH_INTERRUPTS_HPP


#include <type_traits>

#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Forward declaration
	struct taskRegs_t;


	// Interrupt handler type
	using irqHandler_t = std::add_pointer<void(const taskRegs_t* regs)>::type;


	// Interrupts number enumeration
	enum irqNumber_t {

		PIT		= 0,
		KEYBOARD	= 1

	};


	extern "C" {


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

		// Interrupts handler function
		void	irqHandler(const taskRegs_t*);

		// Enable interrupts
		void	irqEnable();
		// Disable interrupts
		void	irqDisable();


	}	// extern "C"


	// Init interrupts
	void	irqInit();

	// Mask interrupts
	void	irqMask(const irqNumber_t);

	// Set interrupts mask
	void	irqMaskSet(const word_t);
	// Get interrupts mask
	word_t	irqMaskGet();

	// Install handler
	void	irqHandlerInstall(irqNumber_t, irqHandler_t);
	// Uninstall handler
	void	irqHandlerUninstall(irqNumber_t);


}	// namespace arch


#endif	// IGROS_ARCH_INTERRUPTS_HPP

