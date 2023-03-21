////////////////////////////////////////////////////////////////
//
//	IgrOS platform description
//
//	File:	platform.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once

// C++
#include <type_traits>
#include <string_view>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel start
extern const igros::igros_byte_t _SECTION_KERNEL_START_;
// Kernel end
extern const igros::igros_byte_t _SECTION_KERNEL_END_;


// OS platform namespace
namespace igros::platform {


	// Platform desciption structure
	class Platform final {

		// Platform name ID
		enum class ARCH_NAME : igros_dword_t {
			UNKNOWN		= 0x00000000_u32,
			I386		= 0x00000001_u32,
			X86_64		= 0x00000002_u32,
			ARM		= 0x00000004_u32,
			ARM64		= 0x00000008_u32,
			AVR		= 0x00000010_u32
		};


#if	defined (IGROS_ARCH_i386)

		// i386 platform
		constexpr static auto PLATFORM_NAME	{ARCH_NAME::I386};

#elif	defined (IGROS_ARCH_x86_64)

		// x86_64 platform
		constexpr static auto PLATFORM_NAME	{ARCH_NAME::X86_64};

#else

		// Unknown platform
		constexpr static auto PLATFORM_NAME	{ARCH_NAME::UNKNOWN};

#endif


		// Platform init function pointer type
		using init_t		= std::add_pointer_t<void ()>;
		// Platform finalize function pointer type
		using finalize_t	= std::add_pointer_t<void ()>;

		// Platform shutdown function pointer type
		using shutdown_t	= std::add_pointer_t<void ()>;
		// Platform reboot function pointer type
		using reboot_t		= std::add_pointer_t<void ()>;

		// Platform suspend function pointer type
		using suspend_t		= std::add_pointer_t<void ()>;
		// Platform wakeup function pointer type
		using wakeup_t		= std::add_pointer_t<void ()>;


		const char* const	mName		{nullptr};		// Platform name

		init_t			mInit		{nullptr};		// Init function
		finalize_t		mFinalzie	{nullptr};		// Finalize function

		shutdown_t		mShutdown	{nullptr};		// Shutdown function
		reboot_t		mReboot		{nullptr};		// Reboot function

		suspend_t		mSuspend	{nullptr};		// Suspend platform
		wakeup_t		mWakeup		{nullptr};		// Wakeup platform


	public:

		// Default c-tor
		constexpr Platform() noexcept = default;
		// C-tor
		constexpr Platform(
			const char* const	name,
			const init_t		init,
			const finalize_t	finalize,
			const shutdown_t	shutdown,
			const reboot_t		reboot,
			const suspend_t		suspend,
			const wakeup_t		wakeup
		) noexcept;

		// Get platform name
		[[nodiscard]]
		auto	name() const noexcept -> const char*;

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

		// Get kernel start address
		[[nodiscard]]
		constexpr static auto	kernelStart() noexcept -> const igros_byte_t*;
		// Get kernel end address
		[[nodiscard]]
		constexpr static auto	kernelEnd() noexcept -> const igros_byte_t*;
		// Get kernel size
		[[nodiscard]]
		constexpr static auto	kernelSize() noexcept -> igros_usize_t;

		// Check if i386
		[[nodiscard]]
		constexpr static auto	isI386() noexcept -> bool;
		// Check if x86_64
		[[nodiscard]]
		constexpr static auto	isX86_64() noexcept -> bool;
		// Check if arm32
		[[nodiscard]]
		constexpr static auto	isARM32() noexcept -> bool;
		// Check if arm64
		[[nodiscard]]
		constexpr static auto	isARM64() noexcept -> bool;
		// Check if avr
		[[nodiscard]]
		constexpr static auto	isAVR() noexcept -> bool;

		// Get kernel current platform (implemented in corresponding platform file)
		[[nodiscard]]
		static auto	current() noexcept -> const Platform&;


	};


