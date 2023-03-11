////////////////////////////////////////////////////////////////
//
//	Architecture interrupts type deduction
//
//	File:	types.hpp
//	Date:	12 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/register.hpp>
// IgrOS-Kernel arch i386
#include <arch/i386/irq.hpp>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/irq.hpp>
// IgrOS-Kernel library
#include <klib/ksingleton.hpp>


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
		void	enable() const noexcept;
		// Disable interrupts
		void	disable() const noexcept;

		// Mask interrupt
		void	mask(const T2 number) const noexcept;
		// Unmask interrupt
		void	unmask(const T2 number) const noexcept;

		// Set interrupts mask
		void	setMask(const igros_word_t mask = 0xFFFF_u16) const noexcept;
		// Get interrupts mask
		[[nodiscard]]
		auto	getMask() const noexcept -> igros_word_t;

		// Install IRQ handler
		template<T2 N, std::add_pointer_t<void (const register_t*)> HANDLE>
		void	install() const noexcept;
		// Uninstall IRQ handler
		template<T2 N>
		void	uninstall() const noexcept;

		// IRQ done (EOI)
		void	eoi(const T2 number) const noexcept;


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
	inline void interrupts_t<T, T2>::mask(const T2 number) const noexcept {
		T::mask(number);
	}

	// Unmask interrupt
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::unmask(const T2 number) const noexcept {
		T::unmask(number);
	}


	// Set interrupts mask
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::setMask(const igros_word_t mask) const noexcept {
		T::setMask(mask);
	}

	// Get interrupts mask
	template<typename T, typename T2>
	[[nodiscard]]
	inline auto interrupts_t<T, T2>::getMask() const noexcept -> igros_word_t {
		return T::getMask();
	}


	// Install IRQ handler
	template<typename T, typename T2>
	template<T2 N, std::add_pointer_t<void (const register_t*)> HANDLE>
	inline void interrupts_t<T, T2>::install() const noexcept {
		T::template install<N, HANDLE>();
	}

	// Uninstall IRQ handler
	template<typename T, typename T2>
	template<T2 N>
	inline void interrupts_t<T, T2>::uninstall() const noexcept {
		T::template uninstall<N>();
	}


	// IRQ done (EOI)
	template<typename T, typename T2>
	inline void interrupts_t<T, T2>::eoi(const T2 number) const noexcept {
		T::eoi(number);
	}


#if	defined (IGROS_ARCH_i386)

	// IRQ type
	using irq	= interrupts_t<i386::irq, i386::irq_t>;

#elif	defined (IGROS_ARCH_x86_64)

	// IRQ type
	using irq	= interrupts_t<x86_64::irq, x86_64::irq_t>;

#else

	static_assert(
		false,
		"Unknown architecture!"
	);

	// IRQ type
	using irq	= interrupts_t<void, void>;

#endif


}	// namespace igros::arch

