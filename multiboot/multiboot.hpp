////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Multiboot 1 header info
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <array>
#include <bit>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>
// IgrOS-Kernel memory
#include <mem/direct.hpp>


// Multiboot code zone
namespace igros::multiboot {


	/// @brief Multiboot 1 header magic
	constexpr static auto HEADER_MAGIC {0x1BADB002_u32};
	/// @brief Multiboot 1 bootloader magic
	constexpr static auto BOOTLOADER_MAGIC {0x2BADB002_u32};


	/// @brief Multiboot 1 header signature check function
	/// @param signature Bootloader magic
	/// @return true if kernel was loaded by multiboot bootloader
	[[nodiscard]]
	inline static auto check(const igros_dword_t signature) noexcept -> bool {
		return (BOOTLOADER_MAGIC == signature);
	}


	/// @brief Multiboot header flags enumeration
	enum class flags_t : igros_dword_t {
		MEM	     = (1_u32 << 0),  ///< Memory info available
		BOOT_DEV     = (1_u32 << 1),  ///< Boot device info available
		CMD	     = (1_u32 << 2),  ///< Kernel command line available
		MODULES	     = (1_u32 << 3),  ///< Kernel modules available
		SYMS_AOUT    = (1_u32 << 4),  ///< A.OUT info available
		SYMS_ELF     = (1_u32 << 5),  ///< ELF info available
		MEM_MAP	     = (1_u32 << 6),  ///< Memory map available
		DRIVES	     = (1_u32 << 7),  ///< Drives info available
		TABLE_CONFIG = (1_u32 << 8),  ///< Configuration table available
		LOADER_NAME  = (1_u32 << 9),  ///< Bootloader name available
		TABLE_APM    = (1_u32 << 10), ///< APM table available
		VBE	     = (1_u32 << 11), ///< VBE table available
		FRAME_BUF    = (1_u32 << 12), ///< Frame buffer info available
	};


#pragma pack(push, 1)

	/// @brief Kernel sections info
	struct infoSections final {
		igros_dword_t header;	 ///< Kernel header section address
		igros_dword_t loadStart; ///< Kernel load address start
		igros_dword_t loadEnd;	 ///< Kernel load address end
		igros_dword_t bssEnd;	 ///< Kernel BSS section end
		igros_dword_t entry;	 ///< Kernel entry point
	};

	/// @brief Multiboot video info
	struct infoVideo final {
		igros_dword_t mode;   ///< Video mode
		igros_dword_t width;  ///< Video mode width
		igros_dword_t height; ///< Video mode height
		igros_dword_t depth;  ///< Video mode depth
	};

	/// @brief Multiboot 1 header
	struct header_t final {
		igros_dword_t magic;	///< Multiboot header magic - must be equal to HEADER_MAGIC
		igros_dword_t flags;	///< Multiboot header flag
		igros_dword_t checksum; ///< Multiboot header checksum
		infoSections  sections; ///< Executable sections data
		infoVideo     video;	///< Video mode data
	};


	/// @brief Multiboot 1 memory map view (defined below)
	class memoryMap_t;

	/// @brief Multiboot 1 information from bootloader
	struct info_t final {

		klib::kFlags<flags_t> flags; ///< Multiboot present features flag

		igros_dword_t memLow;  ///< Multiboot bios memory low info
		igros_dword_t memHigh; ///< Multiboot bios memory high info

		igros_dword_t bootDevice; ///< Multiboot boot device

		igros_dword_t cmdLine; ///< Multiboot bootloader command line

		igros_dword_t modulesCount; ///< Multiboot kernel modules count
		igros_dword_t modulesAddr;  ///< Multiboot kernel modules address

		std::array<igros_dword_t, 4_usize> syms; ///< Multiboot kernel symbols

		igros_dword_t mmapLength; ///< Multiboot memory map length
		igros_dword_t mmapAddr;	  ///< Multiboot memory map start address

		igros_dword_t drivesLength; ///< Multiboot drives info length
		igros_dword_t drivesAddr;   ///< Multiboot drives info start address

		igros_dword_t configTable; ///< Multiboot config table

		igros_dword_t bootloaderName; ///< Multiboot bootloader name

		igros_dword_t apmTable; ///< Multiboot APM table

		igros_dword_t vbeControlInfo;	  ///< Multiboot VBE control info
		igros_dword_t vbeModeInfo;	  ///< Multiboot VBE mode info
		igros_word_t  vbeModeCurrent;	  ///< Multiboot VBE current mode
		igros_word_t  vbeInterfaceSeg;	  ///< Multiboot VBE interface segment
		igros_word_t  vbeInterfaceOffset; ///< Multiboot VBE interface offset
		igros_word_t  vbeInterfaceLen;	  ///< Multiboot VBE interface length

