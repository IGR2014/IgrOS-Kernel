////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		IgrOS platform description
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once

// C++
#include <bit>
#include <string_view>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>


/// @brief Kernel start
extern const igros::igros_byte_t _SECTION_KERNEL_START_;
/// @brief Kernel end
extern const igros::igros_byte_t _SECTION_KERNEL_END_;
// Kernel sections start (from linker script, page aligned)
extern const igros::igros_byte_t _SECTION_TEXT_START_;	 ///< Code section start
extern const igros::igros_byte_t _SECTION_RODATA_START_; ///< Read-only data section start
extern const igros::igros_byte_t _SECTION_DATA_START_;	 ///< Writable data section start
/// @brief Boot stack guard page (from boot.s, page aligned)
extern const igros::igros_byte_t _KERNEL_STACK_GUARD_;
/// @brief Kernel virtual memory offset (absolute symbol, its address is the offset value)
extern const igros::igros_byte_t KERNEL_OFFSET_VIRT;


// OS platform namespace
namespace igros::platform {


	/// @brief Platform desciption structure
	class Platform final {

		/// @brief Platform name ID
		enum class ARCH_NAME : igros_dword_t {
			UNKNOWN = 0x00000000_u32,
			I386	= 0x00000001_u32,
			X86_64	= 0x00000002_u32,
			ARM	= 0x00000004_u32,
			ARM64	= 0x00000008_u32,
			AVR	= 0x00000010_u32,
		};


#if defined(IGROS_ARCH_i386)

		/// @brief i386 platform
		constexpr static auto PLATFORM_NAME {ARCH_NAME::I386};

#elif defined(IGROS_ARCH_x86_64)

		/// @brief x86_64 platform
		constexpr static auto PLATFORM_NAME {ARCH_NAME::X86_64};

#else

		/// @brief Unknown platform
		constexpr static auto PLATFORM_NAME {ARCH_NAME::UNKNOWN};

#endif


		/// @brief Platform init function pointer type
		using init_t = std::add_pointer_t<void()>;
		/// @brief Platform finalize function pointer type
		using finalize_t = std::add_pointer_t<void()>;

		/// @brief Platform shutdown function pointer type
		using shutdown_t = std::add_pointer_t<void()>;
		/// @brief Platform reboot function pointer type
		using reboot_t = std::add_pointer_t<void()>;

		/// @brief Platform suspend function pointer type
		using suspend_t = std::add_pointer_t<void()>;
		/// @brief Platform wakeup function pointer type
		using wakeup_t = std::add_pointer_t<void()>;


		const char* const mName {nullptr}; ///< Platform name

		init_t	   mInit {nullptr};	///< Init function
		finalize_t mFinalize {nullptr}; ///< Finalize function

		shutdown_t mShutdown {nullptr}; ///< Shutdown function
		reboot_t   mReboot {nullptr};	///< Reboot function

		suspend_t mSuspend {nullptr}; ///< Suspend platform
		wakeup_t  mWakeup {nullptr};  ///< Wakeup platform


	public:

		/// @brief Default c-tor
		constexpr Platform() noexcept = default;
		/// @brief C-tor
		/// @param name Platform name
		/// @param initFunc Initialization function
		/// @param finalizeFunc Finalization function
		/// @param shutdownFunc Shutdown function
		/// @param rebootFunc Reboot function
		/// @param suspendFunc Suspend function
		/// @param wakeupFunc Wakeup function
		constexpr Platform(
			const char* const name,
			const init_t	  initFunc,
			const finalize_t  finalizeFunc,
			const shutdown_t  shutdownFunc,
			const reboot_t	  rebootFunc,
			const suspend_t	  suspendFunc,
			const wakeup_t	  wakeupFunc
		) noexcept;

		/// @brief Get platform name
		/// @return Platform name
		[[nodiscard]]
		auto name() const noexcept -> const char*;

		/// @brief Initialize platform
		void initialize() const noexcept;
		/// @brief Finalize platform
		void finalize() const noexcept;

		/// @brief Shutdown platform
		void shutdown() const noexcept;
		/// @brief Reboot platform
		void reboot() const noexcept;

