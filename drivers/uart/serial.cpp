////////////////////////////////////////////////////////////////
//
//	UART driver
//
//	File:	serial.cpp
//	Date:	21 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <bit>
// IgrOS-Kernel arch
#include <arch/io.hpp>
#include <arch/irq.hpp>
// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kmemory.hpp>
#include <klib/kprint.hpp>
#include <klib/kstring.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Serial ports
	constexpr auto SERIAL_PORT_1	{static_cast<io::port_t>(0x03F8_u16)};
	//constexpr auto SERIAL_PORT_2	{static_cast<io::port_t>(0x02F8_u16)};
	//constexpr auto SERIAL_PORT_3	{static_cast<io::port_t>(0x03E8_u16)};
	//constexpr auto SERIAL_PORT_4	{static_cast<io::port_t>(0x02E8_u16)};

	// Serial port data register
	[[nodiscard]]
	constexpr auto SERIAL_PORT_DR(const io::port_t port) noexcept {
		return port;
	}

	// Serial port interrupt enable register
	[[nodiscard]]
	constexpr auto SERIAL_PORT_IER(const io::port_t port) noexcept {
		return port + 1_u16;
	}

	// Serial port interrupt identification and FIFO register
	[[nodiscard]]
	constexpr auto SERIAL_PORT_IIR(const io::port_t port) noexcept {
		return port + 2_u16;
	}

	// Serial port line control register
	[[nodiscard]]
	constexpr auto SERIAL_PORT_LCR(const io::port_t port) noexcept {
		return port + 3_u16;
	}

	// Serial port modem control register
	[[nodiscard]]
	constexpr auto SERIAL_PORT_MCR(const io::port_t port) noexcept {
		return port + 4_u16;
	}

	// Serial port line status control register
	[[nodiscard]]
	constexpr auto SERIAL_PORT_LSR(const io::port_t port) noexcept {
		return port + 5_u16;
	}

	// Serial port modem status control register
	[[nodiscard]]
	constexpr auto SERIAL_PORT_MSR(const io::port_t port) noexcept {
		return port + 6_u16;
	}

	// Serial port scratch register
	[[nodiscard]]
	constexpr auto SERIAL_PORT_SR(const io::port_t port) noexcept {
		return port + 7_u16;
	}


	// Initialize serial port
	[[nodiscard]]
	auto serialInit(const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept -> bool {
		
		// Calculate BAUD rate
		const auto rate	{static_cast<igros_word_t>(115200_u32 / static_cast<igros_dword_t>(baudRate))};
		// LCR value
		const auto lcr	{static_cast<igros_byte_t>(
			((static_cast<igros_byte_t>(dataSize)	& 0x03_u8))		|
			((static_cast<igros_byte_t>(stopBits)	& 0x01_u8) << 2)	|
			((static_cast<igros_byte_t>(parity)	& 0x03_u8) << 3)
		)};

		// Disable SERIAL interrupts
		io::get().writePort8(SERIAL_PORT_IER(SERIAL_PORT_1),	0x00_u8);
		// Set BAUD rate
		io::get().writePort8(SERIAL_PORT_LCR(SERIAL_PORT_1),	0x80_u8);
		// Write BAUD rate low byte
		io::get().writePort8(SERIAL_PORT_DR(SERIAL_PORT_1),	(rate & 0x00FF_u16));
		// Write BAUD rate high byte
		io::get().writePort8(SERIAL_PORT_IER(SERIAL_PORT_1),	(rate >> 8) & 0x00FF_u16);
		// Write LCR params
		io::get().writePort8(SERIAL_PORT_LCR(SERIAL_PORT_1),	lcr);
		// Enable FIFO, clear them with 14-byte threshold
		io::get().writePort8(SERIAL_PORT_IIR(SERIAL_PORT_1),	0xC7_u8);
		// IRQs enabled, RTS/DSR set
		io::get().writePort8(SERIAL_PORT_MCR(SERIAL_PORT_1),	0x0B_u8);
		// Set loopback mode, test the serial chip
		io::get().writePort8(SERIAL_PORT_MCR(SERIAL_PORT_1),	0x1E_u8);
		// Test port with 0xA5 byte
		io::get().writePort8(SERIAL_PORT_DR(SERIAL_PORT_1),	0xA5_u8);

		// Check loopback
		if (0xA5_u8 != io::get().readPort8(SERIAL_PORT_DR(SERIAL_PORT_1))) {
			// Debug
			klib::kprintf(
				"Serial Port #1:\t ERROR - not functional!\n"
			);
			// Could not setup serial port
			return false;
		}

		// Set normal mode (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
		io::get().writePort8(SERIAL_PORT_MCR(SERIAL_PORT_1),	0x0F_u8);

		// Debug
		klib::kprintf(
			"Serial Port #1:\t%d %d%c%d\n",
			static_cast<igros_dword_t>(baudRate),
			static_cast<igros_dword_t>(dataSize) + 5_u32,
			(parity == PARITY::NONE) ? 'N' : '?',
			(static_cast<igros_dword_t>(stopBits) & 0x01_u32) + 1_u32
		);

		// Success
		return true;

	}


	// Is write ready?
	[[nodiscard]]
	auto serialReadyWrite() noexcept -> bool {
		return 0x01_u8 == (io::get().readPort8(SERIAL_PORT_LSR(SERIAL_PORT_1)) & 0x01_u8);
	}

	// Is read ready?
	[[nodiscard]]
	auto serialReadyRead() noexcept -> bool {
		return 0x20_u8 == (io::get().readPort8(SERIAL_PORT_LSR(SERIAL_PORT_1)) & 0x20_u8);
	}


	// Serial write
	[[maybe_unused]]
	auto serialWrite(const char* const src, const igros_usize_t size) noexcept -> igros_usize_t {
		// Writed size
		auto i {0_usize};
		// Write data
		for (;(i < size) && serialReadyWrite(); ++i) {
			// Check if new line
			if ('\n' == src[i]) [[unlikely]] {
				// Add CR
				io::get().writePort8(SERIAL_PORT_DR(SERIAL_PORT_1), '\r');
			}
			// One-by-one
			io::get().writePort8(SERIAL_PORT_DR(SERIAL_PORT_1), src[i]);
		}
		// Return written size
		return i;
	}

	// Serial write
	[[maybe_unused]]
	auto serialWrite(const char* const src) noexcept -> igros_usize_t {
		return serialWrite(src, klib::kstrlen(src));
	}


	// Serial read
	[[maybe_unused]]
	auto serialRead(char* const src, const igros_usize_t size) noexcept -> igros_usize_t {
		// Readed size
		auto i {0_usize};
		// Read data
		for (;(i < size) && serialReadyRead(); ++i) {
			// One-by-one
			src[i] = io::get().readPort8(SERIAL_PORT_DR(SERIAL_PORT_1));
		}
		// Return readed size
		return i;
	}


	// Serial IRQ handler
	void serialInterruptHandler(const register_t* const regs) noexcept {
		// Check IRQ #
		if (regs->number == static_cast<igros_dword_t>(irq::irq_t::UART2)) {
			// Serial #2 | #4
			// Debug data
			klib::kprintf(
				"IRQ #%d\t[UART2]\n"
				"Read:\tNOTHING!\n"
			);
		} else if (regs->number == static_cast<igros_dword_t>(irq::irq_t::UART1)) {
			// Serial #1 | #3
			std::array<char, 128_usize> data;
			// Zero out
			klib::kmemset(data.data(), data.size(), 0x00_u8);
			// Read from UART1
			const auto read {serialRead(data.data(), data.size())};
			// Debug data
			klib::kprintf(
				"IRQ #%d\t[UART1]\n"
				"Read:\t%05d bytes = %s\n",
				read,
				std::bit_cast<const igros_sbyte_t* const>(data.cbegin())
			);
		}
	}

	// Setup serial port
	void serialSetup(const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept {

		// Init seral port
		if (!serialInit(baudRate, dataSize, stopBits, parity)) {
			// Fail
			return;
		}

		// Install UART1 interrupt handler
		irq::get().install<irq::irq_t::UART1, serialInterruptHandler>();
		// Mask UART1 interrupts
		irq::get().mask(irq::irq_t::UART1);

		// Install UART2 interrupt handler
		irq::get().install<irq::irq_t::UART2, serialInterruptHandler>();
		// Mask UART2 interrupts
		irq::get().mask(irq::irq_t::UART2);

	}

}	// namespace igros::arch

