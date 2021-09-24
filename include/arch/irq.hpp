////////////////////////////////////////////////////////////////
//
//	Architecture interrupts type deduction
//
//	File:	types.hpp
//	Date:	24 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
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
	class interrupts_t final : public singleton<interrupts_t<T, T2>> {

		// No copy construction
		interrupts_t(const interrupts_t &other) noexcept = delete;
		// No copy assignment
		interrupts_t& operator=(const interrupts_t &other) noexcept = delete;

		// No move construction
		interrupts_t(interrupts_t &&other) noexcept = delete;
		// No move assignment
		interrupts_t& operator=(interrupts_t &&other) noexcept = delete;


	public:

		// IRQ index type
		using irq_t = T2;
		// IRQ ISR type
		using isr_t = std::add_pointer_t<void(const register_t*)>;

		// Default c-tor
		interrupts_t() noexcept = default;

		// Enable interrupts
		void enable() const noexcept;
		// Disable interrupts
		void disable() const noexcept;

		// Mask interrupt
		void mask(const irq_t number) const noexcept;
		// Unmask interrupt
		void unmask(const irq_t number) const noexcept;

		// Set interrupts mask
		void	setMask(const word_t mask = 0xFFFF) const noexcept;
		// Get interrupts mask
		[[nodiscard]]
		word_t	getMask() const noexcept;

		// Install IRQ handler
		void install(const irq_t number, const isr_t handler) const noexcept;
		// Uninstall IRQ handler
		void uninstall(const irq_t number) const noexcept;

		// IRQ done (EOI)
		void eoi(const irq_t number) const noexcept;


	};


	// Enable interrupts
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::enable() const noexcept {
		T::enable();
	}

	// Disable interrupts
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::disable() const noexcept {
		T::disable();
	}


	// Mask interrupt
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::mask(const irq_t number) const noexcept {
		T::mask(number);
	}

	// Unmask interrupt
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::unmask(const irq_t number) const noexcept {
		T::unmask(number);
	}


	// Set interrupts mask
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::setMask(const word_t mask) const noexcept {
		T::setMask(mask);
	}

	// Get interrupts mask
	template<typename T, typename T2>
	[[nodiscard]]
	inline word_t interrupts_t<T, T2>::getMask() const noexcept {
		return T::getMask();
	}


	// Install IRQ handler
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::install(const irq_t number, const isr_t handler) const noexcept {
		T::install(number, handler);
	}

	// Uninstall IRQ handler
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::uninstall(const irq_t number) const noexcept {
		T::uninstall(number);
	}


	// IRQ done (EOI)
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::eoi(const irq_t number) const noexcept {
		T::eoi(number);
	}


#if	defined (IGROS_ARCH_i386)
	// IRQ type
	using irq	= interrupts_t<i386::irq, i386::irq_t>;
#elif	defined (IGROS_ARCH_x86_64)
	// IRQ type
	using irq	= interrupts_t<x86_64::irq, x86_64::irq_t>;
#else
	// IRQ type
	using irq	= interrupts_t<void, void>;
	static_assert(false, "Unknown architecture!!!");
#endif


}	// namespace igros::arch

