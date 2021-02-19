////////////////////////////////////////////////////////////////
//
//	Multiboot 1 header info
//
//	File:	multiboot.hpp
//	Date:	12 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>

#include <flags.hpp>


// Multiboot code zone
namespace igros::multiboot {


	// Multiboot 1 header magic
	constexpr static auto HEADER_MAGIC	= 0x1BADB002;
	// Multiboot 1 bootloader magic
	constexpr static auto BOOTLOADER_MAGIC	= 0x2BADB002;


	// Multiboot 1 header signature check function
	inline static bool check(const dword_t signature) noexcept {
		return (BOOTLOADER_MAGIC == signature);
	}


	// Multiboot header flags enumeration
	enum class flags_t : dword_t {
		MEM		= (1U << 0),			// Memory info available
		BOOT_DEV	= (1U << 1),			// Boot device info available
		CMD		= (1U << 2),			// Kernel command line available
		MODULES		= (1U << 3),			// Kernel modules available
		SYMS_AOUT	= (1U << 4),			// A.OUT info available
		SYMS_ELF	= (1U << 5),			// ELF info available
		MEM_MAP		= (1U << 6),			// Memory map available
		DRIVES		= (1U << 7),			// Drives info available
		TABLE_CONFIG	= (1U << 8),			// Configuration table available
		LOADER_NAME	= (1U << 9),			// Bootloader name available
		TABLE_APM	= (1U << 10),			// APM table available
		VBE		= (1U << 11),			// VBE table available
		FRAME_BUF	= (1U << 12)			// Frame buffer info available
	};


#pragma pack(push, 1)

	// Kernel sections info
	struct infoSections final {
		dword_t		header;				// Kernel header section address
		dword_t		loadStart;			// Kernel load address start
		dword_t		loadEnd;			// Kernel load address end
		dword_t		bssEnd;				// Kernel BSS section end
		dword_t		entry;				// Kernel entry point
	};

	// Multiboot video info
	struct infoVideo final {
		dword_t		mode;				// Video mode
		dword_t		width;				// Video mode width
		dword_t		height;				// Video mode height
		dword_t		depth;				// Video mode depth
	};

	// Multiboot 1 header
	struct header_t final {
		dword_t		magic;				// Multiboot header magic - must be equal to HEADER_MAGIC
		dword_t		flags;				// Multiboot header flag
		dword_t		checksum;			// Multiboot header checksum
		infoSections	sections;			// Executable sections data
		infoVideo	video;				// Video mode data
	};


	// Multiboot 1 information from bootloader
	struct info_t final {

		dword_t		flags;				// Multiboot present features flag

		dword_t		memLow;				// Multiboot bios memory low info
		dword_t		memHigh;			// Multiboot bios memory high info

		dword_t		bootDevice;			// Multiboot boot device

		dword_t		cmdLine;			// Multiboot bootloader command line

		dword_t		modulesCount;			// Multiboot kernel modules count
		dword_t		modulesAddr;			// Multiboot kernel modules address

		dword_t		syms[4ULL];			// Multiboot kernel symbols

		dword_t		mmapLength;			// Multiboot memory map length
		dword_t		mmapAddr;			// Multiboot memory map start address

		dword_t		drivesLength;			// Multiboot drives info length
		dword_t		drivesAddr;			// Multiboot drives info start address

		dword_t		configTable;			// Multiboot config table

		dword_t		bootloaderName;			// Multiboot bootloader name

		dword_t		apmTable;			// Multiboot APM table

		dword_t		vbeControlInfo;			// Multiboot VBE control info
		dword_t		vbeModeInfo;			// Multiboot VBE mode info
		word_t		vbeModeCurrent;			// Multiboot VBE current mode
		word_t		vbeInterfaceSeg;		// Multiboot VBE interface segment
		word_t		vbeInterfaceOffset;		// Multiboot VBE interface offset
		word_t		vbeInterfaceLen;		// Multiboot VBE interface length

