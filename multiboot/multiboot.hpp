////////////////////////////////////////////////////////////////
//
//	Multiboot 1 header info
//
//	File:	multiboot.hpp
//	Date:	16 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <bit>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>


// Multiboot code zone
namespace igros::multiboot {


	// Multiboot 1 header magic
	constexpr static auto HEADER_MAGIC	{0x1BADB002_u32};
	// Multiboot 1 bootloader magic
	constexpr static auto BOOTLOADER_MAGIC	{0x2BADB002_u32};


	// Multiboot 1 header signature check function
	[[nodiscard]]
	inline static auto check(const igros_dword_t signature) noexcept -> bool {
		return (BOOTLOADER_MAGIC == signature);
	}


	// Multiboot header flags enumeration
	enum class flags_t : igros_dword_t {
		MEM		= (1_u32 << 0),			// Memory info available
		BOOT_DEV	= (1_u32 << 1),			// Boot device info available
		CMD		= (1_u32 << 2),			// Kernel command line available
		MODULES		= (1_u32 << 3),			// Kernel modules available
		SYMS_AOUT	= (1_u32 << 4),			// A.OUT info available
		SYMS_ELF	= (1_u32 << 5),			// ELF info available
		MEM_MAP		= (1_u32 << 6),			// Memory map available
		DRIVES		= (1_u32 << 7),			// Drives info available
		TABLE_CONFIG	= (1_u32 << 8),			// Configuration table available
		LOADER_NAME	= (1_u32 << 9),			// Bootloader name available
		TABLE_APM	= (1_u32 << 10),		// APM table available
		VBE		= (1_u32 << 11),		// VBE table available
		FRAME_BUF	= (1_u32 << 12)			// Frame buffer info available
	};


#pragma pack(push, 1)

	// Kernel sections info
	struct infoSections final {
		igros_dword_t		header;				// Kernel header section address
		igros_dword_t		loadStart;			// Kernel load address start
		igros_dword_t		loadEnd;			// Kernel load address end
		igros_dword_t		bssEnd;				// Kernel BSS section end
		igros_dword_t		entry;				// Kernel entry point
	};

	// Multiboot video info
	struct infoVideo final {
		igros_dword_t		mode;				// Video mode
		igros_dword_t		width;				// Video mode width
		igros_dword_t		height;				// Video mode height
		igros_dword_t		depth;				// Video mode depth
	};

	// Multiboot 1 header
	struct header_t final {
		igros_dword_t		magic;				// Multiboot header magic - must be equal to HEADER_MAGIC
		igros_dword_t		flags;				// Multiboot header flag
		igros_dword_t		checksum;			// Multiboot header checksum
		infoSections		sections;			// Executable sections data
		infoVideo		video;				// Video mode data
	};


	// Multiboot 1 information from bootloader
	struct info_t final {

		klib::kFlags<flags_t>	flags;				// Multiboot present features flag

		igros_dword_t		memLow;				// Multiboot bios memory low info
		igros_dword_t		memHigh;			// Multiboot bios memory high info

		igros_dword_t		bootDevice;			// Multiboot boot device

		igros_dword_t		cmdLine;			// Multiboot bootloader command line

		igros_dword_t		modulesCount;			// Multiboot kernel modules count
		igros_dword_t		modulesAddr;			// Multiboot kernel modules address

		igros_dword_t		syms[4_usize];			// Multiboot kernel symbols

		igros_dword_t		mmapLength;			// Multiboot memory map length
		igros_dword_t		mmapAddr;			// Multiboot memory map start address

		igros_dword_t		drivesLength;			// Multiboot drives info length
		igros_dword_t		drivesAddr;			// Multiboot drives info start address

		igros_dword_t		configTable;			// Multiboot config table

		igros_dword_t		bootloaderName;			// Multiboot bootloader name

		igros_dword_t		apmTable;			// Multiboot APM table

		igros_dword_t		vbeControlInfo;			// Multiboot VBE control info
		igros_dword_t		vbeModeInfo;			// Multiboot VBE mode info
		igros_word_t		vbeModeCurrent;			// Multiboot VBE current mode
		igros_word_t		vbeInterfaceSeg;		// Multiboot VBE interface segment
		igros_word_t		vbeInterfaceOffset;		// Multiboot VBE interface offset
		igros_word_t		vbeInterfaceLen;		// Multiboot VBE interface length

