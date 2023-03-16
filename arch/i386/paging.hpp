////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	16 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch i386
#include <arch/i386/cr.hpp>
#include <arch/i386/register.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>


// i386 namespace
namespace igros::i386 {


	// Page directory/table max entries count
	constexpr auto	PAGE_ENTRY_SHIFT	{10_usize};
	// Page directory/table max entries count
	constexpr auto	PAGE_ENTRY_SIZE		{1_usize << PAGE_ENTRY_SHIFT};
	// Page table max entries count
	constexpr auto	PAGE_ENTRY_MASK		{PAGE_ENTRY_SIZE - 1_usize};
	// Page shift
	constexpr auto	PAGE_SHIFT		{12_usize};
	// Page size
	constexpr auto	PAGE_SIZE		{1_usize << PAGE_SHIFT};
	// Page mask
	constexpr auto	PAGE_MASK		{PAGE_SIZE - 1_usize};

	// Page directory ID shift
	constexpr auto	PAGE_DIRECTORY_SHIFT	{PAGE_SHIFT + PAGE_ENTRY_SHIFT};
	// Page table ID shift
	constexpr auto	PAGE_TABLE_SHIFT	{PAGE_SHIFT};


#pragma push(pack, 1)


	// Page
	union alignas(PAGE_SIZE) page_t {
		igros_pointer_t	next;					// Pointer to next page
		igros_byte_t	bytes[PAGE_SIZE];			// Page raw bytes
	};


	// Page table
	union alignas(PAGE_SIZE) table_t {
		igros_pointer_t	next;					// Pointer to next table
		page_t*		pages[PAGE_ENTRY_SIZE];			// Page table entries
	};


	// Page directory
	union alignas(PAGE_SIZE) directory_t {
		table_t*	tables[PAGE_ENTRY_SIZE];		// Page directory entries
	};


#pragma pop(pack)


	// Paging structure
	class paging final {

		static page_t*	mFreePages;				// Free pages list

		// Copy c-tor
		paging(const paging &other) = delete;
		// Copy assignment
		paging& operator=(const paging &other) = delete;

		// Move c-tor
		paging(paging &&other) = delete;
		// Move assignment
		paging& operator=(paging &&other) = delete;


	public:

		// Page flags
		enum class FLAGS : igros_dword_t {
			CLEAR			= 0x00000000_u32,
			PRESENT			= 0x00000001_u32,
			WRITABLE		= 0x00000002_u32,
			USER_ACCESSIBLE		= 0x00000004_u32,
			WRITE_THROUGH		= 0x00000008_u32,
			NON_CACHED		= 0x00000010_u32,
			ACCESSED		= 0x00000020_u32,
			DIRTY			= 0x00000040_u32,
			HUGE			= 0x00000080_u32,
			GLOBAL			= 0x00000100_u32,
			USER_DEFINED		= 0x00000E00_u32,
			FLAGS_MASK		= PAGE_MASK,
			PHYS_ADDR_MASK		= ~PAGE_MASK
		};

		// Default c-tor
		paging() noexcept = default;

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
		static void	heap(const igros_pointer_t phys, const igros_usize_t size) noexcept;

		// Allocate page
		[[nodiscard]]
		static auto	allocate() noexcept -> igros_pointer_t;
		// Deallocate page
		static void	deallocate(const igros_pointer_t page) noexcept;

		// Make page directory
		[[nodiscard]]
		static auto	makeDirectory() noexcept -> directory_t*;
		// Make page table
		[[nodiscard]]
		static auto	makeTable() noexcept -> table_t*;

		// Check table flags
		[[nodiscard]]
		static auto	checkFlags(const table_t* table, const klib::kFlags<FLAGS> flags) noexcept -> bool;
		// Check page flags
		[[nodiscard]]
		static auto	checkFlags(const page_t* page, const klib::kFlags<FLAGS> flags) noexcept -> bool;

		// Map virtual page to physical page (whole table, explicit page directory)
		static void	mapTable(directory_t* const dir, const page_t* phys, const igros_pointer_t virt, const klib::kFlags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (whole table)
		static void	mapTable(const page_t* phys, const igros_pointer_t virt, const klib::kFlags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single page, explicit page directory)
		static void	mapPage(directory_t* const dir, const page_t* phys, const igros_pointer_t virt, const klib::kFlags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single page)
		static void	mapPage(const page_t* phys, const igros_pointer_t virt, const klib::kFlags<FLAGS> flags) noexcept;

		// Convert virtual address to physical address
		[[nodiscard]]
		static auto	translate(const igros_pointer_t addr) noexcept -> igros_pointer_t;

		// Page Fault Exception handler
		[[noreturn]]
		static void	exHandler(const register_t* regs) noexcept;

		// Set page directory
		static void	flush(const directory_t* const dir) noexcept;


	};


}	// namespace igros::i386