		quad_t		fbAddress;			// Multiboot FB address
		dword_t		fbPitch;			// Multiboot FB pitch
		dword_t		fbWidth;			// Multiboot FB width
		dword_t		fbHeight;			// Multiboot FB height
		byte_t		fbBpp;				// Multiboot FB bpp
		byte_t		fbType;				// Multiboot FB type
		byte_t		fbColorInfo[6ULL];		// Multiboot FB color info


		// Multiboot contains valid memory info
		[[nodiscard]]
		bool	hasInfoMemory() const noexcept;
		// Multiboot contains valid boot device info
		[[nodiscard]]
		bool	hasInfoBootDevice() const noexcept;
		// Multiboot contains valid kernel command line info
		[[nodiscard]]
		bool	hasInfoCommandLine() const noexcept;
		// Multiboot contains valid kernel modules info
		[[nodiscard]]
		bool	hasInfoModules() const noexcept;
		// Multiboot contains valid A.OUT sections info
		[[nodiscard]]
		bool	hasInfoAOUT() const noexcept;
		// Multiboot contains valid ELF sections info
		[[nodiscard]]
		bool	hasInfoELF() const noexcept;
		// Multiboot contains valid memory map info
		[[nodiscard]]
		bool	hasInfoMemoryMap() const noexcept;
		// Multiboot contains valid drives info
		[[nodiscard]]
		bool	hasInfoDrives() const noexcept;
		// Multiboot contains valid config table info
		[[nodiscard]]
		bool	hasInfoConfig() const noexcept;
		// Multiboot contains valid bootloader name info
		[[nodiscard]]
		bool	hasInfoBootloaderName() const noexcept;
		// Multiboot contains valid APM table info
		[[nodiscard]]
		bool	hasInfoAPM() const noexcept;
		// Multiboot contains valid VBE info
		[[nodiscard]]
		bool	hasInfoVBE() const noexcept;
		// Multiboot contains valid FrameBuffer info
		[[nodiscard]]
		bool	hasInfoFrameBuffer() const noexcept;

		// Get multiboot command line
		[[nodiscard]]
		const sbyte_t*	commandLine() const noexcept;
		// Get multiboot bootloader name
		[[nodiscard]]
		const sbyte_t*	loaderName() const noexcept;

		// Print multiboot flags
		void	printFlags() const noexcept;
		// Print multiboot memory info
		void	printMemInfo() const noexcept;
		// Print multiboot memory map
		void	printMemMap() const noexcept;
		// Print multiboot VBE info
		void	printVBEInfo() const noexcept;
		// Print multiboot FB info
		void	printFBInfo() const noexcept;


	};

