////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	10 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <flags.hpp>

#include <arch/x86_64/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Forward declaration
	struct registerx86_64_t;


	// Page Map Level 4 max entries count
	constexpr auto	PAGE_MAP_LEVEL_4_SIZE		= 512ULL;
	// Page Directory Pointer max entries count
	constexpr auto	PAGE_DIRECTORY_POINTR_SIZE	= PAGE_MAP_LEVEL_4_SIZE;
	// Page Directory max entries count
	constexpr auto	PAGE_DIRECTORY_SIZE		= PAGE_MAP_LEVEL_4_SIZE;
	// Page Table max entries count
	constexpr auto	PAGE_TABLE_SIZE			= PAGE_MAP_LEVEL_4_SIZE;
	// Page shift
	constexpr auto	PAGE_SHIFT			= 12U;
	// Page size
	constexpr auto	PAGE_SIZE			= 1ULL << PAGE_SHIFT;
	// Page mask
	constexpr auto	PAGE_MASK			= PAGE_SIZE - 1ULL;


#pragma push(pack, 1)


	// Page
	union alignas(4096ULL) page_t {
		pointer_t		next;					// Pointer to next page
		byte_t			bytes[PAGE_SIZE];			// Page raw bytes
	};


	// Page table
	union alignas(4096ULL) table_t {
		pointer_t		next;					// Pointer to next table
		page_t*			pages[PAGE_TABLE_SIZE];			// Page table entries
	};


	// Page directory
	union alignas(4096ULL) directory_t {
		pointer_t		next;					// Pointer to next directory
		table_t*		tables[PAGE_DIRECTORY_SIZE];		// Page directory entries
	};


	// Page directory pointer
	union alignas(4096ULL) directoryPointer_t {
		pointer_t		next;					// Pointer to next directory pointer
		directory_t*		directories[PAGE_DIRECTORY_SIZE];	// Page directory entries
	};


	// Page Map Level 4
	union alignas(4096ULL) pml4_t {
		directoryPointer_t*	pointers[PAGE_DIRECTORY_SIZE];		// Page Map Level 4 entries
	};


#pragma pop(pack)


	// Paging structure
	class paging final {


		static table_t*	mFreePages;		// Free pages list

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
		enum class FLAGS : quad_t {
			CLEAR			= 0x0000000000000000,
			PRESENT			= 0x0000000000000001,
			WRITABLE		= 0x0000000000000002,
			USER_ACCESSIBLE		= 0x0000000000000004,
			WRITE_THROUGH		= 0x0000000000000008,
			NON_CACHED		= 0x0000000000000010,
			ACCESSED		= 0x0000000000000020,
			DIRTY			= 0x0000000000000040,
			HUGE			= 0x0000000000000080,
			GLOBAL			= 0x0000000000000100,
			USER_DEFINED		= 0x0000000000000E00,
			NON_EXECUTABLE		= 0x8000000000000000,
			FLAGS_MASK		= PAGE_MASK,
			PHYS_ADDR_MASK		= ~PAGE_MASK
		};

		// Default c-tor
		paging() noexcept = default;

		// Identity map kernel + map higher-half + self-map page directory
		static void init() noexcept;

		// Enable paging
		static void enable() noexcept;
		// Disable paging
		static void disable() noexcept;

		// Enable Physical Address Extension
		static void enablePAE() noexcept;
		// Disable Physical Address Extension
		static void disablePAE() noexcept;

		// Initialize paging heap
		static void heap(const pointer_t phys, const std::size_t size) noexcept;

		// Allocate page
		[[nodiscard]]
		static pointer_t	allocate() noexcept;
		// Deallocate page
		static void		deallocate(const pointer_t page) noexcept;

		// Make PML4
		[[nodiscard]]
		static pml4_t*			makePML4() noexcept;
		// Make page directory pointer
		[[nodiscard]]
		static directoryPointer_t*	makeDirectoryPointer() noexcept;
		// Make page directory
		[[nodiscard]]
		static directory_t*		makeDirectory() noexcept;
		// Make page table
		[[nodiscard]]
		static table_t*			makeTable() noexcept;

		// Check directory pointer flags
		[[nodiscard]]
		static bool checkFlags(const directoryPointer_t* dirPtr, const kflags<FLAGS> flags) noexcept;
		// Check directory flags
		[[nodiscard]]
		static bool checkFlags(const directory_t* dir, const kflags<FLAGS> flags) noexcept;
		// Check table flags
		[[nodiscard]]
		static bool checkFlags(const table_t* table, const kflags<FLAGS> flags) noexcept;
		// Check page flags
		[[nodiscard]]
		static bool checkFlags(const page_t* page, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (whole pml4, explicit pml4)
		static void mapPML4(pml4_t* const  pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (whole pml4)
		static void mapPML4(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single directory pointer, explicit pml4)
		static void mapDirectoryPointer(pml4_t* const pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single directory pointer)
		static void mapDirectoryPointer(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single directory, explicit pml4)
		static void mapDirectory(pml4_t* const pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single directory)
		static void mapDirectory(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single table, explicit pml4)
		static void mapTable(pml4_t* const pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single table)
		static void mapTable(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Map virtual page to physical page (single page, explicit page directory)
		static void mapPage(pml4_t* const dir, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;
		// Map virtual page to physical page (single page)
		static void mapPage(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept;

		// Convert virtual address to physical address
		[[nodiscard]]
		static pointer_t	translate(const pointer_t addr) noexcept;

		// Page Fault Exception handler
		static void exHandler(const registerx86_64_t* regs) noexcept;

		// Set page directory
		static void flush(const pml4_t* const dir) noexcept;


	};


}	// namespace igros::x86_64