		igros_quad_t			  fbAddress;   ///< Multiboot FB address
		igros_dword_t			  fbPitch;     ///< Multiboot FB pitch
		igros_dword_t			  fbWidth;     ///< Multiboot FB width
		igros_dword_t			  fbHeight;    ///< Multiboot FB height
		igros_byte_t			  fbBpp;       ///< Multiboot FB bpp
		igros_byte_t			  fbType;      ///< Multiboot FB type
		std::array<igros_byte_t, 6_usize> fbColorInfo; ///< Multiboot FB color info


		/// @brief Test multiboot
		/// @param magic Bootloader magic (halts if wrong)
		void test(const igros_dword_t magic) const noexcept;

		/// @brief Multiboot contains valid memory info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoMemory() const noexcept -> bool;
		/// @brief Multiboot contains valid boot device info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoBootDevice() const noexcept -> bool;
		/// @brief Multiboot contains valid kernel command line info
		[[nodiscard]]
		auto hasInfoCommandLine() const noexcept -> bool;
		/// @brief Multiboot contains valid kernel modules info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoModules() const noexcept -> bool;
		/// @brief Multiboot contains valid A.OUT sections info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoAOUT() const noexcept -> bool;
		/// @brief Multiboot contains valid ELF sections info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoELF() const noexcept -> bool;
		/// @brief Multiboot contains valid memory map info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoMemoryMap() const noexcept -> bool;
		/// @brief Multiboot contains valid drives info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoDrives() const noexcept -> bool;
		/// @brief Multiboot contains valid config table info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoConfig() const noexcept -> bool;
		/// @brief Multiboot contains valid bootloader name info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoBootloaderName() const noexcept -> bool;
		/// @brief Multiboot contains valid APM table info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoAPM() const noexcept -> bool;
		/// @brief Multiboot contains valid VBE info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoVBE() const noexcept -> bool;
		/// @brief Multiboot contains valid FrameBuffer info
		/// @return true if bootloader provided it
		[[nodiscard]]
		auto hasInfoFrameBuffer() const noexcept -> bool;

		/// @brief Get multiboot command line
		[[nodiscard]]
		auto commandLine() const noexcept -> const char*;
		/// @brief Get multiboot bootloader name
		[[nodiscard]]
		auto loaderName() const noexcept -> const char*;
		/// @brief Get multiboot memory map (empty if not provided)
		[[nodiscard]]
		auto memoryMap() const noexcept -> memoryMap_t;

		/// @brief Print multiboot flags
		void printFlags() const noexcept;
		/// @brief Print multiboot memory info
		void printMemInfo() const noexcept;
		/// @brief Print multiboot memory map
		void printMemMap() const noexcept;
		/// @brief Print multiboot VBE info
		void printVBEInfo() const noexcept;
		/// @brief Print multiboot FB info
		void printFBInfo() const noexcept;

		/// @brief Print kernel header
		void printHeader() const noexcept;
	};

	/// @brief Multiboot contains valid memory info
	[[nodiscard]]
	inline auto info_t::hasInfoMemory() const noexcept -> bool {
		return flags.isSet(flags_t::MEM);
	}

	/// @brief Multiboot contains valid boot device info
	[[nodiscard]]
	inline auto info_t::hasInfoBootDevice() const noexcept -> bool {
		return flags.isSet(flags_t::BOOT_DEV);
	}

	/// @brief Multiboot contains valid command line info
	/// @return true if bootloader provided it
	[[nodiscard]]
	inline auto info_t::hasInfoCommandLine() const noexcept -> bool {
		return flags.isSet(flags_t::CMD);
	}

	/// @brief Multiboot contains valid kernel modules info
	[[nodiscard]]
	inline auto info_t::hasInfoModules() const noexcept -> bool {
		return flags.isSet(flags_t::MODULES);
	}

	/// @brief Multiboot contains valid A.OUT sections info
	[[nodiscard]]
	inline auto info_t::hasInfoAOUT() const noexcept -> bool {
		// A.OUT but not ELF
		return flags.isSet(flags_t::SYMS_AOUT) && !flags.isSet(flags_t::SYMS_ELF);
	}

	/// @brief Multiboot contains valid ELF sections info
	[[nodiscard]]
	inline auto info_t::hasInfoELF() const noexcept -> bool {
		// ELF but not A.OUT
		return flags.isSet(flags_t::SYMS_ELF) && !flags.isSet(flags_t::SYMS_AOUT);
	}

	/// @brief Multiboot contains valid memory map info
	[[nodiscard]]
	inline auto info_t::hasInfoMemoryMap() const noexcept -> bool {
		return flags.isSet(flags_t::MEM_MAP);
	}

