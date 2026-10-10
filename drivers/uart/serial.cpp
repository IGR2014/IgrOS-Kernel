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
// IgrOS-Kernel arch
#include <arch/io.hpp>
#include <arch/irq.hpp>
// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>
// IgrOS-Kernel library
#include <klib/kmemory.hpp>
#include <klib/kprint.hpp>
#include <klib/kstring.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Serial port registers (offsets from port base)
	enum class SERIAL_REG : igros_word_t {
		DATA		= 0x00_u16,	// Data (DLAB = 0) / divisor low byte (DLAB = 1)
		IER		= 0x01_u16,	// Interrupt enable (DLAB = 0) / divisor high byte (DLAB = 1)
		IIR		= 0x02_u16,	// Interrupt identification (read) / FIFO control (write)
		LCR		= 0x03_u16,	// Line control
		MCR		= 0x04_u16,	// Modem control
		LSR		= 0x05_u16,	// Line status
		MSR		= 0x06_u16,	// Modem status
		SCRATCH		= 0x07_u16	// Scratch
	};

	// Line status register bits
	constexpr auto SERIAL_LSR_DATA_READY	{0x01_u8};
	constexpr auto SERIAL_LSR_THR_EMPTY	{0x20_u8};

	// Interrupt enable register bits
	constexpr auto SERIAL_IER_RX		{0x01_u8};


	// Serial port register address
	[[nodiscard]]
	constexpr auto serialRegister(const SERIAL_PORT port, const SERIAL_REG reg) noexcept -> port_t {
		return static_cast<port_t>(static_cast<igros_word_t>(port) + static_cast<igros_word_t>(reg));
	}

	// Write serial port register
	static void serialRegWrite(const SERIAL_PORT port, const SERIAL_REG reg, const igros_byte_t value) noexcept {
		io::writePort8(serialRegister(port, reg), value);
	}

	// Read serial port register
	[[nodiscard]]
	static auto serialRegRead(const SERIAL_PORT port, const SERIAL_REG reg) noexcept -> igros_byte_t {
		return io::readPort8(serialRegister(port, reg));
	}


	// Initialize serial port
	[[nodiscard]]
	auto serialInit(const SERIAL_PORT port, const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept -> bool {

		// Calculate BAUD rate divisor
		const auto rate	{static_cast<igros_word_t>(115200_u32 / static_cast<igros_dword_t>(baudRate))};
		// LCR value
		const auto lcr	{static_cast<igros_byte_t>(
			((static_cast<igros_byte_t>(dataSize)	& 0x03_u8))		|
			((static_cast<igros_byte_t>(stopBits)	& 0x01_u8) << 2)	|
			((static_cast<igros_byte_t>(parity)	& 0x03_u8) << 3)
		)};

		// Disable SERIAL interrupts
		serialRegWrite(port, SERIAL_REG::IER,	0x00_u8);
		// Set DLAB to access BAUD rate divisor
		serialRegWrite(port, SERIAL_REG::LCR,	0x80_u8);
		// Write BAUD rate divisor low byte
		serialRegWrite(port, SERIAL_REG::DATA,	static_cast<igros_byte_t>(rate & 0x00FF_u16));
		// Write BAUD rate divisor high byte
		serialRegWrite(port, SERIAL_REG::IER,	static_cast<igros_byte_t>((rate >> 8) & 0x00FF_u16));
		// Write LCR params (also clears DLAB)
		serialRegWrite(port, SERIAL_REG::LCR,	lcr);
		// Enable FIFO, clear them with 14-byte threshold
		serialRegWrite(port, SERIAL_REG::IIR,	0xC7_u8);
		// IRQs enabled, RTS/DSR set
		serialRegWrite(port, SERIAL_REG::MCR,	0x0B_u8);
		// Set loopback mode, test the serial chip
		serialRegWrite(port, SERIAL_REG::MCR,	0x1E_u8);
		// Test port with 0xA5 byte
		serialRegWrite(port, SERIAL_REG::DATA,	0xA5_u8);

		// Check loopback
		if (0xA5_u8 != serialRegRead(port, SERIAL_REG::DATA)) {
			// Could not setup serial port
			return false;
		}

		// Set normal mode (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
		serialRegWrite(port, SERIAL_REG::MCR,	0x0F_u8);

		// Debug
		klib::kprintf(
			"Serial port 0x%x:\t%d %d%c%d",
			static_cast<igros_dword_t>(port),
			static_cast<igros_dword_t>(baudRate),
			static_cast<igros_dword_t>(dataSize) + 5_u32,
			(parity == PARITY::NONE) ? 'N' : '?',
			(static_cast<igros_dword_t>(stopBits) & 0x01_u32) + 1_u32
		);

		// Success
		return true;

	}


	// Is write ready? (LSR bit 5 - transmitter holding register empty)
	[[nodiscard]]
	auto serialReadyWrite(const SERIAL_PORT port) noexcept -> bool {
		return 0x00_u8 != (serialRegRead(port, SERIAL_REG::LSR) & SERIAL_LSR_THR_EMPTY);
	}

	// Is read ready? (LSR bit 0 - data ready)
	[[nodiscard]]
	auto serialReadyRead(const SERIAL_PORT port) noexcept -> bool {
		return 0x00_u8 != (serialRegRead(port, SERIAL_REG::LSR) & SERIAL_LSR_DATA_READY);
	}

	// Wait until write ready (bounded, so missing UART can't hang the kernel)
	[[nodiscard]]
	static auto serialWaitWrite(const SERIAL_PORT port) noexcept -> bool {
		for (auto spin {0_usize}; spin < 100000_usize; ++spin) {
			if (serialReadyWrite(port)) [[likely]] {
				return true;
			}
		}
		return false;
	}


	// Serial write
	[[maybe_unused]]
	auto serialWrite(const SERIAL_PORT port, const char* const src, const igros_usize_t size) noexcept -> igros_usize_t {
		// Written size
		auto i {0_usize};
		// Write data
		for (;(i < size) && serialWaitWrite(port); ++i) {
			// Check if new line
			if ('\n' == src[i]) [[unlikely]] {
				// Add CR
				serialRegWrite(port, SERIAL_REG::DATA, '\r');
				// Wait for CR to be sent
				if (!serialWaitWrite(port)) [[unlikely]] {
					break;
				}
			}
			// One-by-one
			serialRegWrite(port, SERIAL_REG::DATA, static_cast<igros_byte_t>(src[i]));
		}
		// Return written size
		return i;
	}

	// Serial read
	[[maybe_unused]]
	auto serialRead(const SERIAL_PORT port, char* const src, const igros_usize_t size) noexcept -> igros_usize_t {
		// Read size
		auto i {0_usize};
		// Read data
		for (;(i < size) && serialReadyRead(port); ++i) {
			// One-by-one
			src[i] = static_cast<char>(serialRegRead(port, SERIAL_REG::DATA));
		}
		// Return read size
		return i;
	}


	// Serial write to COM1 (kernel console)
	[[maybe_unused]]
	auto serialWrite(const char* const src, const igros_usize_t size) noexcept -> igros_usize_t {
		return serialWrite(SERIAL_PORT::COM1, src, size);
	}

	// Serial write to COM1 (kernel console)
	[[maybe_unused]]
	auto serialWrite(const char* const src) noexcept -> igros_usize_t {
		return serialWrite(SERIAL_PORT::COM1, src, klib::kstrlen(src));
	}


	// Serial #1 | #3 IRQ handler
	static void serialInterruptHandler1([[maybe_unused]] const register_t* const regs) noexcept {
		std::array<char, 128_usize> data;
		// Zero out
		klib::kmemset(data.data(), data.size(), 0x00_u8);
		// Read from COM1 (keep space for null terminator, reading also acknowledges interrupt)
		const auto read {serialRead(SERIAL_PORT::COM1, data.data(), data.size() - 1_usize)};
		// Debug data
		klib::kprintf(
			"IRQ #%u\t[UART1]\n"
			"Read:\t%05zu bytes = %s\n",
			static_cast<igros_dword_t>(irq_t::UART1),
			read,
			data.data()
		);
	}


	// Setup COM1 (115200 8N1) with receive interrupt
	[[nodiscard]]
	auto serialSetup() noexcept -> bool {

		// Init serial port
		if (!serialInit(SERIAL_PORT::COM1, BAUD_RATE::BAUD_115200, DATA_SIZE::CHAR_8, STOP_BITS::STOP_1, PARITY::NONE)) {
			// Fail
			return false;
		}

		// Install UART1 interrupt handler
		irq::install<irq_t::UART1, serialInterruptHandler1>();
		// Enable receive interrupt
		serialRegWrite(SERIAL_PORT::COM1, SERIAL_REG::IER, SERIAL_IER_RX);
		// Unmask UART1 interrupts
		irq::unmask(irq_t::UART1);

		// Success
		return true;

	}


}	// namespace igros::arch