	// Multiboot contains valid memory info
	[[nodiscard]]
	inline bool info_t::hasInfoMemory() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::MEM);
	}

	// Multiboot contains valid boot device info
	[[nodiscard]]
	inline bool info_t::hasInfoBootDevice() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::BOOT_DEV);
	}

	// Multiboot contains valid command line info
	[[nodiscard]]
	inline bool info_t::hasInfoCommandLine() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::CMD);
	}

	// Multiboot contains valid kernel modules info
	[[nodiscard]]
	inline bool info_t::hasInfoModules() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::MODULES);
	}

	// Multiboot contains valid A.OUT sections info
	[[nodiscard]]
	inline bool info_t::hasInfoAOUT() const noexcept {
		// A.OUT but not ELF
		return	kflags<flags_t>(flags).test(flags_t::SYMS_AOUT) && !kflags<flags_t>(flags).test(flags_t::SYMS_ELF);
	}

	// Multiboot contains valid ELF sections info
	[[nodiscard]]
	inline bool info_t::hasInfoELF() const noexcept {
		// ELF but not A.OUT
		return kflags<flags_t>(flags).test(flags_t::SYMS_ELF) && !kflags<flags_t>(flags).test(flags_t::SYMS_AOUT);
	}

	// Multiboot contains valid memory map info
	[[nodiscard]]
	inline bool info_t::hasInfoMemoryMap() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::MEM_MAP);
	}

	// Multiboot contains valid drives info
	[[nodiscard]]
	inline bool info_t::hasInfoDrives() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::DRIVES);
	}

	// Multiboot contains valid config table info
	[[nodiscard]]
	inline bool info_t::hasInfoConfig() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::TABLE_CONFIG);
	}

	// Multiboot contains valid bootloader name info
	[[nodiscard]]
	inline bool info_t::hasInfoBootloaderName() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::LOADER_NAME);
	}

	// Multiboot contains valid APM table info
	[[nodiscard]]
	inline bool info_t::hasInfoAPM() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::TABLE_APM);
	}

	// Multiboot contains valid VBE info
	[[nodiscard]]
	inline bool info_t::hasInfoVBE() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::VBE);
	}

	// Multiboot contains valid FrameBuffer info
	[[nodiscard]]
	inline bool info_t::hasInfoFrameBuffer() const noexcept {
		return kflags<flags_t>(flags).test(flags_t::FRAME_BUF);
	}


	// Get multiboot command line
	[[nodiscard]]
	inline const sbyte_t* info_t::commandLine() const noexcept {
		return (hasInfoCommandLine()) ? reinterpret_cast<const sbyte_t* const>(cmdLine) : nullptr;
	}

	// Get multiboot bootloader name
	[[nodiscard]]
	inline const sbyte_t* info_t::loaderName() const noexcept {
		return (hasInfoBootloaderName()) ? reinterpret_cast<const sbyte_t* const>(bootloaderName) : nullptr;
	}


	// Multiboot 1 memory entry type
	enum class MEMORY_MAP_TYPE : dword_t {
		AVAILABLE	= 1U,			// Memory available
		RESERVED	= 2U,			// Memory reserved
		ACPI		= 3U,			// Memory is ACPI reclaimable
		NVS		= 4U,			// Memory is NVS
		BAD		= 5U			// BAD memory
	};

	// Multiboot 1 memory map entry
	struct memoryMapEntry final {
		dword_t		size;			// Memory entry size
		quad_t		address;		// Memory entry address
		quad_t		length;			// Memory entry length
		MEMORY_MAP_TYPE	type;			// Memory entry type
	};


	// VBE config
	struct vbeConfig final {
		sbyte_t		signature[4ULL];	// VBE signature ("VESA")
		word_t		version;		// VBE version (e.g. 0x0300 = 3.0)
		dword_t		oem;			// OEM string
		dword_t		caps;			// VBE capabilities
		dword_t		modes;			// VBE modes pointer
		word_t		memory;			// VBE video memory size in 64 Kb. blocks
		word_t		rev;			// VBE revision string
		dword_t		vendor;			// VBE vendor string
		dword_t		productName;		// VBE product name string
		dword_t		productRev;		// VBE product revision string
		byte_t		reserved[222ULL];	// VBE reserved
		byte_t		oemData[256ULL];	// VBE OEM data
	};

	// VBE mode
	struct vbeMode final {
		word_t		attributes;
		byte_t		windowA;
		byte_t		windowB;
		word_t		granularity;
		word_t		windowSize;
		word_t		segmentA;
		word_t		segmentB;
		dword_t		funcPtr;
		word_t		pitch;
		word_t		width;
		word_t		height;
		byte_t		charW;
		byte_t		charH;
		byte_t		planes;
		byte_t		bpp;
		byte_t		banks;
		byte_t		memoryModel;
		byte_t		bankSize;
		byte_t		imagePages;
		byte_t		reserved0;
		byte_t		redMask;
		byte_t		redPosition;
		byte_t		greenMask;
		byte_t		greenPosition;
		byte_t		blueMask;
		byte_t		bluePosition;
		byte_t		reservedMask;
		byte_t		reservedPosition;
		byte_t		directColorAttributes;
		dword_t		physbase;
		dword_t		reserved1;
		word_t		reserved2;
	};

#pragma pack(pop)


	// Test multiboot
	void test(const info_t* const multiboot, const dword_t magic) noexcept;


}	// namespace igros::multiboot

