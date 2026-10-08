////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	irq.hpp
//	Date:	22 Dec 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/io.hpp>
#include <arch/x86_64/isr.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Interrupt 0 handler
	void	irqHandler0() noexcept;
	// Interrupt 1 handler
	void	irqHandler1() noexcept;
	// Interrupt 2 handler
	void	irqHandler2() noexcept;
	// Interrupt 3 handler
	void	irqHandler3() noexcept;
	// Interrupt 4 handler
	void	irqHandler4() noexcept;
	// Interrupt 5 handler
	void	irqHandler5() noexcept;
	// Interrupt 6 handler
	void	irqHandler6() noexcept;
	// Interrupt 7 handler
	void	irqHandler7() noexcept;
	// Interrupt 8 handler
	void	irqHandler8() noexcept;
	// Interrupt 9 handler
	void	irqHandler9() noexcept;
	// Interrupt 10 handler
	void	irqHandlerA() noexcept;
	// Interrupt 11 handler
	void	irqHandlerB() noexcept;
	// Interrupt 12 handler
	void	irqHandlerC() noexcept;
	// Interrupt 13 handler
	void	irqHandlerD() noexcept;
	// Interrupt 14 handler
	void	irqHandlerE() noexcept;
	// Interrupt 15 handler
	void	irqHandlerF() noexcept;

	// Save flags and disable interrupts
	[[nodiscard]]
	auto	irqSave() noexcept -> igros::igros_usize_t;
	// Restore flags (interrupts state)
	void	irqRestore(const igros::igros_usize_t flags) noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {


	// Master PIC ports
	constexpr auto PIC_MASTER_CONTROL	{static_cast<port_t>(0x0020_u16)};
	constexpr auto PIC_MASTER_DATA		{static_cast<port_t>(PIC_MASTER_CONTROL + 1_u16)};
	// Slave PIC ports
	constexpr auto PIC_SLAVE_CONTROL	{static_cast<port_t>(0x00A0_u16)};
	constexpr auto PIC_SLAVE_DATA		{static_cast<port_t>(PIC_SLAVE_CONTROL + 1_u16)};


	// Interrupts number enumeration (PIC lines)
	enum class irq_t : igros_dword_t {
		PIT		= 0_u32,
		KEYBOARD	= 1_u32,
		CASCADE		= 2_u32,		// Slave PIC
		UART2		= 3_u32,
		UART1		= 4_u32,
		LPT2		= 5_u32,
		FLOPPY		= 6_u32,
		LPT1		= 7_u32,		// Master PIC spurious
		RTC		= 8_u32,
		ACPI		= 9_u32,
		FREE_10		= 10_u32,
		FREE_11		= 11_u32,
		MOUSE		= 12_u32,
		FPU		= 13_u32,
		ATA_PRIMARY	= 14_u32,
		ATA_SECONDARY	= 15_u32		// Slave PIC spurious
	};

	// Number of PIC interrupt lines
	constexpr auto IRQ_LINES	{16_usize};


	// Interrupt vector of IRQ line
	[[nodiscard]]
	constexpr auto irqVector(const irq_t line) noexcept -> igros_usize_t {
		return IRQ_OFFSET + static_cast<igros_usize_t>(line);
	}

	// Check if interrupt vector belongs to IRQ line
	[[nodiscard]]
	constexpr auto isIrqVector(const igros_usize_t vector) noexcept -> bool {
		return (vector >= IRQ_OFFSET) && (vector < (IRQ_OFFSET + IRQ_LINES));
	}

	// IRQ line of interrupt vector (vector must be IRQ vector)
	[[nodiscard]]
	constexpr auto irqFromVector(const igros_usize_t vector) noexcept -> irq_t {
		return static_cast<irq_t>(vector - IRQ_OFFSET);
	}


	// IRQ structure
	class irq final {

		// Copy c-tor
		irq(const irq &other) = delete;
		// Copy assignment
		auto	operator=(const irq &other) -> irq& = delete;

		// Move c-tor
		irq(irq &&other) = delete;
		// Move assignment
		auto	operator=(irq &&other) -> irq& = delete;


	public:

		// Default c-tor
		irq() noexcept = default;

		// Init IRQ
		static void	init() noexcept;

		// Enable interrupts
		static void	enable() noexcept;
		// Disable interrupts
		static void	disable() noexcept;

		// Mask interrupt
		static void	mask(const irq_t number) noexcept;
		// Unmask interrupt
		static void	unmask(const irq_t number) noexcept;

		// Set interrupts mask
		static void	setMask(const igros_word_t mask = 0xFFFF_u16) noexcept;
		// Get interrupts mask
		[[nodiscard]]
		static auto	getMask() noexcept -> igros_word_t;

		// Install IRQ handler
		template<irq_t N, isr_t HANDLE>
		static void	install() noexcept;
		// Uninstall IRQ handler
		template<irq_t N>
		static void	uninstall() noexcept;

		// Send EOI (IRQ done)
		static void	eoi(const irq_t number) noexcept;

		// Check if IRQ is spurious (only lines 7 and 15 can be)
		[[nodiscard]]
		static auto	isSpurious(const irq_t number) noexcept -> bool;

		// Disable interrupts in scope (restores previous state on exit)
		class guard;


	};


	// Disable interrupts in scope (restores previous state on exit)
	class [[nodiscard]] irq::guard final {

		// Saved flags
		igros_usize_t	mFlags;

		// Copy c-tor
		guard(const guard &other) = delete;
		// Copy assignment
		auto	operator=(const guard &other) -> guard& = delete;


	public:

		// Save flags and disable interrupts
		guard() noexcept : mFlags {::irqSave()} {}
		// Restore flags
		~guard() noexcept {
			::irqRestore(mFlags);
		}


	};


	// Install handler
	template<irq_t N, isr_t HANDLE>
	inline void irq::install() noexcept {
		// Install ISR
		isrHandlerInstall(irqVector(N), HANDLE);
	}

	// Uninstall handler
	template<irq_t N>
	inline void irq::uninstall() noexcept {
		// Uninstall ISR
		isrHandlerUninstall(irqVector(N));
	}


}	// namespace igros::x86_64

