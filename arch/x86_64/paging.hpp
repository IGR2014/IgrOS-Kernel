////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kflags.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Forward declaration
	struct register_t;


	// Page Map Level 4 max entries count
	constexpr auto	PAGE_MAP_LEVEL_4_SIZE		{512_usize};
	// Page Directory Pointer max entries count
	constexpr auto	PAGE_DIRECTORY_POINTR_SIZE	{PAGE_MAP_LEVEL_4_SIZE};
	// Page Directory max entries count
	constexpr auto	PAGE_DIRECTORY_SIZE		{PAGE_MAP_LEVEL_4_SIZE};
	// Page Table max entries count
	constexpr auto	PAGE_TABLE_SIZE			{PAGE_MAP_LEVEL_4_SIZE};
	// Page shift
	constexpr auto	PAGE_SHIFT			{12_usize};
	// Page size
	constexpr auto	PAGE_SIZE			{1_usize << PAGE_SHIFT};
	// Page mask
	constexpr auto	PAGE_MASK			{PAGE_SIZE - 1_usize};


#pragma push(pack, 1)


	// Page
	union alignas(4096_usize) page_t {
		igros_pointer_t		next;					// Pointer to next page
		igros_byte_t		bytes[PAGE_SIZE];			// Page raw bytes
	};


	// Page table
	union alignas(4096_usize) table_t {
		igros_pointer_t		next;					// Pointer to next table
		page_t*			pages[PAGE_TABLE_SIZE];			// Page table entries
	};


	// Page directory
	union alignas(4096_usize) directory_t {
		igros_pointer_t		next;					// Pointer to next directory
		table_t*		tables[PAGE_DIRECTORY_SIZE];		// Page directory entries
	};


	// Page directory pointer
	union alignas(4096_usize) directory_pointer_t {
		igros_pointer_t		next;					// Pointer to next directory pointer
		directory_t*		directories[PAGE_DIRECTORY_SIZE];	// Page directory entries
	};


	// Page Map Level 4
	union alignas(4096_usize) pml4_t {
		directory_pointer_t*	pointers[PAGE_DIRECTORY_SIZE];		// Page Map Level 4 entries
	};


#pragma pop(pack)


	// Paging structure
	class paging final {


		static table_t*		mFreePages;				// Free pages list

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
		enum class FLAGS : igros_quad_t {
			CLEAR			= 0x0000000000000000_u64,
			PRESENT			= 0x0000000000000001_u64,
			WRITABLE		= 0x0000000000000002_u64,
			USER_ACCESSIBLE		= 0x0000000000000004_u64,
			WRITE_THROUGH		= 0x0000000000000008_u64,
			NON_CACHED		= 0x0000000000000010_u64,
			ACCESSED		= 0x0000000000000020_u64,
			DIRTY			= 0x0000000000000040_u64,
			HUGE			= 0x0000000000000080_u64,
			GLOBAL			= 0x0000000000000100_u64,
			USER_DEFINED		= 0x0000000000000E00_u64,
			NON_EXECUTABLE		= 0x8000000000000000_u64,
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

		// Initialize paging heap
		static void	heap(const igros_pointer_t phys, const igros_usize_t size) noexcept;

		// Allocate page
		[[nodiscard]]
		static auto	allocate() noexcept -> igros_pointer_t;
		// Deallocate page
		static void	deallocate(const igros_pointer_t page) noexcept;

		// Make PML4
		[[nodiscard]]
		static auto	makePML4() noexcept -> pml4_t*;
		// Make page directory pointer
		[[nodiscard]]
		static auto	makeDirectoryPointer() noexcept -> directory_pointer_t*;
		// Make page directory
		[[nodiscard]]
		static auto	makeDirectory() noexcept -> directory_t*;
		// Make page table
		[[nodiscard]]
		static auto	makeTable() noexcept -> table_t*;

		// Check directory pointer flags
		[[nodiscard]]
		static auto	checkFlags(const directory_pointer_t* dirPtr, const kflags<FLAGS> flags) noexcept -> bool;
		// Check directory flags
		[[nodiscard]]
		static auto	checkFlags(const directory_t* dir, const kflags<FLAGS> flags) noexcept -> bool;
		// Check table flags
		[[nodiscard]]
		static auto	checkFlags(const table_t* table, const kflags<FLAGS> flags) noexcept -> bool;
		// Check page flags
		[[nodiscard]]
		static auto	checkFlags(const page_t* page, const kflags<FLAGS> flags) noexcept -> bool;

		// Map virtual page to physical page (whole pml4, explicit pml4)
		static void	mapPML4(pml4_t* const  pml4, const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (whole pml4)
		static void	mapPML4(const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single directory pointer, explicit pml4)
		static void	mapDirectoryPointer(pml4_t* const pml4, const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single directory pointer)
		static void	mapDirectoryPointer(const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single directory, explicit pml4)
		static void	mapDirectory(pml4_t* const pml4, const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single directory)
		static void	mapDirectory(const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single table, explicit pml4)
		static void	mapTable(pml4_t* const pml4, const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single table)
		static void	mapTable(const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single page, explicit page directory)
		static void	mapPage(pml4_t* const dir, const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single page)
		static void	mapPage(const page_t* phys, const igros_pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Convert virtual address to physical address
		[[nodiscard]]
		static auto	translate(const igros_pointer_t addr) noexcept -> igros_pointer_t;

		// Page Fault Exception handler
		static void	exHandler(const register_t* regs) noexcept;

		// Set page directory
		static void	flush(const pml4_t* const dir) noexcept;


	};


}	// namespace igros::x86_64

