////////////////////////////////////////////////////////////////
//
//	IgrOS platform description
//
//	File:	platform.hpp
//	Date:	10 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once

// C++
#include <type_traits>
#include <string_view>
// IgrOS
#include <arch/types.hpp>


// Kernel start and end
extern const igros::byte_t _SECTION_KERNEL_START_;
extern const igros::byte_t _SECTION_KERNEL_END_;


// OS platform namespace
namespace igros::platform {


	// Get kernel start address
	[[nodiscard]]
	constexpr const auto KERNEL_START() noexcept {
		return &_SECTION_KERNEL_START_;
	}

	// Get kernel end address
	[[nodiscard]]
	constexpr const auto KERNEL_END() noexcept {
		return &_SECTION_KERNEL_END_;
	}

	// Get kernel size
	[[nodiscard]]
	constexpr const auto KERNEL_SIZE() noexcept {
		return (KERNEL_END() - KERNEL_START());
	}


	// Platform desciption structure
	class description_t final {

		// Platform name ID
		enum class PLATFORM_ARCH_NAME : dword_t {
			UNKNOWN		= 0x00000000,
			I386		= 0x00000001,
			X86_64		= 0x00000002,
			ARM		= 0x00000004,
			ARM64		= 0x00000008,
			AVR		= 0x00000010
		};


#if	defined (IGROS_ARCH_i386)
		// i386 platform
		constexpr static PLATFORM_ARCH_NAME PLATFORM_NAME = PLATFORM_ARCH_NAME::I386;
#elif	defined (IGROS_ARCH_x86_64)
		// x86_64 platform
		constexpr static PLATFORM_ARCH_NAME PLATFORM_NAME = PLATFORM_ARCH_NAME::X86_64;
#else
		// Unknown platform
		constexpr static PLATFORM_ARCH_NAME PLATFORM_NAME = PLATFORM_ARCH_NAME::UNKNOWN;
#endif


		// Platform init function pointer type
		using init_t		= std::add_pointer_t<void()>;
		// Platform finalize function pointer type
		using finalize_t	= std::add_pointer_t<void()>;

		// Platform shutdown function pointer type
		using shutdown_t	= std::add_pointer_t<void()>;
		// Platform reboot function pointer type
		using reboot_t		= std::add_pointer_t<void()>;

		// Platform suspend function pointer type
		using suspend_t		= std::add_pointer_t<void()>;
		// Platform wakeup function pointer type
		using wakeup_t		= std::add_pointer_t<void()>;


		const char* const	mName;			// Platform name

		init_t			mInit;			// Init function
		finalize_t		mFinalzie;		// Finalize function

		shutdown_t		mShutdown;		// Shutdown function
		reboot_t		mReboot;		// Reboot function

		suspend_t		mSuspend;		// Suspend platform
		wakeup_t		mWakeup;		// Wakeup platform


		// C-tor
		description_t() = delete;


	public:

		// C-tor #2
		constexpr description_t(
			const char* const	name,
			const init_t		init,
			const finalize_t	finalize,
			const shutdown_t	shutdown,
			const reboot_t		reboot,
			const suspend_t		suspend,
			const wakeup_t		wakeup
		) noexcept;

		// Check if i386
		[[nodiscard]]
		constexpr bool	isI386() const noexcept;
		// Check if x86_64
		[[nodiscard]]
		constexpr bool	isX86_64() const noexcept;
		// Check if arm32
		[[nodiscard]]
		constexpr bool	isARM32() const noexcept;
		// Check if arm64
		[[nodiscard]]
		constexpr bool	isARM64() const noexcept;
		// Check if avr
		[[nodiscard]]
		constexpr bool	isAVR() const noexcept;

		// Get platform name
		[[nodiscard]]
		const auto	name() const noexcept;

		// Initialize platform
		void	initialize() const noexcept;
		// Finalize platform
		void	finalize() const noexcept;

		// Shutdown platform
		void	shutdown() const noexcept;
		// Reboot platform
		void	reboot() const noexcept;

		// Suspend platform
		void	suspend() const noexcept;
		// Wakeup platform
		void	wakeup() const noexcept;


	};


	// C-tor #2
	constexpr description_t::description_t(
		const char* const	name,
		const init_t		init,
		const finalize_t	finalize,
		const shutdown_t	shutdown,
		const reboot_t		reboot,
		const suspend_t		suspend,
		const wakeup_t		wakeup
	) noexcept
		: mName		(name),
		  mInit		(init),
		  mFinalzie	(finalize),
		  mShutdown	(shutdown),
		  mReboot	(reboot),
		  mSuspend	(suspend),
		  mWakeup	(wakeup) {}


	// Check if i386
	[[nodiscard]]
	constexpr bool description_t::isI386() const noexcept {
		return (PLATFORM_ARCH_NAME::I386 == PLATFORM_NAME);
	}

	// Check if x86_64
	[[nodiscard]]
	constexpr bool description_t::isX86_64() const noexcept {
		return (PLATFORM_ARCH_NAME::X86_64 == PLATFORM_NAME);
	}

	// Check if arm32
	[[nodiscard]]
	constexpr bool description_t::isARM32() const noexcept {
		return (PLATFORM_ARCH_NAME::ARM == PLATFORM_NAME);
	}

	// Check if arm64
	[[nodiscard]]
	constexpr bool description_t::isARM64() const noexcept {
		return (PLATFORM_ARCH_NAME::ARM64 == PLATFORM_NAME);
	}

	// Check if avr
	[[nodiscard]]
	constexpr bool description_t::isAVR() const noexcept {
		return (PLATFORM_ARCH_NAME::AVR == PLATFORM_NAME);
	}


	// Get platform name
	[[nodiscard]]
	inline const auto description_t::name() const noexcept {
		return mName;
	}

	// Initialize platform
	inline void description_t::initialize() const noexcept {
		mInit();
	}

	// Finalize platform
	inline void description_t::finalize() const noexcept {
		mFinalzie();
	}

	// Shutdown platform
	inline void description_t::shutdown() const noexcept {
		mShutdown();
	}

	// Reboot platform
	inline void description_t::reboot() const noexcept {
		mReboot();
	}

	// Suspend platform
	inline void description_t::suspend() const noexcept {
		mSuspend();
	}

	// Wakeup platform
	inline void description_t::wakeup() const noexcept {
		mWakeup();
	}


	// Platform description
	extern const description_t CURRENT_PLATFORM;


}       // namespace igros::platform

