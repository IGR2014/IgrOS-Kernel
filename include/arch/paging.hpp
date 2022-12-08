////////////////////////////////////////////////////////////////
//
//	Paging operations
//
//	File:	paging.hpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// Common headers
#include <singleton.hpp>

// i386
#include <arch/i386/paging.hpp>
// x86_64
#include <arch/x86_64/paging.hpp>


// Arch namespace
namespace igros::arch {


	// Paging description type
	template<typename T>
	class paging_t final : public singleton<paging_t<T>> {

		// No copy construction
		paging_t(const paging_t &other) noexcept = delete;
		// No copy assignment
		paging_t& operator=(const paging_t &other) noexcept = delete;

		// No move construction
		paging_t(paging_t &&other) noexcept = delete;
		// No move assignment
		paging_t& operator=(paging_t &&other) noexcept = delete;


	public:

		// Virtual address pointer
		using virt_t = igros_pointer_t;
		// Physical address pointer
		using phys_t = igros_pointer_t;

		// Default c-tor
		paging_t() noexcept = default;

		// Enable paging
		void	enable() const noexcept;
		// Disable paging
		void	disable() const noexcept;

		// Translate virtual address to physical
		[[nodiscard]]
		auto	translate(const virt_t addr) const noexcept -> phys_t;

		// Map virtual address to physical address
		void	map(const phys_t phys, const virt_t virt, const igros_usize_t count, const kflags<flags_t> flags) noexcept;

		// Get paging data
		[[nodiscard]]
		auto	directory() const noexcept -> phys_t;
		// Flush paging data
		void	flush(const phys_t addr) noexcept;


	};


	// Enable paging
	template<typename T>
	void paging_t<T>::enable() const noexcept {
		T::enable();
	}

	// Disable paging
	template<typename T>
	void paging_t<T>::disable() const noexcept {
		T::disable();
	}


	// Translate virtual address to physical
	template<typename T>
	[[nodiscard]]
	auto paging_t<T>::translate(const virt_t addr) const noexcept -> paging_t<T>::phys_t {
		return T::translate(addr);
	}


	// Map virtual address to physical address
	template<typename T>
	void paging_t<T>::map(const phys_t phys, const virt_t virt, const igros_usize_t count, const kflags<flags_t> flags) noexcept {
		// TODO
	}


	// Get paging data
	template<typename T>
	[[nodiscard]]
	auto paging_t<T>::directory() const noexcept -> paging_t<T>::phys_t {
		return nullptr;
	}

	// Flush paging data
	template<typename T>
	void paging_t<T>::flush(const phys_t addr) noexcept {
		T::setDirectory(addr);
	}


#if	defined (IGROS_ARCH_i386)

	// Paging type
	using paging	= paging_t<i386::paging>;

#elif	defined (IGROS_ARCH_x86_64)

	// Paging type
	using paging	= paging_t<x86_64::paging>;

#else

	static_assert(
		false,
		"Unknown architecture!"
	);

	// Paging type
	using paging	= paging_t<void>;

#endif


}	// namespace igros::arch

