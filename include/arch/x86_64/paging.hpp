////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	31 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <flags.hpp>

#include <types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Forward declaration
	struct taskRegs_t;


	// Paging structure
	class paging final {

		// Page flags
		enum class FLAGS : dword_t {
			CLEAR			= 0x00000000,
			PRESENT			= 0x00000001,
			WRITABLE		= 0x00000002,
			USER_ACCESSIBLE		= 0x00000004,
			WRITE_THROUGH		= 0x00000008,
			NON_CACHED		= 0x00000010,
			ACCESSED		= 0x00000020,
			DIRTY			= 0x00000040,
			HUGE			= 0x00000080,
			GLOBAL			= 0x00000100,
			USER_DEFINED		= 0x00000E00,
			FLAGS_MASK		= 0x00000FFF,
			PHYS_ADDR_MASK		= 0xFFFFF000
		};


	public:

		// Default c-tor
		paging() noexcept = default;

		// Copy c-tor
		paging(const paging &other) = delete;
		// Copy assignment
		paging& operator=(const paging &other) = delete;

		// Move c-tor
		paging(paging &&other) = delete;
		// Move assignment
		paging& operator=(paging &&other) = delete;

		// Init IDT table
		static void init() noexcept;

		// Enable paging
		static void enable() noexcept;
		// Disable paging
		static void disable() noexcept;

		// Enable Page Address Extension
		static void enablePAE() noexcept;
		// Disable Page Address Extension
		static void disablePAE() noexcept;

		// Map virtual page to physical page
		static void map(const pointer_t phys, const pointer_t virt, const FLAGS flags) noexcept;

		// Convert virtual address to physical address
		[[nodiscard]] static pointer_t	toPhys(const pointer_t addr) noexcept;

		// Page Fault Exception handler
		static void exHandler(const taskRegs_t* regs) noexcept;

		// Set page directory
		static void setDirectory(const pointer_t dir) noexcept;


	};


}	// namespace igros::arch

