////////////////////////////////////////////////////////////////
//
//	IgrOS platform description
//
//	File:	platform.hpp
//	Date:	16 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>

#include <arch/types.hpp>


// OS platform namespace
namespace igros::platform {


	// Platform name ID
	enum class name_t : byte_t {
		UNKNOWN		= 0x00,
		I386		= 0x01,
		X86_64		= 0x02,
		ARM		= 0x04,
		ARM64		= 0x08
	};


#if	defined (IGROS_ARCH_i386)

	constexpr auto PLATFORM_NAME = name_t::I386;

#elif	defined (IGROS_ARCH_x86_64)

	constexpr auto PLATFORM_NAME = name_t::X86_64;

#else

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

