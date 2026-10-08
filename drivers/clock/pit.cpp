////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.cpp
//	Date:	17 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch
#include <arch/io.hpp>
#include <arch/irq.hpp>
#include <arch/register.hpp>
#include <arch/types.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/pit.hpp>
// IgrOS-Kernel library
#include <klib/kmath.hpp>
#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// PIT ports
	constexpr auto PIT_CONTROL	{static_cast<io::port_t>(0x0043_u16)};
	constexpr auto PIT_CHANNEL_0	{static_cast<io::port_t>(0x0040_u16)};
	//constexpr auto PIT_CHANNEL_1	{static_cast<io::port_t>(PIT_CHANNEL_0 + 1_u16)};
	//constexpr auto PIT_CHANNEL_2	{static_cast<io::port_t>(PIT_CHANNEL_1 + 1_u16)};


	// Ticks count
	static auto	PIT_TICKS	{0_usize};
	// Current frequency
	static auto	PIT_FREQUENCY	{0_u16};
	// Current divisor
	static auto	PIT_DIVISOR	{1_u16};


        // Setup PIT frequency
	void pitSetupFrequency(const igros_word_t frequency) noexcept {

		// Lowest frequency which divisor fits into 16 bits (~19 Hz)
		constexpr auto PIT_MIN_FREQUENCY {static_cast<igros_word_t>(PIT_MAIN_FREQUENCY / 0xFFFF_u32 + 1_u32)};
		// Clamp requested frequency (also prevents division by zero)
		const auto requested {(frequency < PIT_MIN_FREQUENCY) ? PIT_MIN_FREQUENCY : frequency};

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		PIT_DIVISOR	= static_cast<igros_word_t>(PIT_MAIN_FREQUENCY / requested);
		// Save current real frequency value
		PIT_FREQUENCY	= static_cast<igros_word_t>(PIT_MAIN_FREQUENCY / PIT_DIVISOR);

		// Tell pit we want to change divisor for channel 0
		io::get().writePort8(PIT_CONTROL,	0x36_u8);
		// Set divisor (LOW first, then HIGH)
		io::get().writePort8(PIT_CHANNEL_0,	static_cast<igros_byte_t>(PIT_DIVISOR & 0x00FF_u16));
		io::get().writePort8(PIT_CHANNEL_0,	static_cast<igros_byte_t>((PIT_DIVISOR >> 8) & 0x00FF_u16));

		// Print
		klib::kprintf(
			"REAL frequency set to: %d Hz.",
			PIT_FREQUENCY
		);

        }


	// Get expired ticks
	[[nodiscard]]
	auto pitGetTicks() noexcept -> igros_quad_t {
		// Send latch command for channel 0;
		io::get().writePort8(PIT_CONTROL, 0x00_u8);
		// Get current counter value (counts down from divisor)
		const auto loByte	{io::get().readPort8(PIT_CHANNEL_0)};
		const auto hiByte	{io::get().readPort8(PIT_CHANNEL_0)};
		const auto counter	{static_cast<igros_word_t>((hiByte << 8) | loByte)};
		// Total elapsed ticks value since IRQ
		const auto elapsed	{static_cast<igros_word_t>(PIT_DIVISOR - counter)};
		// Return full expired ticks count
		return PIT_TICKS * PIT_DIVISOR + elapsed;
	}


	// PIT interrupt (#0) handler
	static void pitInterruptHandler([[maybe_unused]] const register_t* regs) noexcept {
		// Output every N-th tick were N = frequency
		if ((0_u16 != PIT_FREQUENCY) && (0_usize == (++PIT_TICKS % PIT_FREQUENCY))) [[unlikely]] {
			// Current time to HH:MM:SS.zzz
			const auto elapsed	{pitGetTicks()};
			const auto res		{klib::kdivmod(elapsed, PIT_MAIN_FREQUENCY)};
			// Remainder is in PIT ticks (< PIT_MAIN_FREQUENCY, so * 1000 fits 32 bits)
			const auto milliseconds	{static_cast<igros_dword_t>(res.reminder) * 1000_u32 / PIT_MAIN_FREQUENCY};
			const auto seconds	{static_cast<igros_dword_t>(res.quotient)};
			const auto minutes	{seconds / 60_u32};
			const auto hours	{minutes / 60_u32};
			// Debug date/time
			klib::kprintf(
				"IRQ #%d\t[PIT]\n"
				"Time:\t%02d:%02d:%02d.%03d (~1 sec.)\n",
				irq::irq_t::PIT,
				hours	% 24_u32,
				minutes	% 60_u32,
				seconds	% 60_u32,
				milliseconds
			);
		}
		// IRQ EOI
		irq::get().eoi(static_cast<const irq::irq_t>(regs->number));
	}


	// Setup programmable interrupt timer
	void pitSetup() noexcept {

		// Setup PIT frequency to 100 HZ
		pitSetupFrequency(PIT_DEFAULT_FREQUENCY);

		// Install PIT interrupt handler
		irq::get().install<irq::irq_t::PIT, pitInterruptHandler>();
		// Mask PIT interrupts
		irq::get().mask(irq::irq_t::PIT);

	}


}	// namespace igros::arch

