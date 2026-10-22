////////////////////////////////////////////////////////////////
//
//	Paging (i386 2-level and x86_64 4-level page tables)
//
//	File:	paging.hpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch x86
#include <arch/x86/native.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>
// IgrOS-Kernel memory
#include <mem/phys.hpp>


// x86 namespace
namespace igros::x86 {


	// Paging
	// Physical memory is mapped at kernel virtual offset (direct map): first 4 MB
	// by boot code, up to top of RAM (DIRECT_MAP_LIMIT at most) once init built
	// kernel page tables. Page tables are accessed through the direct map; table
	// entries hold physical addresses.
	class paging final {

		// Copy c-tor
		paging(const paging &other) = delete;
		// Copy assignment
		auto	operator=(const paging &other) -> paging& = delete;


	public:

		// Page table entry (register-wide)
		using entry_t	= igros_usize_t;

		// Page table entry flags
		enum class FLAGS : entry_t {
			PRESENT		= 0x001,			// Entry is valid
			WRITABLE	= 0x002,			// Write access
			USER		= 0x004,			// User mode access
			WRITE_THROUGH	= 0x008,			// Write-through caching
			NON_CACHED	= 0x010,			// Caching disabled
			ACCESSED	= 0x020,			// Set by CPU on access
			DIRTY		= 0x040,			// Set by CPU on write (pages)
			HUGE		= 0x080,			// Large page (directory levels)
			GLOBAL		= 0x100,			// Not flushed on CR3 change
#if	defined (IGROS_ARCH_x86_64)
			NO_EXECUTE	= 0x8000000000000000		// Instruction fetch not allowed (EFER.NXE)
#else
			NO_EXECUTE	= 0x000				// No NX bit without PAE: ignored
#endif
		};

		// Page size
		constexpr static auto	PAGE_SHIFT	{mem::PAGE_SHIFT};
		constexpr static auto	PAGE_SIZE	{mem::PAGE_SIZE};

#if	defined (IGROS_ARCH_i386)

		// Page directory -> page table (4 KB pages, no PAE)
		constexpr static auto	LEVELS		{2_usize};
		constexpr static auto	INDEX_BITS	{10_usize};
		// Physical address bits of entry
		constexpr static auto	ADDRESS_MASK	{entry_t {0xFFFFF000}};
		// Physical memory mapped at kernel offset (3 GB .. 3.75 GB), rest is for dynamic mappings
		constexpr static auto	DIRECT_MAP_LIMIT	{mem::phys_t {0x30000000}};

#elif	defined (IGROS_ARCH_x86_64)

		// PML4 -> PDPT -> page directory -> page table
		constexpr static auto	LEVELS		{4_usize};
		constexpr static auto	INDEX_BITS	{9_usize};
		// Physical address bits of entry (52-bit physical addresses)
		constexpr static auto	ADDRESS_MASK	{entry_t {0x000FFFFFFFFFF000}};
		// Physical memory mapped at kernel offset (-2 GB .. -1 GB), rest is for dynamic mappings
		constexpr static auto	DIRECT_MAP_LIMIT	{mem::phys_t {0x40000000}};

#endif

		// Entries per table
		constexpr static auto	ENTRIES		{1_usize << INDEX_BITS};
		// Large page size (4 MB on i386, 2 MB on x86_64)
		constexpr static auto	LARGE_PAGE_SIZE	{PAGE_SIZE << INDEX_BITS};

		// Address isn't mapped
		constexpr static auto	INVALID		{~mem::phys_t {0}};


		// Build kernel page tables and switch to them:
		// - first 4 MB at kernel offset with kernel section rights (code: read + execute,
		//   read-only data: read, everything else: read + write, no execute),
		// - first 4 MB identity mapped (boot data), except null page, no execute,
		// - rest of RAM at kernel offset with large pages (direct map), no execute;
		// enables NX (if supported) and write protection in kernel mode (CR0.WP)
		static void	init() noexcept;

		// Map 4 KB page (false if structures can't be allocated or address is in a large page)
		[[nodiscard]]
		static auto	map(const mem::phys_t phys, const void* const virt, const klib::kFlags<FLAGS> flags) noexcept -> bool;
		// Unmap 4 KB page
		static void	unmap(const void* const virt) noexcept;

		// Physical address of virtual address (INVALID if not mapped)
		[[nodiscard]]
		static auto	translate(const void* const virt) noexcept -> mem::phys_t;

		// Current top level table physical address
		[[nodiscard]]
		static auto	root() noexcept -> mem::phys_t;

		// End of physical memory reachable through direct map
		[[nodiscard]]
		static auto	directMapEnd() noexcept -> mem::phys_t;
		// Start of kernel dynamic mappings (right above direct map)
		[[nodiscard]]
		static auto	dynamicStart() noexcept -> igros_byte_t*;

		// Page fault handler
		[[noreturn]]
		static void	exHandler(const register_t* regs) noexcept;


	private:

		// Table at physical address (through direct map)
		[[nodiscard]]
		static auto	table(const mem::phys_t phys) noexcept -> entry_t*;
		// Allocate empty table (reachable through direct map, 0 if no memory)
		[[nodiscard]]
		static auto	allocTable() noexcept -> mem::phys_t;
		// Index of virtual address in table of given level (0 - page table)
		[[nodiscard]]
		static auto	index(const void* const virt, const igros_usize_t level) noexcept -> igros_usize_t;
		// Entry of virtual address at given level (0 - page table) of given hierarchy
		// (create - allocate missing tables)
		[[nodiscard]]
		static auto	entry(const mem::phys_t top, const void* const virt, const bool create, const bool user, const igros_usize_t level = 0_usize) noexcept -> entry_t*;
		// Entry value for physical address with flags (drops NX if not enabled)
		[[nodiscard]]
		static auto	makeEntry(const mem::phys_t phys, const klib::kFlags<FLAGS> flags) noexcept -> entry_t;
		// Map 4 KB page in given hierarchy
		[[nodiscard]]
		static auto	map(const mem::phys_t top, const mem::phys_t phys, const void* const virt, const klib::kFlags<FLAGS> flags) noexcept -> bool;
		// Map large page in given hierarchy (fresh directory entry only)
		[[nodiscard]]
		static auto	mapLarge(const mem::phys_t top, const mem::phys_t phys, const void* const virt, const klib::kFlags<FLAGS> flags) noexcept -> bool;
		// Rights of kernel offset mapping of low memory physical page (kernel sections)
		[[nodiscard]]
		static auto	kernelFlags(const mem::phys_t phys) noexcept -> klib::kFlags<FLAGS>;

		// Check mapping on a scratch page
		[[nodiscard]]
		static auto	selfTest() noexcept -> bool;


	};


}	// namespace igros::x86

