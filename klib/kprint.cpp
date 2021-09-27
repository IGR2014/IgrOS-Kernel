////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.cpp
//	Date:	27 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdarg>
#include <array>

#include <drivers/vga/vmem.hpp>
#include <drivers/uart/serial.hpp>

#include <klib/kprint.hpp>
#include <klib/kstring.hpp>
#include <klib/kmath.hpp>
#include <klib/kmemory.hpp>


// Kernel library code zone
namespace igros::klib {


	// Default temporary buffer for kitoa
	constexpr std::size_t			KITOA_BUFF_LEN		{65U};
	// Constant integer symbols values buffer
	constexpr std::array<sbyte_t, 16ULL>	KITOA_CONST_BUFFER	{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


	// Kernel large unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const quad_t value, const radix_t radix) noexcept {

		// Temporary copy of value
		auto tempValue	= value;
		auto tempSize	= size;

		// Temporary buffer for value text representation
		std::array<sbyte_t, KITOA_BUFF_LEN> tempBuffer;
		// Zero-initialize
		kmemset(tempBuffer.data(), tempBuffer.size(), '\0');

		// Setup counter to last - 1 position in temporary buffer
		auto pos = KITOA_BUFF_LEN - 2U;
		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate digit index
			auto divres		= kudivmod(tempValue, static_cast<dword_t>(radix));
			// Save current digit to temporary buffer
			tempBuffer[pos--]	= KITOA_CONST_BUFFER[divres.reminder];
			// Divide value by base to remove current digit
			tempValue		= divres.quotient;
		} while (tempValue > 0U);

