////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.cpp
//	Date:	16 Dec 2022
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

		// Calculate PIT divisor (Base PIT frequency / required frequency)
		PIT_DIVISOR	= static_cast<igros_word_t>(PIT_MAIN_FREQUENCY / frequency);
		// Save current real frequency value
		PIT_FREQUENCY	= static_cast<igros_word_t>(PIT_MAIN_FREQUENCY / PIT_DIVISOR);

		// Tell pit we want to change divisor for channel 0
		io::get().writePort8(PIT_CONTROL,	0x36_u16);
		// Set divisor (LOW first, then HIGH)
		io::get().writePort8(PIT_CHANNEL_0,	(PIT_DIVISOR & 0x00FF_u16));
		io::get().writePort8(PIT_CHANNEL_0,	(PIT_DIVISOR & 0xFF00_u16) >> 8);

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
		io::get().writePort8(PIT_CONTROL, 0x0000_u16);
		// Get number of elapsed ticks since last IRQ
		const auto loByte	{io::get().readPort8(PIT_CHANNEL_0)};
		const auto hiByte	{io::get().readPort8(PIT_CHANNEL_0)};
		// Total elapsed ticks value since IRQ
		const auto elapsed	{static_cast<igros_word_t>(hiByte << 8) | loByte};
		// Return full expired ticks count
		return PIT_TICKS * PIT_DIVISOR + elapsed;
	}


	// PIT interrupt (#0) handler
	void pitInterruptHandler(const register_t* regs) noexcept {
		// Output every N-th tick were N = frequency
		if (0_usize == (++PIT_TICKS % PIT_FREQUENCY)) [[unlikely]] {
			// Current time to HH:MM:SS.zzz
			const auto elapsed	{pitGetTicks()};
			const auto res		{klib::kudivmod(elapsed, PIT_MAIN_FREQUENCY)};
			const auto nanoseconds	{static_cast<igros_dword_t>(res.reminder)};
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
				nanoseconds
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
		irq::get().install(irq::irq_t::PIT, pitInterruptHandler);
		// Mask PIT interrupts
		irq::get().mask(irq::irq_t::PIT);

		// Print
		klib::kprintf(
			"IRQ #%d [PIT] installed\n",
			irq::irq_t::PIT
		);

	}


}	// namespace igros::arch

