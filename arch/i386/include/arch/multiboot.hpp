////////////////////////////////////////////////////////////////
//
//	KMultiboot header info
//
//	File:	multiboot.hpp
//	Date:	06 May 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_MULTIBOOT_1_HPP
#define IGROS_ARCH_MULTIBOOT_1_HPP


#include <arch/types.hpp>


// Multiboot code zone
namespace multiboot {


	// Multiboot 1 header magic
	static const dword_t	HEADER_MAGIC		= 0x1BADB002;
	// Multiboot 1 bootloader magic
	static const dword_t	BOOTLOADER_MAGIC	= 0x2BADB002;


#pragma pack(push, 1)

	// Multiboot 1 header
	struct header {

		dword_t		magic;			// Multiboot header magic - must be equal to HEADER_MAGIC

		dword_t		flags;			// Multiboot header flag

		dword_t		checksum;		// Multiboot header checksum

		// Executable sections data
		dword_t		sectionHeader;
		dword_t		sectionLoadStart;
		dword_t		sectionLoadEnd;
		dword_t		sectionBSSEnd;
		dword_t		sectionEntry;

		// Video mode data
		dword_t		videoMode;
		dword_t		videoWidth;
		dword_t		videoHeight;
		dword_t		videoDepth;

	};


	// Multiboot 1 information from bootloader
	struct info {

		dword_t		flags;			// Multiboot present features flag

		dword_t		memoryLow;		// Multiboot low memory bios info
		dword_t		memoryHigh;		// Multiboot high memory bios info

		dword_t		bootDevice;		// Multiboot boot device

		dword_t		commandLine;		// Multiboot kernel command line

		dword_t		modulesCount;		// Boot modules count
		dword_t		modulesAddress;		// Boot modules address

	};


	// Multiboot 1 memory entry type
	enum class mmapEntryType : dword_t {

		AVAILABLE	= 1,			// Memory available
		RESERVED	= 2,			// Memory reserved
		ACPI		= 3,			// Memory is ACPI reclaimable
		NVS		= 4,			// Memory is NVS
		BAD		= 5			// BAD memory

	};

	// Multiboot 1 memory map entry
	struct mmapEntry {

		dword_t		size;			// Memory entry size
		quad_t		address;		// Memory entry address
		quad_t		length;			// Memory entry length
		mmapEntryType	type;			// Memory entry type

	};

#pragma pack(pop)


}	// namespace arch


#endif	// IGROS_ARCH_MULTIBOOT_1_HPP


