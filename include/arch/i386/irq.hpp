////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_INTERRUPTS_HPP
#define IGROS_ARCH_INTERRUPTS_HPP


#include <types.hpp>
#include <isr.hpp>


// Arch-dependent code zone
namespace arch {


	// Interrupts number enumeration
	enum class irqNumber_t : dword_t {
		PIT		= 0u,
		KEYBOARD	= 1u
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


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// IRQ structure
	class irq final {

	public:

		// Default c-tor
		irq() noexcept = default;

		// Copy c-tor
		irq(const irq &other) = delete;
		// Copy assignment
		irq& operator=(const irq &other) = delete;

		// Move c-tor
		irq(irq &&other) = delete;
		// Move assignment
		irq& operator=(irq &&other) = delete;

		// Init IRQ
		static void init() noexcept;

		// Enable interrupts
		static void enable() noexcept;
		// Disable interrupts
		static void disable() noexcept;

		// Mask interrupt
		static void mask(const irqNumber_t irqNumber) noexcept;
		// Unmask interrupt
		static void unmask(const irqNumber_t irqNumber) noexcept;

		// Set interrupts mask
		static void			setMask(const word_t mask = 0xFFFF) noexcept;
		// Get interrupts mask
		[[nodiscard]] static word_t	getMask() noexcept;

		// Install IRQ handler
		constexpr static void install(const irqNumber_t irqNumber, const isrHandler_t irqHandler) noexcept;
		// Uninstall IRQ handler
		constexpr static void uninstall(const irqNumber_t irqNumber) noexcept;


	};


	// Install handler
	constexpr void irq::install(const irqNumber_t irqNumber, const isrHandler_t irqHandler) noexcept {
		// Install ISR
		isrHandlerInstall(dword_t(irqNumber) + IRQ_OFFSET, irqHandler);
	}

	// Uninstall handler
	constexpr void irq::uninstall(const irqNumber_t irqNumber) noexcept {
		// Uninstall ISR
		isrHandlerUninstall(dword_t(irqNumber) + IRQ_OFFSET);
	}


}	// namespace arch


#endif	// IGROS_ARCH_INTERRUPTS_HPP

