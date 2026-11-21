////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		UART driver
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <cstddef>
#include <span>
#include <string_view>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel drivers
#include <drivers/driver.hpp>


// Arch-dependent code zone
namespace igros::arch {


	/// @brief Serial ports (standard PC I/O bases)
	enum class SERIAL_PORT : igros_word_t {
		COM1 = 0x03F8_u16,
		COM2 = 0x02F8_u16,
		COM3 = 0x03E8_u16,
		COM4 = 0x02E8_u16,
	};

	/// @brief Baud rates
	enum class BAUD_RATE : igros_dword_t {
		BAUD_110    = 110_u32,
		BAUD_300    = 300_u32,
		BAUD_600    = 600_u32,
		BAUD_1200   = 1200_u32,
		BAUD_2400   = 2400_u32,
		BAUD_4800   = 4800_u32,
		BAUD_9600   = 9600_u32,
		BAUD_14400  = 14400_u32,
		BAUD_19200  = 19200_u32,
		BAUD_38400  = 38400_u32,
		BAUD_57600  = 57600_u32,
		BAUD_115200 = 115200_u32,
	};

	/// @brief Data size
	enum class DATA_SIZE : igros_byte_t {
		CHAR_5 = 0x00,
		CHAR_6 = 0x01,
		CHAR_7 = 0x02,
		CHAR_8 = 0x03,
	};

	/// @brief Stop bits
	enum class STOP_BITS : igros_byte_t {
		STOP_1	 = 0x00_u8,
		STOP_1_5 = 0x01_u8,
		STOP_2	 = 0x03_u8,
	};

	/// @brief Parity
	enum class PARITY : igros_byte_t {
		NONE  = 0x00_u8,
		ODD   = 0x01_u8,
		EVEN  = 0x02_u8,
		MARK  = 0x05_u8,
		SPACE = 0x07_u8,
	};


	/// @brief Initialize serial port
	/// @param port Serial port
	/// @param baudRate Baud rate
	/// @param dataSize Data bits
	/// @param stopBits Stop bits
	/// @param parity Parity
	/// @return true if port passed loopback test
	[[nodiscard]]
	auto serialInit(const SERIAL_PORT port, const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept -> bool;

	/// @brief Is write ready?
	/// @param port Serial port
	/// @return true if transmitter holding register is empty
	[[nodiscard]]
	auto serialReadyWrite(const SERIAL_PORT port) noexcept -> bool;
	/// @brief Is read ready?
	/// @param port Serial port
	/// @return true if received byte is available
	[[nodiscard]]
	auto serialReadyRead(const SERIAL_PORT port) noexcept -> bool;

	/// @brief Serial write
	/// @param port Serial port
	/// @param text Text
	/// @return Number of written characters
	[[maybe_unused]]
	auto serialWrite(const SERIAL_PORT port, const std::string_view text) noexcept -> igros_usize_t;
	/// @brief Serial read
	/// @param port Serial port
	/// @param buffer Output buffer
	/// @return Number of read characters
	[[maybe_unused]]
	auto serialRead(const SERIAL_PORT port, const std::span<char> buffer) noexcept -> igros_usize_t;

	/// @brief Serial write to COM1 (kernel console)
	/// @param text Text
	/// @return Number of written characters
	[[maybe_unused]]
	auto serialWrite(const std::string_view text) noexcept -> igros_usize_t;
	/// @brief Serial write to COM1 (kernel console)
	/// @param src Null-terminated string
	/// @return Number of written characters
	[[maybe_unused]]
	auto serialWrite(const char* const src) noexcept -> igros_usize_t;

	/// @brief Serial port driver (one device object per port)
	namespace serial {

		/// @brief Serial port configuration
		struct config_t {
			SERIAL_PORT port;     ///< Port
			BAUD_RATE   baudRate; ///< Baud rate
			DATA_SIZE   dataSize; ///< Data bits
			STOP_BITS   stopBits; ///< Stop bits
			PARITY	    parity;   ///< Parity
		};

		/// @brief Serial port (COM1/COM3 share IRQ 4, COM2/COM4 - IRQ 3)
		class uart_t final {

		public:

			/// @brief Serial port device
			/// @param config Port configuration
			explicit constexpr uart_t(const config_t &config) noexcept
				: mConfig {config} {}

			/// @brief Setup port with receive interrupt
			/// @return true if port is set up
			[[nodiscard]]
			auto init() noexcept -> bool;
			/// @brief Stop receive interrupt (IRQ line is masked once both its ports are stopped)
			void deinit() noexcept;
			/// @brief Read available bytes
			/// @param buffer Output buffer
			/// @return Number of read bytes
			[[nodiscard]]
			auto read(const std::span<std::byte> buffer) const noexcept -> igros_usize_t; // flawfinder: ignore (device read, bounded by span size)
			/// @brief Write bytes (new line is sent as CR LF)
			/// @param data Data
			/// @return Number of written bytes
			[[nodiscard]]
			auto write(const std::span<const std::byte> data) const noexcept -> igros_usize_t;

			/// @brief Port configuration
			/// @return Configuration
			[[nodiscard]]
			constexpr auto config() const noexcept -> const config_t & {
				return mConfig;
			}

			/// @brief Receive interrupt (prints received data)
			void interrupt() const noexcept;

		private:

			config_t mConfig; ///< Configuration
		};

		/// @brief Standard port configuration (115200 8N1)
		/// @param port Port
		/// @return Configuration
		[[nodiscard]]
		constexpr auto standard(const SERIAL_PORT port) noexcept -> config_t {
			return config_t {
				.port	  = port,
				.baudRate = BAUD_RATE::BAUD_115200,
				.dataSize = DATA_SIZE::CHAR_8,
				.stopBits = STOP_BITS::STOP_1,
				.parity	  = PARITY::NONE,
			};
		}

		/// @brief Standard ports (COM1 is kernel console)
		inline constinit uart_t com1 {standard(SERIAL_PORT::COM1)};
		inline constinit uart_t com2 {standard(SERIAL_PORT::COM2)};
		inline constinit uart_t com3 {standard(SERIAL_PORT::COM3)};
		inline constinit uart_t com4 {standard(SERIAL_PORT::COM4)};

		/// @brief Standard ports driver descriptions
		inline constexpr drivers::driver_t COM1 {drivers::make("COM1", com1)};
		inline constexpr drivers::driver_t COM2 {drivers::make("COM2", com2)};
		inline constexpr drivers::driver_t COM3 {drivers::make("COM3", com3)};
		inline constexpr drivers::driver_t COM4 {drivers::make("COM4", com4)};

	} // namespace serial


} // namespace igros::arch