		/// @brief Suspend platform
		void suspend() const noexcept;
		/// @brief Wakeup platform
		void wakeup() const noexcept;

		/// @brief Get kernel start address
		/// @return Kernel start address
		[[nodiscard]]
		constexpr static auto kernelStart() noexcept -> const igros_byte_t*;
		/// @brief Get kernel end address
		/// @return Kernel end address
		[[nodiscard]]
		constexpr static auto kernelEnd() noexcept -> const igros_byte_t*;
		/// @brief Get kernel size
		/// @return Kernel size in bytes
		[[nodiscard]]
		static auto kernelSize() noexcept -> igros_usize_t;
		/// @brief Get kernel code start (code: [textStart, rodataStart))
		[[nodiscard]]
		constexpr static auto textStart() noexcept -> const igros_byte_t*;
		/// @brief Get kernel read-only data start (read-only data: [rodataStart, dataStart))
		[[nodiscard]]
		constexpr static auto rodataStart() noexcept -> const igros_byte_t*;
		/// @brief Get kernel writable data start (data and bss: [dataStart, kernelEnd))
		[[nodiscard]]
		constexpr static auto dataStart() noexcept -> const igros_byte_t*;
		/// @brief Get boot stack guard page (one page right below boot stack, never mapped)
		[[nodiscard]]
		constexpr static auto stackGuard() noexcept -> const igros_byte_t*;
		/// @brief Get kernel virtual memory offset (kernel virtual address = physical address + offset)
		[[nodiscard]]
		static auto kernelOffset() noexcept -> igros_usize_t;

		/// @brief Check if i386
		/// @return true if kernel is built for i386
		[[nodiscard]]
		constexpr static auto isI386() noexcept -> bool;
		/// @brief Check if x86_64
		/// @return true if kernel is built for x86_64
		[[nodiscard]]
		constexpr static auto isX86_64() noexcept -> bool;
		/// @brief Check if arm32
		/// @return true if kernel is built for ARM32
		[[nodiscard]]
		constexpr static auto isARM32() noexcept -> bool;
		/// @brief Check if arm64
		/// @return true if kernel is built for ARM64
		[[nodiscard]]
		constexpr static auto isARM64() noexcept -> bool;
		/// @brief Check if avr
		/// @return true if kernel is built for AVR
		[[nodiscard]]
		constexpr static auto isAVR() noexcept -> bool;

		/// @brief Get kernel current platform (implemented in corresponding platform file)
		/// @return Platform the kernel runs on
		[[nodiscard]]
		static auto current() noexcept -> const Platform &;
	};


	/// @brief C-tor
	constexpr Platform::Platform(
		const char* const name,
		const init_t	  initFunc, // NOLINT(bugprone-easily-swappable-parameters): platform callbacks in fixed order
		const finalize_t  finalizeFunc,
		const shutdown_t  shutdownFunc,
		const reboot_t	  rebootFunc,
		const suspend_t	  suspendFunc,
		const wakeup_t	  wakeupFunc
	) noexcept :
		mName(name),
		mInit(initFunc),
		mFinalize(finalizeFunc),
		mShutdown(shutdownFunc),
		mReboot(rebootFunc),
		mSuspend(suspendFunc),
		mWakeup(wakeupFunc) {}


	/// @brief Get platform name
	[[nodiscard]]
	inline auto Platform::name() const noexcept -> const char* {
		// Get platform name string
		return mName;
	}


	/// @brief Initialize platform
	inline void Platform::initialize() const noexcept {
		// Check if no init func
		if (nullptr == mInit) [[unlikely]] {
			// Exit
			return;
		}
		// Do init
		mInit();
	}

	/// @brief Finalize platform
	inline void Platform::finalize() const noexcept {
		// Check if no finalize func
		if (nullptr == mFinalize) [[unlikely]] {
			// Exit
			return;
		}
		// Do finalize
		mFinalize();
	}

	/// @brief Shutdown platform
	inline void Platform::shutdown() const noexcept {
		// Check if no shutdown func
		if (nullptr == mShutdown) [[unlikely]] {
			// Exit
			return;
		}
		// Do shutdown
		mShutdown();
	}

