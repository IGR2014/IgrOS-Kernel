////////////////////////////////////////////////////////////////
//
//	IgrOS platform description
//
//	File:	platform.hpp
//	Date:	17 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>

#include <arch/types.hpp>


// Kernel start and end
extern const igros::byte_t _SECTION_KERNEL_START_;
extern const igros::byte_t _SECTION_KERNEL_END_;


// OS platform namespace
namespace igros::platform {


	// Get kernel start address
	inline static const auto KERNEL_START() noexcept {
		return &_SECTION_KERNEL_START_;
	}

	// Get kernel end address
	inline static const auto KERNEL_END() noexcept {
		return &_SECTION_KERNEL_END_;
	}

	// Get kernel size
	inline static const auto KERNEL_SIZE() noexcept {
		return (KERNEL_END() - KERNEL_START());
	}


	// Platform name ID
	enum class name_t : byte_t {
		UNKNOWN		= 0x00,
		I386		= 0x01,
		X86_64		= 0x02,
		ARM		= 0x04,
		ARM64		= 0x08
	};


#if	defined (IGROS_ARCH_i386)
	// i386 platform
	constexpr auto PLATFORM_NAME = name_t::I386;
#elif	defined (IGROS_ARCH_x86_64)
	// x86_64 platform
	constexpr auto PLATFORM_NAME = name_t::X86_64;
#else
	// Unknown platform
	constexpr auto PLATFORM_NAME = name_t::UNKNOWN;
#endif

	// Check if i386
	constexpr bool isI386() noexcept {
		return (name_t::I386 == PLATFORM_NAME);
	}

	// Check if x86_64
	constexpr bool isX86_64() noexcept {
		return (name_t::X86_64 == PLATFORM_NAME);
	}


	// Platform init function pointer type
	using initPtr_t		= std::add_pointer_t<void()>;
	// Platform finalize function pointer type
	using finalizePtr_t	= std::add_pointer_t<void()>;

	// Platform shutdown function pointer type
	using shutdownPtr_t	= std::add_pointer_t<void()>;
	// Platform reboot function pointer type
	using rebootPtr_t	= std::add_pointer_t<void()>;

	// Platform suspend function pointer type
	using suspendPtr_t	= std::add_pointer_t<void()>;
	// Platform wakeup function pointer type
	using wakeupPtr_t	= std::add_pointer_t<void()>;


	// Platform desciption structure
	struct description_t {

		const sbyte_t* const	mName;			// Platform name

		initPtr_t		mInit;			// Init function
		finalizePtr_t		mFinalzie;		// Finalize function

		shutdownPtr_t		mShutdown;		// Shutdown function
		rebootPtr_t		mReboot;		// Reboot function

		suspendPtr_t		mSuspend;		// Suspend platform
		wakeupPtr_t		mWakeup;		// Wakeup platform

	};


	// Platform description
	extern const description_t CURRENT_PLATFORM;


	// Get platform name
	[[nodiscard]] inline auto name() noexcept {
		return CURRENT_PLATFORM.mName;
	}

	// Initialize platform
	inline void initialize() noexcept {
		CURRENT_PLATFORM.mInit();
	}

	// Finalize platform
	inline void finalize() noexcept {
		CURRENT_PLATFORM.mFinalzie();
	}

	// Shutdown platform
	inline void shutdown() noexcept {
		CURRENT_PLATFORM.mShutdown();
	}

	// Reboot platform
	inline void reboot() noexcept {
		CURRENT_PLATFORM.mReboot();
	}

	// Suspend platform
	inline void suspend() noexcept {
		CURRENT_PLATFORM.mSuspend();
	}

	// Wakeup platform
	inline void wakeup() noexcept {
		CURRENT_PLATFORM.mWakeup();
	}


}       // namespace igros::platform

