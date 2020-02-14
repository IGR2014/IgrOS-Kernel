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
		constexpr void	irqHandler0() noexcept;
		// Interrupt 1 handler
		constexpr void	irqHandler1() noexcept;
		// Interrupt 2 handler
		constexpr void	irqHandler2() noexcept;
		// Interrupt 3 handler
		constexpr void	irqHandler3() noexcept;
		// Interrupt 4 handler
		constexpr void	irqHandler4() noexcept;
		// Interrupt 5 handler
		constexpr void	irqHandler5() noexcept;
		// Interrupt 6 handler
		constexpr void	irqHandler6() noexcept;
		// Interrupt 7 handler
		constexpr void	irqHandler7() noexcept;
		// Interrupt 8 handler
		constexpr void	irqHandler8() noexcept;
		// Interrupt 9 handler
		constexpr void	irqHandler9() noexcept;
		// Interrupt 10 handler
		constexpr void	irqHandlerA() noexcept;
		// Interrupt 11 handler
		constexpr void	irqHandlerB() noexcept;
		// Interrupt 12 handler
		constexpr void	irqHandlerC() noexcept;
		// Interrupt 13 handler
		constexpr void	irqHandlerD() noexcept;
		// Interrupt 14 handler
		constexpr void	irqHandlerE() noexcept;
		// Interrupt 15 handler
		constexpr void	irqHandlerF() noexcept;


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