	/// @brief Reboot platform
	inline void Platform::reboot() const noexcept {
		// Check if no reboot func
		if (nullptr == mReboot) [[unlikely]] {
			// Exit
			return;
		}
		// Do reboot
		mReboot();
	}

	/// @brief Suspend platform
	inline void Platform::suspend() const noexcept {
		// Check if no suspend func
		if (nullptr == mSuspend) [[unlikely]] {
			// Exit
			return;
		}
		// Do suspend
		mSuspend();
	}

	/// @brief Wakeup platform
	inline void Platform::wakeup() const noexcept {
		// Check if no wakeup func
		if (nullptr == mWakeup) [[unlikely]] {
			// Exit
			return;
		}
		// Do wakeup
		mWakeup();
	}


	/// @brief Get kernel start address
	[[nodiscard]]
	constexpr auto Platform::kernelStart() noexcept -> const igros_byte_t* {
		// Pointer to kernel start (from linker script)
		return &_SECTION_KERNEL_START_;
	}

	/// @brief Get kernel end address
	[[nodiscard]]
	constexpr auto Platform::kernelEnd() noexcept -> const igros_byte_t* {
		// Pointer to kernel end (from linker script)
		return &_SECTION_KERNEL_END_;
	}

	/// @brief Get kernel size
	[[nodiscard]]
	inline auto Platform::kernelSize() noexcept -> igros_usize_t {
		// Kernel size (end - start): addresses, not pointers - linker symbols are
		// different objects, so pointer subtraction would be undefined behavior
		return std::bit_cast<igros_usize_t>(Platform::kernelEnd()) - std::bit_cast<igros_usize_t>(Platform::kernelStart());
	}

	/// @brief Get kernel code start
	/// @return Kernel code start address
	[[nodiscard]]
	constexpr auto Platform::textStart() noexcept -> const igros_byte_t* {
		return &_SECTION_TEXT_START_;
	}

	/// @brief Get kernel read-only data start
	/// @return Kernel read-only data start address
	[[nodiscard]]
	constexpr auto Platform::rodataStart() noexcept -> const igros_byte_t* {
		return &_SECTION_RODATA_START_;
	}

	/// @brief Get kernel writable data start
	/// @return Kernel writable data start address
	[[nodiscard]]
	constexpr auto Platform::dataStart() noexcept -> const igros_byte_t* {
		return &_SECTION_DATA_START_;
	}

	/// @brief Get boot stack guard page
	/// @return Boot stack guard page address
	[[nodiscard]]
	constexpr auto Platform::stackGuard() noexcept -> const igros_byte_t* {
		return &_KERNEL_STACK_GUARD_;
	}

	/// @brief Get kernel virtual memory offset
	/// @return Kernel virtual memory offset
	[[nodiscard]]
	inline auto Platform::kernelOffset() noexcept -> igros_usize_t {
		// Offset from linker script
		return std::bit_cast<igros_usize_t>(&KERNEL_OFFSET_VIRT);
	}


	/// @brief Check if i386
	[[nodiscard]]
	constexpr auto Platform::isI386() noexcept -> bool {
		// Check platform i386
		return ARCH_NAME::I386 == PLATFORM_NAME;
	}

	/// @brief Check if x86_64
	[[nodiscard]]
	constexpr auto Platform::isX86_64() noexcept -> bool {
		// Check platform x86_64
		return ARCH_NAME::X86_64 == PLATFORM_NAME;
	}

	/// @brief Check if arm32
	[[nodiscard]]
	constexpr auto Platform::isARM32() noexcept -> bool {
		// Check platform arm
		return ARCH_NAME::ARM == PLATFORM_NAME;
	}

	/// @brief Check if arm64
	[[nodiscard]]
	constexpr auto Platform::isARM64() noexcept -> bool {
		// Check platform arm64
		return ARCH_NAME::ARM64 == PLATFORM_NAME;
	}

	/// @brief Check if avr
	[[nodiscard]]
	constexpr auto Platform::isAVR() noexcept -> bool {
		// Check platform avr
		return ARCH_NAME::AVR == PLATFORM_NAME;
	}


} // namespace igros::platform

