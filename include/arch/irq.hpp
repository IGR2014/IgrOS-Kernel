////////////////////////////////////////////////////////////////
//
//	Architecture interrupts type deduction
//
//	File:	types.hpp
//	Date:	21 Jul 2020
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
	class tInterrupts final : public singleton<tInterrupts<T, T2>> {

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

		// Default c-tor
		tInterrupts() noexcept = default;

		// Enable interrupts
	inline	void enable() const noexcept;
		// Disable interrupts
	inline	void disable() const noexcept;

		// Mask interrupt
	inline	void mask(const irq_t number) const noexcept;
		// Unmask interrupt
	inline	void unmask(const irq_t number) const noexcept;

		// Set interrupts mask
	inline	void	setMask(const word_t mask = 0xFFFF) const noexcept;
		// Get interrupts mask
		[[nodiscard]]
	inline	word_t	getMask() const noexcept;

		// Install IRQ handler
	inline	void install(const irq_t number, const isr_t handler) const noexcept;
		// Uninstall IRQ handler
	inline	void uninstall(const irq_t number) const noexcept;

		// IRQ done (EOI)
	inline	void eoi(const irq_t number) const noexcept;


	};


	// Enable interrupts
	template<typename T, typename T2>
	inline void tInterrupts<T, T2>::enable() const noexcept {
		T::enable();
	}

	// Disable interrupts
	template<typename T, typename T2>
	inline void tInterrupts<T, T2>::disable() const noexcept {
		T::disable();
	}


	// Mask interrupt
	template<typename T, typename T2>
	inline void tInterrupts<T, T2>::mask(const irq_t number) const noexcept {
		T::mask(number);
	}

	// Unmask interrupt
	template<typename T, typename T2>
	inline void tInterrupts<T, T2>::unmask(const irq_t number) const noexcept {
		T::unmask(number);
	}


	// Set interrupts mask
	template<typename T, typename T2>
	inline void tInterrupts<T, T2>::setMask(const word_t mask) const noexcept {
		T::setMask(mask);
	}

	// Get interrupts mask
	template<typename T, typename T2>
	[[nodiscard]]
	inline word_t tInterrupts<T, T2>::getMask() const noexcept {
		return T::getMask();
	}


	// Install IRQ handler
	template<typename T, typename T2>
	inline void tInterrupts<T, T2>::install(const irq_t number, const isr_t handler) const noexcept {
		T::install(number, handler);
	}

	// Uninstall IRQ handler
	template<typename T, typename T2>
	inline void tInterrupts<T, T2>::uninstall(const irq_t number) const noexcept {
		T::uninstall(number);
	}


	// IRQ done (EOI)
	template<typename T, typename T2>
	inline void tInterrupts<T, T2>::eoi(const irq_t number) const noexcept {
		T::eoi(number);
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

