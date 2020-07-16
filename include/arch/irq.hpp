////////////////////////////////////////////////////////////////
//
//	Architecture interrupts type deduction
//
//	File:	types.hpp
//	Date:	16 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// Common headers
#include <singleton.hpp>

// IRQ registers
#include <arch/register.hpp>

// i386
#include <arch/i386/irq.hpp>
// x86_64
#include <arch/x86_64/irq.hpp>


// Arch namespace
namespace igros::arch {


	// Interrupts description type
	template<typename T, typename T2>
	class tInterrupts final : public singleton<T> {

		// No copy construction
		tInterrupts(const tInterrupts &other) noexcept = delete;
		// No copy assignment
		tInterrupts& operator=(const tInterrupts &other) noexcept = delete;

		// No move construction
		tInterrupts(tInterrupts &&other) noexcept = delete;
		// No move assignment
		tInterrupts& operator=(tInterrupts &&other) noexcept = delete;


	public:

		// IRQ index type
		using irq_t = T2;
		// IRQ ISR type
		using isr_t = std::add_pointer_t<void(const register_t*)>;

		// Enable interrupts
	inline	void enable() noexcept;
		// Disable interrupts
	inline	void disable() noexcept;

		// Mask interrupt
	inline	void mask(const irq_t number) noexcept;
		// Unmask interrupt
	inline	void unmask(const irq_t number) noexcept;

		// Set interrupts mask
	inline	void			setMask(const word_t mask = 0xFFFF) noexcept;
		// Get interrupts mask
	[[nodiscard]] inline	word_t	getMask() noexcept;

		// Install IRQ handler
	inline	void install(const irq_t number, const isr_t handler) noexcept;
		// Uninstall IRQ handler
	inline	void uninstall(const irq_t number) noexcept;


	};


	// Enable interrupts
	template<typename T, typename T2>
	void tInterrupts<T, T2>::enable() noexcept {
		T::enable();
	}

	// Disable interrupts
	template<typename T, typename T2>
	void tInterrupts<T, T2>::disable() noexcept {
		T::disable();
	}


	// Mask interrupt
	template<typename T, typename T2>
	void tInterrupts<T, T2>::mask(const irq_t number) noexcept {
		T::mask(number);
	}

	// Unmask interrupt
	template<typename T, typename T2>
	void tInterrupts<T, T2>::unmask(const irq_t number) noexcept {
		T::unmask(number);
	}


	// Set interrupts mask
	template<typename T, typename T2>
	void tInterrupts<T, T2>::setMask(const word_t mask) noexcept {
		T::setMask(mask);
	}

	// Get interrupts mask
	template<typename T, typename T2>
	[[nodiscard]] word_t tInterrupts<T, T2>::getMask() noexcept {
		return T::getMask();
	}


	// Install IRQ handler
	template<typename T, typename T2>
	void tInterrupts<T, T2>::install(const irq_t number, const isr_t handler) noexcept {
		T::install(number, handler);
	}

	// Uninstall IRQ handler
	template<typename T, typename T2>
	void tInterrupts<T, T2>::uninstall(const irq_t number) noexcept {
		T::uninstall(number);
	}


#if	defined (IGROS_ARCH_i386)
	// IRQ type
	using irq	= tInterrupts<i386::irq, i386::irq_t>;
#elif	defined (IGROS_ARCH_x86_64)
	// IRQ type
	using irq	= tInterrupts<x86_64::irq, x86_64::irq_t>;
#else
	// IRQ type
	using irq	= tInterrupts<void, void>;
	static_assert(false, u8"Unknown architecture!!!");
#endif


}	// namespace igros::arch

