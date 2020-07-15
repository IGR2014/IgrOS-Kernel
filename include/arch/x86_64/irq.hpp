////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.hpp
//	Date:	14 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/x86_64/types.hpp>
#include <arch/x86_64/isr.hpp>
#include <arch/x86_64/port.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Master PIC ports
	constexpr auto PIC_MASTER_CONTROL	= static_cast<port_t>(0x0020);
	constexpr auto PIC_MASTER_DATA		= static_cast<port_t>(PIC_MASTER_CONTROL + 1);
	// Slave PIC ports
	constexpr auto PIC_SLAVE_CONTROL	= static_cast<port_t>(0x00A0);
	constexpr auto PIC_SLAVE_DATA		= static_cast<port_t>(PIC_SLAVE_CONTROL + 1);


	// Interrupts number enumeration
	enum class irq_t : dword_t {
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
		static void mask(const irq_t irqNumber) noexcept;
		// Unmask interrupt
		static void unmask(const irq_t irqNumber) noexcept;

		// Set interrupts mask
		static void			setMask(const word_t mask = 0xFFFF) noexcept;
		// Get interrupts mask
		[[nodiscard]] static word_t	getMask() noexcept;

		// Install IRQ handler
		static void install(const irq_t irqNumber, const isr_t irqHandler) noexcept;
		// Uninstall IRQ handler
		static void uninstall(const irq_t irqNumber) noexcept;


	};


}	// namespace igros::x86_64