	// C-tor
	constexpr Platform::Platform(
		const char* const	name,
		const init_t		init,
		const finalize_t	finalize,
		const shutdown_t	shutdown,
		const reboot_t		reboot,
		const suspend_t		suspend,
		const wakeup_t		wakeup
	) noexcept :
		mName		(name),
		mInit		(init),
		mFinalzie	(finalize),
		mShutdown	(shutdown),
		mReboot		(reboot),
		mSuspend	(suspend),
		mWakeup		(wakeup) {}


	// Get platform name
	[[nodiscard]]
	inline auto Platform::name() const noexcept -> const char* {
		// Get platform name string
		return mName;
	}


	// Initialize platform
	inline void Platform::initialize() const noexcept {
		// Check if no init func
		if (nullptr == mInit) [[unlikely]] {
			// Exit
			return;
		}
		// Do init
		mInit();
	}

	// Finalize platform
	inline void Platform::finalize() const noexcept {
		// Check if no finalize func
		if (nullptr == mFinalzie) [[unlikely]] {
			// Exit
			return;
		}
		// Do finalize
		mFinalzie();
	}

	// Shutdown platform
	inline void Platform::shutdown() const noexcept {
		// Check if no shutdown func
		if (nullptr == mShutdown) [[unlikely]] {
			// Exit
			return;
		}
		// Do shutdown
		mShutdown();
	}

	// Reboot platform
	inline void Platform::reboot() const noexcept {
		// Check if no reboot func
		if (nullptr == mReboot) [[unlikely]] {
			// Exit
			return;
		}
		// Do reboot
		mReboot();
	}

	// Suspend platform
	inline void Platform::suspend() const noexcept {
		// Check if no suspend func
		if (nullptr == mSuspend) [[unlikely]] {
			// Exit
			return;
		}
		// Do suspend
		mSuspend();
	}

	// Wakeup platform
	inline void Platform::wakeup() const noexcept {
		// Check if no wakeup func
		if (nullptr == mWakeup) [[unlikely]] {
			// Exit
			return;
		}
		// Do wakeup
		mWakeup();
	}


	// Get kernel start address
	[[nodiscard]]
	constexpr auto Platform::kernelStart() noexcept -> const igros_byte_t* {
		// Pointer to kernel start (from linker script)
		return &_SECTION_KERNEL_START_;
	}

	// Get kernel end address
	[[nodiscard]]
	constexpr auto Platform::kernelEnd() noexcept -> const igros_byte_t* {
		// Pointer to kernel end (from linker script)
		return &_SECTION_KERNEL_END_;
	}

	// Get kernel size
	[[nodiscard]]
	constexpr auto Platform::kernelSize() noexcept -> igros_usize_t {
		// Kernel size (end - start)
		return Platform::kernelEnd() - Platform::kernelStart();
	}


	// Check if i386
	[[nodiscard]]
	constexpr auto Platform::isI386() noexcept -> bool {
		// Check platform i386
		return ARCH_NAME::I386 == PLATFORM_NAME;
	}

	// Check if x86_64
	[[nodiscard]]
	constexpr auto Platform::isX86_64() noexcept -> bool {
		// Check platform x86_64
		return ARCH_NAME::X86_64 == PLATFORM_NAME;
	}

	// Check if arm32
	[[nodiscard]]
	constexpr auto Platform::isARM32() noexcept -> bool {
		// Check platform arm
		return ARCH_NAME::ARM == PLATFORM_NAME;
	}

	// Check if arm64
	[[nodiscard]]
	constexpr auto Platform::isARM64() noexcept -> bool {
		// Check platform arm64
		return ARCH_NAME::ARM64 == PLATFORM_NAME;
	}

	// Check if avr
	[[nodiscard]]
	constexpr auto Platform::isAVR() noexcept -> bool {
		// Check platform avr
		return ARCH_NAME::AVR == PLATFORM_NAME;
	}


}       // namespace igros::platform

