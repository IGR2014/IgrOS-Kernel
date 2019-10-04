////////////////////////////////////////////////////////////////
//
//	Multiboot 1 header info
//
//	File:	multiboot.hpp
//	Date:	04 Oct 2019
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


	// Multiboot header flags enumeration
	enum FLAGS : dword_t {

		MEM		= (1 << 0),			// Memory info available
		BOOT_DEV	= (1 << 1),			// Boot device info available
		CMD		= (1 << 2),			// Kernel command line available
		MODULES		= (1 << 3),			// Kernel modules available
		SYMS_AOUT	= (1 << 4),			// A.OUT info available
		SYMS_ELF	= (1 << 5),			// ELF info available
		MEM_MAP		= (1 << 6),			// Memory map available
		DRIVES		= (1 << 7),			// Drives info available
		TABLE_CONFIG	= (1 << 8),			// Configuration table available
		LOADER_NAME	= (1 << 9),			// Bootloader name available
		TABLE_APM	= (1 << 10),			// APM table available
		VBE		= (1 << 11),			// VBE table available
		FRAME_BUF	= (1 << 12)			// Frame buffer info available

	};


#pragma pack(push, 1)

	// Kernel sections info
	struct infoSections {

		dword_t		header;				// Kernel header section address
		dword_t		loadStart;			// Kernel load address start
		dword_t		loadEnd;			// Kernel load address end
		dword_t		bssEnd;				// Kernel BSS section end
		dword_t		entry;				// Kernel entry point

	};

	// Multiboot video info
	struct infoVideo {

		dword_t		mode;				// Video mode
		dword_t		width;				// Video mode width
		dword_t		height;				// Video mode height
		dword_t		depth;				// Video mode depth

	};

	// Multiboot 1 header
	struct header_t {

		dword_t		magic;				// Multiboot header magic - must be equal to HEADER_MAGIC
		dword_t		flags;				// Multiboot header flag
		dword_t		checksum;			// Multiboot header checksum
		infoSections	sections;			// Executable sections data
		infoVideo	video;				// Video mode data

	};


	// Multiboot 1 information from bootloader
	struct info_t {

		dword_t		flags;				// Multiboot present features flag

		dword_t		memLow;				// Multiboot low memory bios info
		dword_t		memHigh;			// Multiboot high memory bios info

		dword_t		bootDevice;			// Multiboot boot device

		dword_t		cmdLine;			// Multiboot kernel command line

		dword_t		modulesCount;			// Multiboot kernel modules count
		dword_t		modulesAddr;			// Multiboot kernel modules address

		dword_t		syms[4];

		dword_t		mmapLength;			// Multiboot kernel memory map length
		dword_t		mmapAddr;			// Multiboot kernel memory map start address

		dword_t		drivesLength;			// Multiboot kernel drives info length
		dword_t		drivesAddr;			// Multiboot kernel drives info start address

		dword_t		configTable;			// Multiboot kernel config table

		dword_t		bootloaderName;			// Multiboot kernel bootloader name


		// Multiboot contains valid memory info
		inline bool		hasInfoMemory()		const;
		// Multiboot contains valid boot device info
		inline bool		hasInfoBootDevice()	const;
		// Multiboot contains valid kernel command line info
		inline bool		hasInfoCommandLine()	const;
		// Multiboot contains valid kernel modules info
		inline bool		hasInfoModules()	const;
		// Multiboot contains valid A.OUT sections info
		inline bool		hasInfoAOUT()		const;
		// Multiboot contains valid ELF sections info
		inline bool		hasInfoELF()		const;
		// Multiboot contains valid memory map info
		inline bool		hasInfoMemoryMap()	const;
		// Multiboot contains valid drives info
		inline bool		hasInfoDrives()		const;
		// Multiboot contains valid config table info
		inline bool		hasInfoConfig()		const;
		// Multiboot contains valid bootloader name info
		inline bool		hasInfoBootloaderName()	const;
		// Multiboot contains valid APM table info
		inline bool		hasInfoAPM()		const;
		// Multiboot contains valid VBE info
		inline bool		hasInfoVBE()		const;
		// Multiboot contains valid FrameBuffer info
		inline bool		hasInfoFrameBuffer()	const;

		// Dump multiboot flags
		void			dumpFlags()		const;

		// Get multiboot command line
		inline const sbyte_t*	commandLine()		const;
		// Get multiboot bootloader name
		inline const sbyte_t*	loaderName()		const;

	};

	// Multiboot contains valid memory info
	bool info_t::hasInfoMemory() const {
		return flags & FLAGS::MEM;
	}

	// Multiboot contains valid boot device info
	bool info_t::hasInfoBootDevice() const {
		return flags & FLAGS::BOOT_DEV;
	}

	// Multiboot contains valid command line info
	bool info_t::hasInfoCommandLine() const {
		return flags & FLAGS::CMD;
	}

	// Multiboot contains valid kernel modules info
	bool info_t::hasInfoModules() const {
		return flags & FLAGS::MODULES;
	}

	// Multiboot contains valid A.OUT sections info
	bool info_t::hasInfoAOUT() const {
		// A.OUT but not ELF
		return (flags & FLAGS::SYMS_AOUT) && !(flags & FLAGS::SYMS_ELF);
	}

	// Multiboot contains valid ELF sections info
	bool info_t::hasInfoELF() const {
		// ELF but not A.OUT
		return (flags & FLAGS::SYMS_ELF) && !(flags & FLAGS::SYMS_AOUT);
	}

	// Multiboot contains valid memory map info
	bool info_t::hasInfoMemoryMap() const {
		return flags & FLAGS::MEM_MAP;
	}

	// Multiboot contains valid drives info
	bool info_t::hasInfoDrives() const {
		return flags & FLAGS::DRIVES;
	}

	// Multiboot contains valid config table info
	bool info_t::hasInfoConfig() const {
		return flags & FLAGS::TABLE_CONFIG;
	}

	// Multiboot contains valid bootloader name info
	bool info_t::hasInfoBootloaderName() const {
		return flags & FLAGS::LOADER_NAME;
	}

	// Multiboot contains valid APM table info
	bool info_t::hasInfoAPM() const {
		return flags & FLAGS::TABLE_APM;
	}

	// Multiboot contains valid VBE info
	bool info_t::hasInfoVBE() const {
		return flags & FLAGS::VBE;
	}

	// Multiboot contains valid FrameBuffer info
	bool info_t::hasInfoFrameBuffer() const {
		return flags & FLAGS::FRAME_BUF;
	}


	// Get multiboot command line
	const sbyte_t* info_t::commandLine() const {
		return (flags & FLAGS::CMD) ? reinterpret_cast<const sbyte_t*>(cmdLine) : nullptr;
	}

	// Get multiboot bootloader name
	const sbyte_t* info_t::loaderName() const {
		return (flags & FLAGS::LOADER_NAME) ? reinterpret_cast<const sbyte_t*>(bootloaderName) : nullptr;
	}


	// Multiboot 1 memory entry type
	enum class MEMORY_MAP_TYPE : dword_t {

		AVAILABLE	= 1,			// Memory available
		RESERVED	= 2,			// Memory reserved
		ACPI		= 3,			// Memory is ACPI reclaimable
		NVS		= 4,			// Memory is NVS
		BAD		= 5			// BAD memory

	};

	// Multiboot 1 memory map entry
	struct memoryMapEntry_t {

		dword_t		size;			// Memory entry size
		quad_t		address;		// Memory entry address
		quad_t		length;			// Memory entry length
		MEMORY_MAP_TYPE	type;			// Memory entry type

	};

#pragma pack(pop)


	// Multiboot 1 header signature check function
	inline const bool check(const dword_t signature) {
		return (BOOTLOADER_MAGIC == signature);
	}


}	// namespace multiboot


#endif	// IGROS_ARCH_MULTIBOOT_1_HPP