		// Resulting string length
		auto strLength = (KITOA_BUFF_LEN - ++pos);
		// Check size fit
		if (strLength > tempSize) {
			return buffer;
		}
		// Revert temporary buffer we created from value to src buffer
		return kstrcpy(&tempBuffer[pos], buffer, strLength);

	}

	// Kernel large integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const squad_t value, const radix_t radix) noexcept {

		// Temporary copy of value
		auto tempValue	= value;
		auto tempSize	= size;

		// Check if sign is negative and value should be represented
		// as decimal (binary, octal and hexidemical values have no sign)
		if (	(value < 0)
			&& (radix_t::DEC == radix)) {
			// Write minus sign to buffer
			*buffer++ = '-';
			// Decrement size counter
			--tempSize;
			// Made value positive
			tempValue = -tempValue;
		}

		// Temporary buffer for value text representation
		std::array<sbyte_t, KITOA_BUFF_LEN> tempBuffer;
		// Zero-initialize
		kmemset(tempBuffer.data(), tempBuffer.size(), '\0');

		// Setup counter to last - 1 position in temporary buffer
		auto pos = KITOA_BUFF_LEN - 2U;
		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate digit index
			auto divres		= kudivmod(tempValue, static_cast<dword_t>(radix));
			// Save current digit to temporary buffer
			tempBuffer[pos--]	= KITOA_CONST_BUFFER[divres.reminder];
			// Divide value by base to remove current digit
			tempValue		= divres.quotient;
		} while (tempValue > 0);

		// Resulting string length
		auto strLength = (KITOA_BUFF_LEN - ++pos);
		// Check size fit
		if (strLength > tempSize) {
			return buffer;
		}
		// Revert temporary buffer we created from value to src buffer
		return kstrcpy(&tempBuffer[pos], buffer, strLength);

	}


	// Kernel size type to string function
	sbyte_t* kstoa(sbyte_t* buffer, const std::size_t size, const std::size_t value, const radix_t radix) noexcept {
		return kitoa(buffer, size, static_cast<quad_t>(value), radix);
	}


	// Kernel vsnprintf function
	void kvsnprintf(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, va_list list) noexcept {

		// Foramt string iterator
		auto fmtIterator = 0ULL;
		// Resulting string iterator
		auto strIterator = buffer;

		// String pointer holder
		auto str = static_cast<sbyte_t*>(nullptr);

		// Number holder
		std::array<sbyte_t, KITOA_BUFF_LEN> number;
		// Zero-initialize
		kmemset(number.data(), number.size(), '\0');

		// Preceding char fill lambda
		constexpr auto fillPreceding = [](auto &str, const auto len, const auto width, const auto fill) constexpr noexcept {
			// Check if value should be extended with fillChar
			if (len < width) {
				// Calc remaining length
				auto sz = width - len;
				// Copy string
				kmemset(str, sz, static_cast<byte_t>(fill));
				// Move iterator to string's end
				str += sz;
			}
		};

		// Argument type enumeration
		enum class argType_t : byte_t {
			BYTE	= 0x01,
			WORD	= 0x02,
			DWORD	= 0x04,
			QUAD	= 0x08
		};

		// Integer print lambda
		auto printInteger = [&list, &fillPreceding](auto &str, const auto radix, const auto type, const auto width, const auto fill, const auto sign) noexcept {
			// Number holder
			std::array<sbyte_t, KITOA_BUFF_LEN> number;
			// Zero-initialize
			kmemset(number.data(), number.size(), '\0');
			// Value
			auto value	= 0ULL;
			// Check argument size specifier
			switch (type) {
				// Convert quad word to string
				case argType_t::QUAD:
					// Signed or unsigned
					value = (sign) ? va_arg(list, squad_t) : va_arg(list, quad_t);
					break;
				// Convert word to string
				case argType_t::WORD:
					// Signed or unsigned
					value = (sign) ? static_cast<sword_t>(va_arg(list, sdword_t)) : static_cast<word_t>(va_arg(list, dword_t));
					break;
				// Convert byte to string
				case argType_t::BYTE:
					// Signed or unsigned
					value = (sign) ? static_cast<sbyte_t>(va_arg(list, sdword_t)) : static_cast<byte_t>(va_arg(list, dword_t));
					break;
				// Convert double word to string
				case argType_t::DWORD:
					// Signed or unsigned
					value = (sign) ? va_arg(list, sdword_t) : va_arg(list, dword_t);
					break;
				// Unknown
				default:
					break;
			}
			// Get string length
			const auto len = kstrlen(kitoa(number.data(), number.size(), value, radix));
			// Fill with preceding symbols
			fillPreceding(str, len, width, fill);
			// Copy string
			kstrcpy(number.data(), str, len);
			// Move iterator to string's end
			str += len;
		};

		// Iterate through format string
		while (
			(fmtIterator < size)		&&
			('\0' != format[fmtIterator])
		) {

			// If symbol is not placeholder symbol '%'
			if ('%' != format[fmtIterator]) {
				// Copy string till the next placeholder symbol '%'
				*strIterator++ = format[fmtIterator++];
			// Placeholder symbol '%' received
			} else {

				// Fill character
				auto fillChar	= ' ';
				auto fillWidth	= 0U;
				// Check format fill char
				if ('0' == format[fmtIterator + 1U]) {
					// Set fillchar to '0'
					fillChar = '0';
					// Adjust format iterator
					++fmtIterator;
				}

				// Check format fill width
				if (
					'1'	<= format[fmtIterator + 1U]	&&
					'9'	>= format[fmtIterator + 1U]
				) {
					// Set fill width to value
					fillWidth = static_cast<dword_t>(format[fmtIterator + 1U] - '0');
					// Adjust format iterator
					++fmtIterator;
				}

				// Argument type (dword by default)
				auto argType = argType_t::DWORD;
				// Check if quad specifier
				if ('l' == format[fmtIterator + 1U]) {
					// Check if quad specifier
					if ('l' == format[fmtIterator + 2U]) {
						// Quad argument
						argType = argType_t::QUAD;
						// Adjust format iterator
						fmtIterator += 2U;
					}
				// Otherwise it could be word
				} else if ('h' == format[fmtIterator + 1U]) {
					// Or even byte
					if ('h' == format[fmtIterator + 2U]) {
						// Byte argument
						argType = argType_t::BYTE;
						// Adjust format iterator
						fmtIterator += 2U;
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
						*strIterator++ = format[fmtIterator];
						break;

					// Character
					case 'c':
						// Fill with preceding symbols
						fillPreceding(strIterator, sizeof(sbyte_t), fillWidth, fillChar);
						// Copy character to resulting string
						*strIterator++ = static_cast<sbyte_t>(va_arg(list, dword_t));
						break;

					// Binary integer
					case 'b':
						// Print integer
						printInteger(strIterator, radix_t::BIN, argType, fillWidth, fillChar, false);
						break;

					// Octal integer
					case 'o':
						// Print integer
						printInteger(strIterator, radix_t::OCT, argType, fillWidth, fillChar, false);
						break;

					// Integer
					case 'd':
					[[fallthrough]];
					// Integer too
					case 'i':
						// Print integer
						printInteger(strIterator, radix_t::DEC, argType, fillWidth, fillChar, true);
						break;

					// Unsigned integer
					case 'u':
						// Print integer
						printInteger(strIterator, radix_t::DEC, argType, fillWidth, fillChar, false);
						break;

					// Hexidemical integer
					case 'x':
						// Print integer
						printInteger(strIterator, radix_t::HEX, argType, fillWidth, fillChar, false);
						break;

					// Address
					case 'p': {
						// For pointers width is always size of pointer_t (aka void*) on current platform
						// And fill char is always '0'
						fillWidth	= sizeof(pointer_t) << 1;
						fillChar	= '0';
						// Convert pointer to string
						kptoa(number.data(), (sizeof(pointer_t) << 3) + 1ULL, va_arg(list, pointer_t));
						// Get string length
						const auto len = kstrlen(number.data());
						// Fill with preceding symbols
						fillPreceding(strIterator, len, fillWidth, fillChar);
						// Copy string
						kstrcpy(number.data(), strIterator, len);
						// Move iterator to string's end
						strIterator += len;
					} break;

					// Size
					case 'z': {
						// Convert pointer to string
						kstoa(number.data(), (sizeof(std::size_t) << 3) + 1ULL, va_arg(list, std::size_t));
						// Get string length
						const auto len = kstrlen(number.data());
						// Fill with preceding symbols
						fillPreceding(strIterator, len, fillWidth, fillChar);
						// Copy string
						kstrcpy(number.data(), strIterator, len);
						// Move iterator to string's end
						strIterator += len;
					} break;

					// String
					case 's': {
						// Get string from args
						str = va_arg(list, sbyte_t*);
						// Get string length
						const auto len = kstrlen(str);
						// Copy string
						kstrcpy(str, strIterator, len);
						// Move iterator to string's end
						strIterator += len;
					} break;

					// Default action
					default:
						// Copy character to resulting string
						*strIterator++ = '?';
						break;

				}

				// Incremet format iterator
				++fmtIterator;

				// Reset fill char
				fillChar	= ' ';
				fillWidth	= 0U;

			}

		}

		// Insert null terminator
		*strIterator++ = '\0';

	}


	// Kernel snprintf function
	void ksnprintf(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, ...) noexcept {
		// Kernel variadic argument list
		va_list list {};
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		kvsnprintf(buffer, size, format, list);
		// End variadic arguments list
		va_end(list);
	}

	// Kernel sprintf function
	void ksprintf(sbyte_t* buffer, const sbyte_t* format, ...) noexcept {
		// Kernel variadic argument list
		va_list list {};
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		kvsnprintf(buffer, 1024ULL, format, list);
		// End variadic arguments list
		va_end(list);
	}


	// Kernel printf function
	void kprintf(const sbyte_t* format, ...) noexcept {
		// Text buffer
		static std::array<sbyte_t, 1024ULL> buffer;
		// Zero-initialize
		kmemset(buffer.data(), buffer.size(), '\0');
		// Kernel variadic argument list
		va_list list {};
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		kvsnprintf(buffer.data(), buffer.size(), format, list);
		// End variadic arguments list
		va_end(list);
		// Output buffer
		arch::vmemWrite(buffer.data());
		arch::vmemWrite("\r\n");
		// Output to serial
		arch::serialWrite(buffer.data());
		arch::serialWrite("\r\n");
	}


}	// namespace igros::klib

