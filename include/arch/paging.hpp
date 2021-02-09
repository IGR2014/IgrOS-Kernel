////////////////////////////////////////////////////////////////
//
//	Paging operations
//
//	File:	paging.hpp
//	Date:	03 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
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
	class tPaging final : public singleton<tPaging<T>> {

		// No copy construction
		tPaging(const tPaging &other) noexcept = delete;
		// No copy assignment
		tPaging& operator=(const tPaging &other) noexcept = delete;

		// No move construction
		tPaging(tPaging &&other) noexcept = delete;
		// No move assignment
		tPaging& operator=(tPaging &&other) noexcept = delete;


	public:

		// Virtual address pointer
		using virt_t = pointer_t;
		// Physical address pointer
		using phys_t = pointer_t;

		// Default c-tor
		tPaging() noexcept = default;

		// Enable paging
		void	enable() const noexcept;
		// Disable paging
		void	disable() const noexcept;

		// Translate virtual address to physical
		[[nodiscard]]
		phys_t	translate(const virt_t addr) const noexcept;

		// Map virtual address to physical address
		void	map(const phys_t phys, const virt_t virt, const std::size_t count, const kflags<flags_t> flags) noexcept;

		// Get paging data
		[[nodiscard]]
		phys_t	directory() const noexcept;
		// Flush paging data
		void	flush(const phys_t addr) noexcept;


	};


	// Enable paging
	template<typename T>
	void tPaging<T>::enable() const noexcept {
		T::enable();
	}

	// Disable paging
	template<typename T>
	void tPaging<T>::disable() const noexcept {
		T::disable();
	}


	// Translate virtual address to physical
	template<typename T>
	[[nodiscard]]
	tPaging<T>::phys_t tPaging<T>::translate(const virt_t addr) const noexcept {
		return T::translate(addr);
	}


	// Map virtual address to physical address
	template<typename T>
	void tPaging<T>::map(const phys_t phys, const virt_t virt, const std::size_t count, const kflags<flags_t> flags) noexcept {
		// TODO
	}


	// Get paging data
	template<typename T>
	[[nodiscard]]
	tPaging<T>::phys_t tPaging<T>::directory() const noexcept {
		return nullptr;
	}

	// Flush paging data
	template<typename T>
	void tPaging<T>::flush(const phys_t addr) noexcept {
		T::setDirectory(addr);
	}


#if	defined (IGROS_ARCH_i386)
	// Paging type
	using paging = tPaging<i386::paging>;
#elif	defined (IGROS_ARCH_x86_64)
	// Paging type
	using paging = tPaging<x86_64::paging>;
#else
	// Paging type
	using paging = tPaging<void>;
	static_assert(false, u8"Unknown architecture!!!");
#endif


}	// namespace igros::arch