	/// @brief Multiboot contains valid drives info
	[[nodiscard]]
	inline auto info_t::hasInfoDrives() const noexcept -> bool {
		return flags.isSet(flags_t::DRIVES);
	}

	/// @brief Multiboot contains valid config table info
	[[nodiscard]]
	inline auto info_t::hasInfoConfig() const noexcept -> bool {
		return flags.isSet(flags_t::TABLE_CONFIG);
	}

	/// @brief Multiboot contains valid bootloader name info
	[[nodiscard]]
	inline auto info_t::hasInfoBootloaderName() const noexcept -> bool {
		return flags.isSet(flags_t::LOADER_NAME);
	}

	/// @brief Multiboot contains valid APM table info
	[[nodiscard]]
	inline auto info_t::hasInfoAPM() const noexcept -> bool {
		return flags.isSet(flags_t::TABLE_APM);
	}

	/// @brief Multiboot contains valid VBE info
	[[nodiscard]]
	inline auto info_t::hasInfoVBE() const noexcept -> bool {
		return flags.isSet(flags_t::VBE);
	}

	/// @brief Multiboot contains valid FrameBuffer info
	[[nodiscard]]
	inline auto info_t::hasInfoFrameBuffer() const noexcept -> bool {
		return flags.isSet(flags_t::FRAME_BUF);
	}


	/// @brief Get multiboot command line (nullptr if not provided or not directly mapped)
	/// @return Command line, nullptr if not provided
	[[nodiscard]]
	inline auto info_t::commandLine() const noexcept -> const char* {
		return hasInfoCommandLine() ? mem::direct::string(cmdLine) : nullptr;
	}

	/// @brief Get multiboot bootloader name (nullptr if not provided or not directly mapped)
	/// @return Bootloader name, nullptr if not provided
	[[nodiscard]]
	inline auto info_t::loaderName() const noexcept -> const char* {
		return hasInfoBootloaderName() ? mem::direct::string(bootloaderName) : nullptr;
	}


	/// @brief Multiboot 1 memory entry type
	enum class MEMORY_MAP_TYPE : igros_dword_t {
		AVAILABLE = 1_u32, ///< Memory available
		RESERVED  = 2_u32, ///< Memory reserved
		ACPI	  = 3_u32, ///< Memory is ACPI reclaimable
		NVS	  = 4_u32, ///< Memory is NVS
		BAD	  = 5_u32, ///< BAD memory
	};

	/// @brief Multiboot 1 memory map entry
	struct memoryMapEntry final {
		igros_dword_t	size;	 ///< Memory entry size
		igros_quad_t	address; ///< Memory entry address
		igros_quad_t	length;	 ///< Memory entry length
		MEMORY_MAP_TYPE type;	 ///< Memory entry type
	};

	/// @brief Multiboot 1 module entry
	struct moduleEntry final {
		igros_dword_t start;	///< Module start address
		igros_dword_t end;	///< Module end address
		igros_dword_t name;	///< Module name string
		igros_dword_t reserved; ///< Reserved
	};


	/// @brief VBE config
	struct vbeConfig final {
		std::array<igros_sbyte_t, 4_usize> signature;	///< VBE signature ("VESA")
		igros_word_t			   version;	///< VBE version (e.g. 0x0300 = 3.0)
		igros_dword_t			   oem;		///< OEM string
		igros_dword_t			   caps;	///< VBE capabilities
		igros_dword_t			   modes;	///< VBE modes pointer
		igros_word_t			   memory;	///< VBE video memory size in 64 Kb. blocks
		igros_word_t			   rev;		///< VBE revision string
		igros_dword_t			   vendor;	///< VBE vendor string
		igros_dword_t			   productName; ///< VBE product name string
		igros_dword_t			   productRev;	///< VBE product revision string
		std::array<std::byte, 222_usize>   reserved;	///< VBE reserved
		std::array<std::byte, 256_usize>   oemData;	///< VBE OEM data
	};

