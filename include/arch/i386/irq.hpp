////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.hpp
//	Date:	02 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/i386/types.hpp>
#include <arch/i386/port.hpp>
#include <arch/i386/isr.hpp>


// i386 namespace
namespace igros::i386 {


	// Master PIC ports
	constexpr auto PIC_MASTER_CONTROL	= port_t {0x0020};
	constexpr auto PIC_MASTER_DATA		= port_t {PIC_MASTER_CONTROL + 1};
	// Slave PIC ports
	constexpr auto PIC_SLAVE_CONTROL	= port_t {0x00A0};
	constexpr auto PIC_SLAVE_DATA		= port_t {PIC_SLAVE_CONTROL + 1};


	// Interrupts number enumeration
	enum class irq_t : dword_t {
		PIT		= 0u,
		KEYBOARD	= 1u,
		PIC		= 2u,
		UART2		= 3u,
		UART1		= 4u
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

		// Copy c-tor
		irq(const irq &other) = delete;
		// Copy assignment
		irq& operator=(const irq &other) = delete;

		// Move c-tor
		irq(irq &&other) = delete;
		// Move assignment
		irq& operator=(irq &&other) = delete;


	public:

		// Default c-tor
		irq() noexcept = default;

		// Init IRQ
		static void init() noexcept;

		// Enable interrupts
		static void enable() noexcept;
		// Disable interrupts
		static void disable() noexcept;

		// Mask interrupt
		static void mask(const irq_t number) noexcept;
		// Unmask interrupt
		static void unmask(const irq_t number) noexcept;

		// Set interrupts mask
		static void			setMask(const word_t mask = 0xFFFF) noexcept;
		// Get interrupts mask
		[[nodiscard]] static word_t	getMask() noexcept;

		// Install IRQ handler
		static void install(const irq_t number, const isr_t handler) noexcept;
		// Uninstall IRQ handler
		static void uninstall(const irq_t number) noexcept;

		// Send EOI (IRQ done)
		static void eoi(const irq_t number) noexcept;


	};


}	// namespace igros::i386

