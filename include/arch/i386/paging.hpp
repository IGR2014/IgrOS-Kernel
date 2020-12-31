////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	21 Dec 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <flags.hpp>

#include <arch/i386/types.hpp>
#include <arch/i386/cr.hpp>
#include <arch/i386/register.hpp>


// i386 namespace
namespace igros::i386 {


	// Page directory/table max entries count
	constexpr auto	PAGE_ENTRY_SHIFT	= 10;
	// Page directory/table max entries count
	constexpr auto	PAGE_ENTRY_SIZE		= 1u << PAGE_ENTRY_SHIFT;
	// Page table max entries count
	constexpr auto	PAGE_ENTRY_MASK		= PAGE_ENTRY_SIZE - 1u;
	// Page shift
	constexpr auto	PAGE_SHIFT		= 12;
	// Page size
	constexpr auto	PAGE_SIZE		= 1u << PAGE_SHIFT;
	// Page mask
	constexpr auto	PAGE_MASK		= PAGE_SIZE - 1u;

	// Page directory ID shift
	constexpr auto	PAGE_DIRECTORY_SHIFT	= PAGE_SHIFT + PAGE_ENTRY_SHIFT;
	// Page table ID shift
	constexpr auto	PAGE_TABLE_SHIFT	= PAGE_SHIFT;


#pragma push(pack, 1)


	// Page
	union alignas(PAGE_SIZE) page_t {
		pointer_t	next;					// Pointer to next page
		byte_t		bytes[PAGE_SIZE];			// Page raw bytes
	};


	// Page table
	union alignas(PAGE_SIZE) table_t {
		pointer_t	next;					// Pointer to next table
		page_t*		pages[PAGE_ENTRY_SIZE];			// Page table entries
	};


	// Page directory
	union alignas(PAGE_SIZE) directory_t {
		table_t*	tables[PAGE_ENTRY_SIZE];		// Page directory entries
	};


#pragma pop(pack)


	// Paging structure
	class paging final {

		static page_t*	mFreePages;		// Free pages list


	public:

		// Page flags
		enum class flags_t : dword_t {
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
			FLAGS_MASK		= PAGE_MASK,
			PHYS_ADDR_MASK		= ~PAGE_MASK
		};

		// Default c-tor
		paging() = default;

		// Copy c-tor
		paging(const paging &other) = delete;
		// Copy assignment
		paging& operator=(const paging &other) = delete;

		// Move c-tor
		paging(paging &&other) = delete;
		// Move assignment
		paging& operator=(paging &&other) = delete;

		// Identity map kernel + map higher-half + self-map page directory
		static void	init() noexcept;

		// Enable paging
		static void	enable() noexcept;
		// Disable paging
		static void	disable() noexcept;

		// Enable Physical Address Extension
		static void	enablePAE() noexcept;
		// Disable Physical Address Extension
		static void	disablePAE() noexcept;

		// Enable Page Size Extension
		static void	enablePSE() noexcept;
		// Disable Page Size Extension
		static void	disablePSE() noexcept;

		// Initialize paging memory
		static void	heap(const pointer_t phys, const std::size_t size) noexcept;

		// Allocate page
		[[nodiscard]]
		static pointer_t	allocate() noexcept;
		// Deallocate page
		static void		deallocate(const pointer_t page) noexcept;

		// Make page directory
		[[nodiscard]]
		static directory_t*	makeDirectory() noexcept;
		// Make page table
		[[nodiscard]]
		static table_t*		makeTable() noexcept;

		// Check table flags
		[[nodiscard]]
		static bool	checkFlags(const table_t* table, const kflags<flags_t> flags) noexcept;
		// Check page flags
		[[nodiscard]]
		static bool	checkFlags(const page_t* page, const kflags<flags_t> flags) noexcept;

		// Map virtual page to physical page (whole table, explicit page directory)
		static void	mapTable(directory_t* const dir, const page_t* phys, const pointer_t virt, const kflags<flags_t> flags) noexcept;
		// Map virtual page to physical page (whole table)
		static void	mapTable(const page_t* phys, const pointer_t virt, const kflags<flags_t> flags) noexcept;

		// Map virtual page to physical page (single page, explicit page directory)
		static void	mapPage(directory_t* const dir, const page_t* phys, const pointer_t virt, const kflags<flags_t> flags) noexcept;
		// Map virtual page to physical page (single page)
		static void	mapPage(const page_t* phys, const pointer_t virt, const kflags<flags_t> flags) noexcept;

		// Convert virtual address to physical address
		[[nodiscard]]
		static pointer_t	translate(const pointer_t addr) noexcept;

		// Page Fault Exception handler
		static void	exHandler(const register_t* regs) noexcept;

		// Set page directory
		static void	flush(const directory_t* const dir) noexcept;


	};


}	// namespace igros::i386

