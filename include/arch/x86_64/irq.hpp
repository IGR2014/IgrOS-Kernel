////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.hpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/x86_64/types.hpp>
#include <arch/x86_64/isr.hpp>
#include <arch/x86_64/io.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Interrupt 0 handler
	inline void	irqHandler0() noexcept;
	// Interrupt 1 handler
	inline void	irqHandler1() noexcept;
	// Interrupt 2 handler
	inline void	irqHandler2() noexcept;
	// Interrupt 3 handler
	inline void	irqHandler3() noexcept;
	// Interrupt 4 handler
	inline void	irqHandler4() noexcept;
	// Interrupt 5 handler
	inline void	irqHandler5() noexcept;
	// Interrupt 6 handler
	inline void	irqHandler6() noexcept;
	// Interrupt 7 handler
	inline void	irqHandler7() noexcept;
	// Interrupt 8 handler
	inline void	irqHandler8() noexcept;
	// Interrupt 9 handler
	inline void	irqHandler9() noexcept;
	// Interrupt 10 handler
	inline void	irqHandlerA() noexcept;
	// Interrupt 11 handler
	inline void	irqHandlerB() noexcept;
	// Interrupt 12 handler
	inline void	irqHandlerC() noexcept;
	// Interrupt 13 handler
	inline void	irqHandlerD() noexcept;
	// Interrupt 14 handler
	inline void	irqHandlerE() noexcept;
	// Interrupt 15 handler
	inline void	irqHandlerF() noexcept;

#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {


	// Master PIC ports
	constexpr auto PIC_MASTER_CONTROL	= portx86_64_t {0x0020};
	constexpr auto PIC_MASTER_DATA		= portx86_64_t {PIC_MASTER_CONTROL + 1U};
	// Slave PIC ports
	constexpr auto PIC_SLAVE_CONTROL	= portx86_64_t {0x00A0};
	constexpr auto PIC_SLAVE_DATA		= portx86_64_t {PIC_SLAVE_CONTROL + 1U};


	// Interrupts number enumeration
	enum class irqx86_64_t : dword_t {
		PIT		= 0U,
		KEYBOARD	= 1U,
		PIC		= 2U,
		UART2		= 3U,
		UART1		= 4U
	};


	// IRQ structure
	class irqx86_64 final {

		// Copy c-tor
		irqx86_64(const irqx86_64 &other) = delete;
		// Copy assignment
		irqx86_64& operator=(const irqx86_64 &other) = delete;

		// Move c-tor
		irqx86_64(irqx86_64 &&other) = delete;
		// Move assignment
		irqx86_64& operator=(irqx86_64 &&other) = delete;


	public:

		// Default c-tor
		irqx86_64() noexcept = default;

		// Init IRQ
		static void init() noexcept;

		// Enable interrupts
		static void enable() noexcept;
		// Disable interrupts
		static void disable() noexcept;

		// Mask interrupt
		static void mask(const irqx86_64_t number) noexcept;
		// Unmask interrupt
		static void unmask(const irqx86_64_t number) noexcept;

		// Set interrupts mask
		static void	setMask(const word_t mask = 0xFFFF) noexcept;
		// Get interrupts mask
		[[nodiscard]]
		static word_t	getMask() noexcept;

		// Install IRQ handler
		static void install(const irqx86_64_t number, const isrx86_64_t handler) noexcept;
		// Uninstall IRQ handler
		static void uninstall(const irqx86_64_t number) noexcept;

		// Send EOI (IRQ done)
		static void eoi(const irqx86_64_t number) noexcept;


	};


}	// namespace igros::x86_64

