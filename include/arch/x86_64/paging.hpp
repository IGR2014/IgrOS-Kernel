////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	07 Jul 2020
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


	// Page Map Level 4 max entries count
	constexpr auto	PAGE_MAP_LEVEL_4_SIZE		= 512ull;
	// Page Directory Pointer max entries count
	constexpr auto	PAGE_DIRECTORY_POINTR_SIZE	= PAGE_MAP_LEVEL_4_SIZE;
	// Page Directory max entries count
	constexpr auto	PAGE_DIRECTORY_SIZE		= PAGE_MAP_LEVEL_4_SIZE;
	// Page Table max entries count
	constexpr auto	PAGE_TABLE_SIZE			= PAGE_MAP_LEVEL_4_SIZE;
	// Page shift
	constexpr auto	PAGE_SHIFT			= 21;
	// Page size
	constexpr auto	PAGE_SIZE			= 1u << PAGE_SHIFT;
	// Page mask
	constexpr auto	PAGE_MASK			= PAGE_SIZE - 1u;


#pragma push(pack, 1)


	// Page
	union alignas(4096ull) page_t {
		pointer_t		next;					// Pointer to next page
		byte_t			bytes[PAGE_SIZE];			// Page raw bytes
	};


	// Page table
	union alignas(4096ull) table_t {
		pointer_t		next;					// Pointer to next table
		page_t*			pages[PAGE_TABLE_SIZE];			// Page table entries
	};


	// Page directory
	union alignas(4096ull) directory_t {
		pointer_t		next;					// Pointer to next directory
		table_t*		tables[PAGE_DIRECTORY_SIZE];		// Page directory entries
	};


	// Page directory pointer
	union alignas(4096ull) directoryPointer_t {
		pointer_t		next;					// Pointer to next directory pointer
		directory_t*		directories[PAGE_DIRECTORY_SIZE];	// Page directory entries
	};


	// Page Map Level 4
	union alignas(4096ull) pml4_t {
		directoryPointer_t*	pointers[PAGE_DIRECTORY_SIZE];		// Page Map Level 4 entries
	};


#pragma pop(pack)


	// Paging structure
	class paging final {


		static table_t*	mFreePages;		// Free pages list


	public:

		// Page flags
		enum class flags_t : quad_t {
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

		// Copy c-tor
		paging(const paging &other) = delete;
		// Copy assignment
		paging& operator=(const paging &other) = delete;

		// Move c-tor
		paging(paging &&other) = delete;
		// Move assignment
		paging& operator=(paging &&other) = delete;

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
		[[nodiscard]] static pointer_t	allocate() noexcept;
		// Deallocate page
		static void			deallocate(const pointer_t page) noexcept;

		// Map virtual page to physical page
		static void map(pml4_t* pml4, const page_t* phys, const pointer_t virt, const flags_t flags) noexcept;
		// Map virtual page to physical page
		static void map(const page_t* phys, const pointer_t virt, const flags_t flags) noexcept;

		// Convert virtual address to physical address
		[[nodiscard]] static pointer_t	toPhys(const pointer_t addr) noexcept;

		// Page Fault Exception handler
		static void exHandler(const taskRegs_t* regs) noexcept;

		// Set page directory
		static void setDirectory(const pml4_t* dir) noexcept;


	};


}	// namespace igros::arch

