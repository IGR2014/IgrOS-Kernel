////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.cpp
//	Date:	17 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <cstdarg>
// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>
#include <klib/kstring.hpp>
#include <klib/kmath.hpp>
#include <klib/kmemory.hpp>


// Kernel library code zone
namespace igros::klib {


	// Kernel vsnprintf function
	void kvsnprintf(char* const buffer, const igros_usize_t size, const char* const format, std::va_list list) noexcept {

		// Error checks
		if ((nullptr == buffer) || (0_usize == size) || (nullptr == format)) [[unlikely]] {
			return;
		}

		// Zero-initialize
		kmemset(buffer, size, '\0');

		// Foramt string iterator
		auto fmtIterator	{0_usize};
		// Resulting string iterator
		auto strIterator	{buffer};

		// String pointer holder
		auto str		{static_cast<char*>(nullptr)};

		// Preceding char fill lambda
		constexpr auto fillPreceding = [](auto &str, const auto len, const auto width, const auto fill) constexpr noexcept {
			// Check if value should be extended with fillChar
			if (std::cmp_less(len, width)) [[likely]] {
				// Calc remaining length
				auto sz {width - len};
				// Copy string
				kmemset(str, sz, static_cast<igros_byte_t>(fill));
				// Move iterator to string's end
				str += sz;
			}
		};

		// Argument type enumeration
		enum class argType_t : igros_byte_t {
			BYTE	= 0x01_u8,
			WORD	= 0x02_u8,
			DWORD	= 0x04_u8,
			QUAD	= 0x08_u8,
			SIZE	= 0x10_u8,
			PNTR	= 0x80_u8
		};

		// Integer print lambda
		auto printInteger = [&list, &fillPreceding](auto &str, const auto radix, const auto type, const auto width, const auto fill, const auto sign) constexpr noexcept {
			// Default temporary buffer for kitoa
			constexpr auto KITOA_BUFF_LEN {65_usize};
			// Number holder
			auto number {std::array<char, KITOA_BUFF_LEN> {}};
			// Output length
			auto length {0_usize};
			// Check argument size specifier
			switch (type) {
				// Convert size to string
				case argType_t::PNTR:
					// Print signed or unsigned 64-bit value and get length of resulting string
					length = kstrlen(
						kptoa(
							number.data(),
							number.size(),
							va_arg(list, igros_pointer_t)
						)
					);
					// Done
					break;
				// Convert size to string
				case argType_t::SIZE:
					// Print signed or unsigned 64-bit value and get length of resulting string
					length = kstrlen(
						kitoa(
							number.data(),
							number.size(),
							va_arg(list, igros_usize_t),
							radix
						)
					);
					// Done
					break;
				// Convert quad word to string
				case argType_t::QUAD:
					// Print signed or unsigned 64-bit value and get length of resulting string
					length = kstrlen(
						sign ?
						kitoa(
							number.data(),
							number.size(),
							va_arg(list, igros_squad_t),
							radix
						) :
						kitoa(
							number.data(),
							number.size(),
							va_arg(list, igros_quad_t),
							radix
						)
					);
					// Done
					break;
				// Convert double word to string
				case argType_t::DWORD:
					// Print signed or unsigned 32-bit value and get length of resulting string
					length = kstrlen(
						sign ?
						kitoa(
							number.data(),
							number.size(),
							va_arg(list, igros_sdword_t),
							radix
						) :
						kitoa(
							number.data(),
							number.size(),
							va_arg(list, igros_dword_t),
							radix
						)
					);
					// Done
					break;
				// Convert word to string
				case argType_t::WORD:
					// Print signed or unsigned 16-bit value and get length of resulting string
					length = kstrlen(
						sign ?
						kitoa(
							number.data(),
							number.size(),
							static_cast<igros_sword_t>(va_arg(list, igros_sdword_t)),
							radix
						) :
						kitoa(
							number.data(),
							number.size(),
							static_cast<igros_word_t>(va_arg(list, igros_dword_t)),
							radix
						)
					);
					// Done
					break;
				// Convert byte to string
				case argType_t::BYTE:
					// Print signed or unsigned 8-bit value and get length of resulting string
					length = kstrlen(
						sign ?
						kitoa(
							number.data(),
							number.size(),
							static_cast<igros_sbyte_t>(va_arg(list, igros_sdword_t)),
							radix
						) :
						kitoa(
							number.data(),
							number.size(),
							static_cast<igros_byte_t>(va_arg(list, igros_dword_t)),
							radix
						)
					);
					// Done
					break;
				// Unknown
				default:
					// Done
					break;
			}
			// Fill with preceding symbols
			fillPreceding(str, length, width, fill);
			// Copy string
			kstrcpy(number.data(), str, length);
			// Move iterator to string's end
			str += length;
		};

		// Iterate through format string
		while (
			(std::cmp_less(fmtIterator, size))	&&
			('\0' != format[fmtIterator])
		) {

			// If symbol is not placeholder symbol '%'
			if ('%' != format[fmtIterator]) {
				// Copy string till the next placeholder symbol '%'
				*(strIterator++) = format[fmtIterator++];
			// Placeholder symbol '%' received
			} else {

				// Fill character
				auto fillChar	{' '};
				auto fillWidth	{0_usize};
				// Check format fill char
				if ('0' == format[fmtIterator + 1_usize]) {
					// Set fillchar to '0'
					fillChar = '0';
					// Adjust format iterator
					++fmtIterator;
				}

				// Check format fill width
				if (
					'1'	<= format[fmtIterator + 1_usize]	&&
					'9'	>= format[fmtIterator + 1_usize]
				) {
					// Set fill width to value
					fillWidth = static_cast<igros_dword_t>(format[fmtIterator + 1_usize] - '0');
					// Adjust format iterator
					++fmtIterator;
				}

				// Argument type (dword by default)
				auto argType {argType_t::DWORD};
				// Check if quad specifier
				if ('l' == format[fmtIterator + 1_usize]) {
					// Check if quad specifier
					if ('l' == format[fmtIterator + 2_usize]) {
						// Quad argument
						argType = argType_t::QUAD;
						// Adjust format iterator
						fmtIterator += 2_usize;
					}
				// Otherwise it could be word
				} else if ('h' == format[fmtIterator + 1_usize]) {
					// Or even byte
					if ('h' == format[fmtIterator + 2_usize]) {
						// Byte argument
						argType = argType_t::BYTE;
						// Adjust format iterator
						fmtIterator += 2_usize;
					} else {
						// Word argument
						argType = argType_t::WORD;
						// Adjust format iterator
						++fmtIterator;
					}
				// Otherwise it's double word
				}

				// Determine type
				switch (format[++fmtIterator]) {

					// '%' character
					case '%':
						// Copy placeholder symbol '%'
						*(strIterator++) = format[fmtIterator];
						// Done
						break;

					// Character
					case 'c':
						// Fill with preceding symbols
						fillPreceding(strIterator, sizeof(igros_sbyte_t), fillWidth, fillChar);
						// Copy character to resulting string
						*(strIterator++) = static_cast<igros_sbyte_t>(va_arg(list, igros_dword_t));
						// Done
						break;

					// Binary integer
					case 'b':
						// Print integer
						printInteger(strIterator, radix_t::BIN, argType, fillWidth, fillChar, false);
						// Done
						break;

					// Octal integer
					case 'o':
						// Print integer
						printInteger(strIterator, radix_t::OCT, argType, fillWidth, fillChar, false);
						// Done
						break;

					// Integer
					case 'd':
					[[fallthrough]];
					// Integer too
					case 'i':
						// Print integer
						printInteger(strIterator, radix_t::DEC, argType, fillWidth, fillChar, true);
						// Done
						break;

					// Unsigned integer
					case 'u':
						// Print integer
						printInteger(strIterator, radix_t::DEC, argType, fillWidth, fillChar, false);
						// Done
						break;

					// Hexidemical integer
					case 'x':
						// Print integer
						printInteger(strIterator, radix_t::HEX, argType, fillWidth, fillChar, false);
						// Done
						break;

					// Address
					case 'p':
						// Print integer
						printInteger(strIterator, radix_t::HEX, argType_t::PNTR, (sizeof(igros_pointer_t) << 1), '0', false);
						// Done
						break;

					// Size
					case 'z':
						// Print integer
						printInteger(strIterator, radix_t::DEC, argType_t::SIZE, fillWidth, fillChar, false);
						// Done
						break;

					// String
					case 's': {
						// Get string from args
						str = va_arg(list, char*);
						// Get string length
						const auto len {kstrlen(str)};
						// Copy string
						kstrcpy(str, strIterator, len);
						// Move iterator to string's end
						strIterator += len;
					// Done
					} break;

					// Default action
					default:
						// Copy character to resulting string
						*(strIterator++) = '?';
						// Done
						break;

				}

				// Incremet format iterator
				++fmtIterator;

			}

		}

		// Insert null terminator
		*(strIterator++) = '\0';

	}


	// Kernel snprintf function
	void ksnprintf(char* const buffer, const igros_usize_t size, const char* const format, ...) noexcept {
		// Kernel variadic argument list
		std::va_list list {};
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		kvsnprintf(buffer, size, format, list);
		// End variadic arguments list
		va_end(list);
	}

	// Kernel sprintf function
	void ksprintf(char* const buffer, const char* const format, ...) noexcept {
		// Kernel variadic argument list
		std::va_list list {};
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		kvsnprintf(buffer, 1024_usize, format, list);
		// End variadic arguments list
		va_end(list);
	}


	// Kernel printf function
	void kprintf(const char* const format, ...) noexcept {
		// Text buffer
		auto buffer {std::array<char, 1024_usize> {}};
		// Kernel variadic argument list
		std::va_list list {};
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		kvsnprintf(buffer.data(), buffer.size(), format, list);
		// End variadic arguments list
		va_end(list);
		// Output buffer
		arch::vmemWrite(buffer.data());
		arch::vmemWrite("\n");
		// Output to serial
		arch::serialWrite(buffer.data());
		arch::serialWrite("\n");
	}


}	// namespace igros::klib

