////////////////////////////////////////////////////////////////
//
//	UART driver
//
//	File:	serial.cpp
//	Date:	08 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <array>

#include <arch/io.hpp>
#include <arch/irq.hpp>

#include <klib/kmemory.hpp>
#include <klib/kprint.hpp>
#include <klib/kstring.hpp>

#include <drivers/vga/vmem.hpp>
#include <drivers/uart/serial.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Serial ports
	constexpr auto SERIAL_PORT_1	= static_cast<io::port_t>(0x03F8);
	constexpr auto SERIAL_PORT_2	= static_cast<io::port_t>(0x02F8);
	constexpr auto SERIAL_PORT_3	= static_cast<io::port_t>(0x03E8);
	constexpr auto SERIAL_PORT_4	= static_cast<io::port_t>(0x02E8);

	// Serial port data register
	constexpr auto SERIAL_PORT_DR(const io::port_t port) noexcept {
		return port;
	}

	// Serial port interrupt enable register
	constexpr auto SERIAL_PORT_IER(const io::port_t port) noexcept {
		return port + 1U;
	}

	// Serial port interrupt identification and FIFO register
	constexpr auto SERIAL_PORT_IIR(const io::port_t port) noexcept {
		return port + 2U;
	}

	// Serial port line control register
	constexpr auto SERIAL_PORT_LCR(const io::port_t port) noexcept {
		return port + 3U;
	}

	// Serial port modem control register
	constexpr auto SERIAL_PORT_MCR(const io::port_t port) noexcept {
		return port + 4U;
	}

	// Serial port line status control register
	constexpr auto SERIAL_PORT_LSR(const io::port_t port) noexcept {
		return port + 5U;
	}

	// Serial port modem status control register
	constexpr auto SERIAL_PORT_MSR(const io::port_t port) noexcept {
		return port + 6U;
	}

	// Serial port scratch register
	constexpr auto SERIAL_PORT_SR(const io::port_t port) noexcept {
		return port + 7U;
	}


	// Initialize serial port
	[[nodiscard]]
	bool serialInit(const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept {
		
		// Calculate BAUD rate
		const auto rate	= static_cast<word_t>(115200 / static_cast<dword_t>(baudRate));
		// LCR value
		const auto lcr	= (static_cast<byte_t>(dataSize)	& 0x03)
				| ((static_cast<byte_t>(stopBits)	& 0x01) << 2)
				| ((static_cast<byte_t>(parity)		& 0x03) << 3);
		
		// Disable SERIAL interrupts
		io::get().writePort8(SERIAL_PORT_IER(SERIAL_PORT_1), 0x00);
		// Set BAUD rate
		io::get().writePort8(SERIAL_PORT_LCR(SERIAL_PORT_1), 0x80);
		// Write BAUD rate low byte
		io::get().writePort8(SERIAL_PORT_DR(SERIAL_PORT_1), rate & 0x00FF);
		// Write BAUD rate high byte
		io::get().writePort8(SERIAL_PORT_IER(SERIAL_PORT_1), (rate >> 8) & 0x00FF);
		// Write LCR params
		io::get().writePort8(SERIAL_PORT_LCR(SERIAL_PORT_1), lcr);
		// Enable FIFO, clear them with 14-byte threshold
		io::get().writePort8(SERIAL_PORT_IIR(SERIAL_PORT_1), 0xC7);
		// IRQs enabled, RTS/DSR set
		io::get().writePort8(SERIAL_PORT_MCR(SERIAL_PORT_1), 0x0B);
		// Set loopback mode, test the serial chip
		io::get().writePort8(SERIAL_PORT_MCR(SERIAL_PORT_1), 0x1E);
		// Test port with 0xA5 byte
		io::get().writePort8(SERIAL_PORT_DR(SERIAL_PORT_1), 0xA5);

		// Check loopback
		if (0xA5 != io::get().readPort8(SERIAL_PORT_DR(SERIAL_PORT_1))) {
			// Debug
			klib::kprintf(
				"Serial Port #1:\t ERROR - not functional!\r\n"
			);
			// Could not setup serial port
			return false;
		}

		// Set normal mode (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
		io::get().writePort8(SERIAL_PORT_MCR(SERIAL_PORT_1), 0x0F);

		// Debug
		klib::kprintf(
			u8"Serial Port #1:\t%d %d%c%d\r\n",
			static_cast<dword_t>(baudRate),
			static_cast<dword_t>(dataSize) + 5U,
			(parity == PARITY::NONE) ? u8'N' : u8'?',
			(static_cast<dword_t>(stopBits) & 0x01) + 1U
		);

		// Success
		return true;

	}


	// Is write ready?
	[[nodiscard]]
	bool serialReadyWrite() noexcept {
		return 0x01 == (io::get().readPort8(SERIAL_PORT_LSR(SERIAL_PORT_1)) & 0x01);
	}

	// Is read ready?
	[[nodiscard]]
	bool serialReadyRead() noexcept {
		return 0x20 == (io::get().readPort8(SERIAL_PORT_LSR(SERIAL_PORT_1)) & 0x20);
	}


	// Serial write
	[[nodiscard]]
	std::size_t serialWrite(const byte_t* const src, const std::size_t size) noexcept {
		// Wait for write ready
		while (!serialReadyWrite()) {};
		// Writed size
		auto i = 0ULL;
		// Write data
		for (;(i < size) && serialReadyWrite(); ++i) {
			// One-by-one
			io::get().writePort8(SERIAL_PORT_DR(SERIAL_PORT_1), src[i]);
		}
		// Return written size
		return i;
	}

	// Serial write
	[[nodiscard]]
	std::size_t serialWrite(const sbyte_t* const src, const std::size_t size) noexcept {
		return serialWrite(reinterpret_cast<const byte_t* const>(src), size);
	}

	// Serial write
	[[nodiscard]]
	std::size_t serialWrite(const sbyte_t* const src) noexcept {
		return serialWrite(src, klib::kstrlen(src));
	}


	// Serial read
	[[nodiscard]]
	std::size_t serialRead(byte_t* const src, const std::size_t size) noexcept {
		// Wait for read ready
		while (!serialReadyRead()) {};
		// Readed size
		auto i = 0ULL;
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
		if (regs->number == static_cast<dword_t>(irq::irq_t::UART2)) {
			// Serial #2 | #4
			// Debug data
			klib::kprintf(
				u8"IRQ #%d\t[UART2]\r\n"
				u8"Read:\tNOTHING!\r\n"
			);
			// Interrupt done
			//irq::get().eoi(static_cast<irq::irq_t>(regs->number));
		} else if (regs->number == static_cast<dword_t>(irq::irq_t::UART1)) {
			// Serial #1 | #3
			std::array<byte_t, 128ULL> data;
			// Zero out
			klib::kmemset(data.data(), data.size(), 0x00);
			// Read from UART1
			const auto read = serialRead(data.data(), data.size());
			// Debug data
			klib::kprintf(
				u8"IRQ #%d\t[UART1]\r\n"
				u8"Read:\t%05d bytes = %s\r\n",
				read,
				reinterpret_cast<const sbyte_t* const>(data.cbegin())
			);
			// Interrupt done
			//irq::get().eoi(static_cast<irq::irq_t>(regs->number));
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
		irq::get().install(irq::irq_t::UART1, serialInterruptHandler);
		// Mask UART1 interrupts
		irq::get().mask(irq::irq_t::UART1);

		// Install UART2 interrupt handler
		irq::get().install(irq::irq_t::UART2, serialInterruptHandler);
		// Mask UART2 interrupts
		irq::get().mask(irq::irq_t::UART2);

		// Debug
		klib::kprintf(
			u8"IRQ #%d [UART1] installed\r\n"
			u8"IRQ #%d [UART2] installed\r\n",
			irq::irq_t::UART1,
			irq::irq_t::UART2
		);

		// Test serial
		const auto res = serialWrite(u8"Hello World\r\n");
		// Error check
		if (res != klib::kstrlen(u8"Hello World\r\n")) {
			// Debug
			klib::kprintf(
				u8"Serial Port #1:\tERROR - bad write (%d of %d bytes)",
				res,
				klib::kstrlen(u8"Hello World\r\n")
			);
		}

	}

}	// namespace igros::arch

