////////////////////////////////////////////////////////////////
//
//	Multiboot 1 header info
//
//	File:	multiboot.hpp
//	Date:	21 Jun 2019
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
	enum flags : dword_t {

		MEM		= (1 << 0),			//
		BOOT_DEV	= (1 << 1),			//
		CMD		= (1 << 2),			//
		MODULES		= (1 << 3),			//
		SYMS_AOUT	= (1 << 4),			//
		SYMS_ELF	= (1 << 5),			//
		MEM_MAP		= (1 << 6),			//
		DRIVES		= (1 << 7),			//
		TABLE_CONFIG	= (1 << 8),			//
		LOADER_NAME	= (1 << 9),			//
		TABLE_APM	= (1 << 10),			//
		VBE		= (1 << 11),			//
		FRAME_BUF	= (1 << 12)			//

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
	struct header {

		dword_t		magic;				// Multiboot header magic - must be equal to HEADER_MAGIC
		dword_t		flags;				// Multiboot header flag
		dword_t		checksum;			// Multiboot header checksum
		infoSections	sections;			// Executable sections data
		infoVideo	video;				// Video mode data

	};


	// Multiboot 1 information from bootloader
	struct info {

		dword_t		flags;				// Multiboot present features flag

		dword_t		memLow;				// Multiboot low memory bios info
		dword_t		memHigh;			// Multiboot high memory bios info

		dword_t		bootDevice;			// Multiboot boot device

		dword_t		cmdLine;			// Multiboot kernel command line

		dword_t		modulesCount;			// Multiboot kernel modules count
		dword_t		modulesAddr;			// Multiboot kernel modules address

		dword_t		syms[4];

		dword_t		mmapLength;
		dword_t		mmapAddr;

		dword_t		drivesLength;
		dword_t		drivesAddr;

		dword_t		configTable;

		dword_t		bootloaderName;


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
	bool info::hasInfoMemory() const {

		return flags & flags::MEM;

	}

	// Multiboot contains valid boot device info
	bool info::hasInfoBootDevice() const {

		return flags & flags::BOOT_DEV;

	}

	// Multiboot contains valid command line info
	bool info::hasInfoCommandLine() const {

		return flags & flags::CMD;

	}

	// Multiboot contains valid kernel modules info
	bool info::hasInfoModules() const {

		return flags & flags::MODULES;

	}

	// Multiboot contains valid A.OUT sections info
	bool info::hasInfoAOUT() const {

		// A.OUT but not ELF
		return (flags & flags::SYMS_AOUT) && !(flags & flags::SYMS_ELF);

	}

	// Multiboot contains valid ELF sections info
	bool info::hasInfoELF() const {

		// ELF but not A.OUT
		return (flags & flags::SYMS_ELF) && !(flags & flags::SYMS_AOUT);

	}

	// Multiboot contains valid memory map info
	bool info::hasInfoMemoryMap() const {

		return flags & flags::MEM_MAP;

	}

	// Multiboot contains valid drives info
	bool info::hasInfoDrives() const {

		return flags & flags::DRIVES;

	}

	// Multiboot contains valid config table info
	bool info::hasInfoConfig() const {

		return flags & flags::TABLE_CONFIG;

	}

	// Multiboot contains valid bootloader name info
	bool info::hasInfoBootloaderName() const {

		return flags & flags::LOADER_NAME;

	}

	// Multiboot contains valid APM table info
	bool info::hasInfoAPM() const {

		return flags & flags::TABLE_APM;

	}

	// Multiboot contains valid VBE info
	bool info::hasInfoVBE() const {

		return flags & flags::VBE;

	}

	// Multiboot contains valid FrameBuffer info
	bool info::hasInfoFrameBuffer() const {

		return flags & flags::FRAME_BUF;

	}


	// Get multiboot command line
	const sbyte_t* info::commandLine() const {

		return (flags & flags::CMD) ? reinterpret_cast<const sbyte_t*>(cmdLine) : nullptr;

	}

	// Get multiboot bootloader name
	const sbyte_t* info::loaderName() const {

		return (flags & flags::LOADER_NAME) ? reinterpret_cast<const sbyte_t*>(bootloaderName) : nullptr;

	}


	// Multiboot 1 memory entry type
	enum class mmapType_t : dword_t {

		AVAILABLE	= 1,			// Memory available
		RESERVED	= 2,			// Memory reserved
		ACPI		= 3,			// Memory is ACPI reclaimable
		NVS		= 4,			// Memory is NVS
		BAD		= 5			// BAD memory

	};

	// Multiboot 1 memory map entry
	struct mmapEntry_t {

		dword_t		size;			// Memory entry size
		quad_t		address;		// Memory entry address
		quad_t		length;			// Memory entry length
		mmapType_t	type;			// Memory entry type

	};

#pragma pack(pop)


	// Multiboot 1 header signature check function
	inline constexpr bool check(const dword_t signature) {

		return (BOOTLOADER_MAGIC == signature);

	}


}	// namespace multiboot


#endif	// IGROS_ARCH_MULTIBOOT_1_HPP