	/// @brief VBE mode
	struct vbeMode final {
		igros_word_t  attributes;	     ///< Mode attributes (bit 7 - linear frame buffer)
		igros_byte_t  windowA;		     ///< Window A attributes
		igros_byte_t  windowB;		     ///< Window B attributes
		igros_word_t  granularity;	     ///< Window granularity in KB
		igros_word_t  windowSize;	     ///< Window size in KB
		igros_word_t  segmentA;		     ///< Window A segment
		igros_word_t  segmentB;		     ///< Window B segment
		igros_dword_t funcPtr;		     ///< Window positioning function (real mode)
		igros_word_t  pitch;		     ///< Bytes per scan line
		igros_word_t  width;		     ///< Width in pixels (characters in text modes)
		igros_word_t  height;		     ///< Height in pixels (characters in text modes)
		igros_byte_t  charW;		     ///< Character cell width
		igros_byte_t  charH;		     ///< Character cell height
		igros_byte_t  planes;		     ///< Number of memory planes
		igros_byte_t  bpp;		     ///< Bits per pixel
		igros_byte_t  banks;		     ///< Number of banks
		igros_byte_t  memoryModel;	     ///< Memory model (6 - direct color)
		igros_byte_t  bankSize;		     ///< Bank size in KB
		igros_byte_t  imagePages;	     ///< Number of extra image pages
		igros_byte_t  reserved0;	     ///< Reserved
		igros_byte_t  redMask;		     ///< Red mask size
		igros_byte_t  redPosition;	     ///< Red field position
		igros_byte_t  greenMask;	     ///< Green mask size
		igros_byte_t  greenPosition;	     ///< Green field position
		igros_byte_t  blueMask;		     ///< Blue mask size
		igros_byte_t  bluePosition;	     ///< Blue field position
		igros_byte_t  reservedMask;	     ///< Reserved mask size
		igros_byte_t  reservedPosition;	     ///< Reserved field position
		igros_byte_t  directColorAttributes; ///< Direct color mode attributes
		igros_dword_t physbase;		     ///< Linear frame buffer physical address
		igros_dword_t reserved1;	     ///< Reserved
		igros_word_t  reserved2;	     ///< Reserved
	};

#pragma pack(pop)


	/// @brief Multiboot 1 memory map view (entries have variable size)
	class memoryMap_t final {

		const igros_byte_t* mBegin {nullptr}; ///< First entry
		const igros_byte_t* mEnd {nullptr};   ///< Memory map end


	public:

		/// @brief Memory map end marker
		struct sentinel_t final {
			const igros_byte_t* end; ///< Memory map end
		};

		/// @brief Memory map entries iterator
		class iterator_t final {

			const igros_byte_t* mEntry; ///< Current entry

		public:

			/// @brief C-tor
			/// @param entry Entry address
			constexpr explicit iterator_t(const igros_byte_t* const entry) noexcept : mEntry {entry} {}

			/// @brief Get current entry
			/// @return Current entry
			[[nodiscard]]
			auto operator*() const noexcept -> const memoryMapEntry & {
				return *std::bit_cast<const memoryMapEntry*>(mEntry);
			}

			/// @brief Move to next entry ("size" field doesn't count itself)
			/// @return This iterator
			auto operator++() noexcept -> iterator_t & {
				const auto size {std::bit_cast<const memoryMapEntry*>(mEntry)->size};
				// Zero size would never advance - assume standard entry size
				mEntry += sizeof(memoryMapEntry::size) + ((0_u32 != size) ? size : (sizeof(memoryMapEntry) - sizeof(memoryMapEntry::size)));
				return *this;
			}

			/// @brief Check if whole entry no longer fits into memory map
			/// @param sentinel Memory map end
			/// @return true if iteration is done
			[[nodiscard]]
			auto operator==(const sentinel_t &sentinel) const noexcept -> bool {
				return (mEntry + sizeof(memoryMapEntry)) > sentinel.end;
			}
		};

		/// @brief Empty memory map
		constexpr memoryMap_t() noexcept = default;
		/// @brief Memory map of given length
		/// @param begin First entry address
		/// @param length Memory map length in bytes
		constexpr memoryMap_t(const igros_byte_t* const begin, const igros_usize_t length) noexcept :
			mBegin {begin},
			mEnd {begin + length} {}

		/// @brief First entry
		/// @return Iterator to first entry
		[[nodiscard]]
		constexpr auto begin() const noexcept -> iterator_t {
			return iterator_t {mBegin};
		}
		/// @brief Memory map end
		/// @return Sentinel of memory map end
		[[nodiscard]]
		constexpr auto end() const noexcept -> sentinel_t {
			return sentinel_t {mEnd};
		}
	};


	/// @brief Get multiboot memory map (empty if not provided or not directly mapped)
	/// @return Memory map
	[[nodiscard]]
	inline auto info_t::memoryMap() const noexcept -> memoryMap_t {
		// Check if memory map provided
		if (!hasInfoMemoryMap()) [[unlikely]] {
			return memoryMap_t {};
		}
		// Memory map at physical address
		const auto* const entries {mem::direct::pointer<const igros_byte_t>(mmapAddr, mmapLength)};
		if (nullptr == entries) [[unlikely]] {
			return memoryMap_t {};
		}
		return memoryMap_t {entries, mmapLength};
	}


} // namespace igros::multiboot

