////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Programmable interrupt timer
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


// IgrOS-Kernel arch
#include <arch/io.hpp>
#include <arch/irq.hpp>
#include <arch/register.hpp>
#include <arch/types.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/pit.hpp>
// IgrOS-Kernel library
#include <klib/kMath.hpp>
#include <klib/kPrint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// PIT ports
	constexpr auto PIT_CONTROL {static_cast<port_t>(0x0043_u16)};	///< Mode/command register
	constexpr auto PIT_CHANNEL_0 {static_cast<port_t>(0x0040_u16)}; ///< Channel 0 data port (system timer)
	// (channels 1 and 2 - ports 0x41 and 0x42 - are not used)


	/// @brief Timer handling IRQ 0
	constinit static pit::timer_t* pitActive {nullptr};


	// Setup timer frequency
	void pit::timer_t::setFrequency(const igros_word_t frequency) noexcept {

		// Lowest frequency which divisor fits into 16 bits (~19 Hz)
		constexpr auto PIT_MIN_FREQUENCY {static_cast<igros_word_t>((PIT_MAIN_FREQUENCY / 0xFFFF_u32) + 1_u32)};
		// Clamp requested frequency (also prevents division by zero)
		const auto     requested {(frequency < PIT_MIN_FREQUENCY) ? PIT_MIN_FREQUENCY : frequency};

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		mDivisor = static_cast<igros_word_t>(PIT_MAIN_FREQUENCY / requested);
		// Save current real frequency value
		mFrequency = static_cast<igros_word_t>(PIT_MAIN_FREQUENCY / mDivisor);

		// Channel 0, low then high byte, mode 2 (rate generator: counter runs from divisor
		// down to 1 once per period, so elapsed ticks are divisor - counter; mode 3 runs it twice)
		io::writePort8(PIT_CONTROL, 0x34_u8);
		// Set divisor (LOW first, then HIGH)
		io::writePort8(PIT_CHANNEL_0, static_cast<igros_byte_t>(mDivisor & 0x00FF_u16));
		io::writePort8(PIT_CHANNEL_0, static_cast<igros_byte_t>((static_cast<igros_dword_t>(mDivisor) >> 8) & 0x00FF_u32));

		// Print
		klib::kprintf(
			"PIT frequency:\t%d Hz\n",
			mFrequency
		);
	}

	// Get real timer frequency
	[[nodiscard]]
	auto pit::timer_t::frequency() const noexcept -> igros_word_t {
		return mFrequency;
	}

	// Get expired ticks
	[[nodiscard]]
	auto pit::timer_t::ticks() const noexcept -> igros_quad_t {
		// Send latch command for channel 0
		io::writePort8(PIT_CONTROL, 0x00_u8);
		// Get current counter value (counts down from divisor)
		const auto loByte {io::readPort8(PIT_CHANNEL_0)};
		const auto hiByte {io::readPort8(PIT_CHANNEL_0)};
		const auto counter {static_cast<igros_word_t>((static_cast<igros_dword_t>(hiByte) << 8) | loByte)};
		// Total elapsed ticks value since IRQ
		const auto elapsed {static_cast<igros_word_t>(mDivisor - counter)};
		// Return full expired ticks count
		return (static_cast<igros_quad_t>(mPeriods) * mDivisor) + elapsed;
	}


	// Timer interrupt
	void pit::timer_t::interrupt() noexcept {
		// Count every period (ticks() relies on it, regardless of output below)
		++mPeriods;
		// Output every N-th tick were N = frequency
		if ((0_u16 != mFrequency) && (0_usize == (mPeriods % mFrequency))) [[unlikely]] {
			// Current time to HH:MM:SS.zzz
			const auto elapsed {ticks()};
			const auto res {klib::kdivmod(elapsed, PIT_MAIN_FREQUENCY)};
			// Remainder is in PIT ticks (< PIT_MAIN_FREQUENCY, so * 1000 fits 32 bits)
			const auto milliseconds {static_cast<igros_dword_t>(res.remainder) * 1000_u32 / PIT_MAIN_FREQUENCY};
			const auto seconds {static_cast<igros_dword_t>(res.quotient)};
			const auto minutes {seconds / 60_u32};
			const auto hours {minutes / 60_u32};
			// Debug date/time
			klib::kprintf(
				"IRQ #%u\t[PIT]\n"
				"Time:\t%02d:%02d:%02d.%03d (~1 sec.)\n",
				static_cast<igros_dword_t>(irq_t::PIT),
				hours % 24_u32,
				minutes % 60_u32,
				seconds % 60_u32,
				milliseconds
			);
		}
	}


	/// @brief PIT interrupt (#0) handler
	/// @param regs Registers saved on interrupt (unused)
	static void pitInterruptHandler([[maybe_unused]] const register_t* regs) noexcept {
		if (nullptr != pitActive) [[likely]] {
			pitActive->interrupt();
		}
	}


	// Setup timer
	[[nodiscard]]
	auto pit::timer_t::init() noexcept -> bool {

		// Setup PIT frequency to 100 HZ
		setFrequency(PIT_DEFAULT_FREQUENCY);

		// This timer handles interrupts
		pitActive = this;
		// Install PIT interrupt handler
		irq::install<irq_t::PIT, pitInterruptHandler>();
		// Unmask PIT interrupts
		irq::unmask(irq_t::PIT);

		// Success
		return true;
	}

	// Stop timer interrupts
	// cppcheck-suppress functionConst ; changes device (hardware) state
	void pit::timer_t::deinit() noexcept {
		// Mask PIT interrupts
		irq::mask(irq_t::PIT);
		// Uninstall PIT interrupt handler
		irq::uninstall<irq_t::PIT>();
		// Timer no longer handles interrupts
		if (this == pitActive) {
			pitActive = nullptr;
		}
	}

	// Timer control
	[[nodiscard]]
	auto pit::timer_t::ioctl(const igros_dword_t request, const igros_pointer_t arg) noexcept -> bool {
		// Every request has an argument
		if (nullptr == arg) [[unlikely]] {
			return false;
		}
		switch (static_cast<IOCTL>(request)) {
			case IOCTL::SET_FREQUENCY:
				setFrequency(*static_cast<igros_word_t*>(arg));
				return true;
			case IOCTL::GET_FREQUENCY:
				*static_cast<igros_word_t*>(arg) = frequency();
				return true;
			case IOCTL::GET_TICKS:
				*static_cast<igros_quad_t*>(arg) = ticks();
				return true;
			default:
				// Unknown request
				return false;
		}
	}


} // namespace igros::arch