		igros_quad_t		fbAddress;			// Multiboot FB address
		igros_dword_t		fbPitch;			// Multiboot FB pitch
		igros_dword_t		fbWidth;			// Multiboot FB width
		igros_dword_t		fbHeight;			// Multiboot FB height
		igros_byte_t		fbBpp;				// Multiboot FB bpp
		igros_byte_t		fbType;				// Multiboot FB type
		igros_byte_t		fbColorInfo[6_usize];		// Multiboot FB color info


		// Test multiboot
		void	test(const igros_dword_t magic) const noexcept;

		// Multiboot contains valid memory info
		[[nodiscard]]
		auto	hasInfoMemory() const noexcept -> bool;
		// Multiboot contains valid boot device info
		[[nodiscard]]
		auto	hasInfoBootDevice() const noexcept -> bool;
		// Multiboot contains valid kernel command line info
		[[nodiscard]]
		auto	hasInfoCommandLine() const noexcept -> bool;
		// Multiboot contains valid kernel modules info
		[[nodiscard]]
		auto	hasInfoModules() const noexcept -> bool;
		// Multiboot contains valid A.OUT sections info
		[[nodiscard]]
		auto	hasInfoAOUT() const noexcept -> bool;
		// Multiboot contains valid ELF sections info
		[[nodiscard]]
		auto	hasInfoELF() const noexcept -> bool;
		// Multiboot contains valid memory map info
		[[nodiscard]]
		auto	hasInfoMemoryMap() const noexcept -> bool;
		// Multiboot contains valid drives info
		[[nodiscard]]
		auto	hasInfoDrives() const noexcept -> bool;
		// Multiboot contains valid config table info
		[[nodiscard]]
		auto	hasInfoConfig() const noexcept -> bool;
		// Multiboot contains valid bootloader name info
		[[nodiscard]]
		auto	hasInfoBootloaderName() const noexcept -> bool;
		// Multiboot contains valid APM table info
		[[nodiscard]]
		auto	hasInfoAPM() const noexcept -> bool;
		// Multiboot contains valid VBE info
		[[nodiscard]]
		auto	hasInfoVBE() const noexcept -> bool;
		// Multiboot contains valid FrameBuffer info
		[[nodiscard]]
		auto	hasInfoFrameBuffer() const noexcept -> bool;

		// Get multiboot command line
		[[nodiscard]]
		auto	commandLine() const noexcept -> const char*;
		// Get multiboot bootloader name
		[[nodiscard]]
		auto	loaderName() const noexcept -> const char*;

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

		// Print kernel header
		void	printHeader() const noexcept;


	};

	// Multiboot contains valid memory info
	[[nodiscard]]
	inline auto info_t::hasInfoMemory() const noexcept -> bool {
		return flags.isSet(flags_t::MEM);
	}

	// Multiboot contains valid boot device info
	[[nodiscard]]
	inline auto info_t::hasInfoBootDevice() const noexcept -> bool {
		return flags.isSet(flags_t::BOOT_DEV);
	}

	// Multiboot contains valid command line info
	[[nodiscard]]
	inline auto info_t::hasInfoCommandLine() const noexcept -> bool {
		return flags.isSet(flags_t::CMD);
	}

	// Multiboot contains valid kernel modules info
	[[nodiscard]]
	inline auto info_t::hasInfoModules() const noexcept -> bool {
		return flags.isSet(flags_t::MODULES);
	}

	// Multiboot contains valid A.OUT sections info
	[[nodiscard]]
	inline auto info_t::hasInfoAOUT() const noexcept -> bool {
		// A.OUT but not ELF
		return flags.isSet(flags_t::SYMS_AOUT) && !flags.isSet(flags_t::SYMS_ELF);
	}

	// Multiboot contains valid ELF sections info
	[[nodiscard]]
	inline auto info_t::hasInfoELF() const noexcept -> bool {
		// ELF but not A.OUT
		return flags.isSet(flags_t::SYMS_ELF) && !flags.isSet(flags_t::SYMS_AOUT);
	}

	// Multiboot contains valid memory map info
	[[nodiscard]]
	inline auto info_t::hasInfoMemoryMap() const noexcept -> bool {
		return flags.isSet(flags_t::MEM_MAP);
	}

	// Multiboot contains valid drives info
	[[nodiscard]]
	inline auto info_t::hasInfoDrives() const noexcept -> bool {
		return flags.isSet(flags_t::DRIVES);
	}

	// Multiboot contains valid config table info
	[[nodiscard]]
	inline auto info_t::hasInfoConfig() const noexcept -> bool {
		return flags.isSet(flags_t::TABLE_CONFIG);
	}

	// Multiboot contains valid bootloader name info
	[[nodiscard]]
	inline auto info_t::hasInfoBootloaderName() const noexcept -> bool {
		return flags.isSet(flags_t::LOADER_NAME);
	}

	// Multiboot contains valid APM table info
	[[nodiscard]]
	inline auto info_t::hasInfoAPM() const noexcept -> bool {
		return flags.isSet(flags_t::TABLE_APM);
	}

	// Multiboot contains valid VBE info
	[[nodiscard]]
	inline auto info_t::hasInfoVBE() const noexcept -> bool {
		return flags.isSet(flags_t::VBE);
	}

	// Multiboot contains valid FrameBuffer info
	[[nodiscard]]
	inline auto info_t::hasInfoFrameBuffer() const noexcept -> bool {
		return flags.isSet(flags_t::FRAME_BUF);
	}


	// Get multiboot command line
	[[nodiscard]]
	inline auto info_t::commandLine() const noexcept -> const char* {
		return (hasInfoCommandLine()) ? std::bit_cast<const char* const>(static_cast<igros_usize_t>(cmdLine)) : nullptr;
	}

	// Get multiboot bootloader name
	[[nodiscard]]
	inline auto info_t::loaderName() const noexcept -> const char* {
		return (hasInfoBootloaderName()) ? std::bit_cast<const char* const>(static_cast<igros_usize_t>(bootloaderName)) : nullptr;
	}


	// Multiboot 1 memory entry type
	enum class MEMORY_MAP_TYPE : igros_dword_t {
		AVAILABLE		= 1_u32,			// Memory available
		RESERVED		= 2_u32,			// Memory reserved
		ACPI			= 3_u32,			// Memory is ACPI reclaimable
		NVS			= 4_u32,			// Memory is NVS
		BAD			= 5_u32				// BAD memory
	};

	// Multiboot 1 memory map entry
	struct memoryMapEntry final {
		igros_dword_t		size;				// Memory entry size
		igros_quad_t		address;			// Memory entry address
		igros_quad_t		length;				// Memory entry length
		MEMORY_MAP_TYPE		type;				// Memory entry type
	};


	// VBE config
	struct vbeConfig final {
		igros_sbyte_t		signature[4_usize];		// VBE signature ("VESA")
		igros_word_t		version;			// VBE version (e.g. 0x0300 = 3.0)
		igros_dword_t		oem;				// OEM string
		igros_dword_t		caps;				// VBE capabilities
		igros_dword_t		modes;				// VBE modes pointer
		igros_word_t		memory;				// VBE video memory size in 64 Kb. blocks
		igros_word_t		rev;				// VBE revision string
		igros_dword_t		vendor;				// VBE vendor string
		igros_dword_t		productName;			// VBE product name string
		igros_dword_t		productRev;			// VBE product revision string
		igros_byte_t		reserved[222_usize];		// VBE reserved
		igros_byte_t		oemData[256_usize];		// VBE OEM data
	};

	// VBE mode
	struct vbeMode final {
		igros_word_t		attributes;
		igros_byte_t		windowA;
		igros_byte_t		windowB;
		igros_word_t		granularity;
		igros_word_t		windowSize;
		igros_word_t		segmentA;
		igros_word_t		segmentB;
		igros_dword_t		funcPtr;
		igros_word_t		pitch;
		igros_word_t		width;
		igros_word_t		height;
		igros_byte_t		charW;
		igros_byte_t		charH;
		igros_byte_t		planes;
		igros_byte_t		bpp;
		igros_byte_t		banks;
		igros_byte_t		memoryModel;
		igros_byte_t		bankSize;
		igros_byte_t		imagePages;
		igros_byte_t		reserved0;
		igros_byte_t		redMask;
		igros_byte_t		redPosition;
		igros_byte_t		greenMask;
		igros_byte_t		greenPosition;
		igros_byte_t		blueMask;
		igros_byte_t		bluePosition;
		igros_byte_t		reservedMask;
		igros_byte_t		reservedPosition;
		igros_byte_t		directColorAttributes;
		igros_dword_t		physbase;
		igros_dword_t		reserved1;
		igros_word_t		reserved2;
	};

#pragma pack(pop)


}	// namespace igros::multiboot

