////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.cpp
//	Date:	08 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdarg>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>
#include <klib/kstring.hpp>
#include <klib/kmath.hpp>
#include <klib/kmemory.hpp>


// Kernel library code zone
namespace igros::klib {


	// Default temporary buffer for kitoa
	constexpr std::size_t	KITOA_BUFF_LEN		= 65u;
	// Constant integer symbols values buffer
	constexpr sbyte_t	KITOA_CONST_BUFFER[]	= {u8"0123456789ABCDEF"};


	// Kernel large unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const quad_t value, const radix_t radix) noexcept {

		// Temporary copy of value
		auto tempValue	= value;
		auto tempSize	= size;

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN];
		// Zero-initialize
		kmemset(tempBuffer, u8'\0');

		// Setup counter to last - 1 position in temporary buffer
		auto pos = KITOA_BUFF_LEN - 2u;
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
		} while (tempValue > 0u);

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
			*buffer++ = u8'-';
			// Decrement size counter
			--tempSize;
			// Made value positive
			tempValue = -tempValue;
		}

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN];
		// Zero-initialize
		kmemset(tempBuffer, u8'\0');

		// Setup counter to last - 1 position in temporary buffer
		auto pos = KITOA_BUFF_LEN - 2u;
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


	// Kernel unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const dword_t value, const radix_t radix) noexcept {

		// Temporary copy of value
		auto tempValue	= value;
		auto tempSize	= size;

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN];
		// Zero-initialize
		kmemset(tempBuffer, u8'\0');

		// Setup counter to last - 1 position in temporary buffer
		auto pos = KITOA_BUFF_LEN - 2u;
		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate digit index
			auto digitIndex		= tempValue % static_cast<byte_t>(radix);
			// Calculate new value
			auto newValue		= tempValue / static_cast<byte_t>(radix);
			// Save current digit to temporary buffer
			tempBuffer[pos--]	= KITOA_CONST_BUFFER[digitIndex];
			// Divide value by base to remove current digit
			tempValue		= newValue;
		} while (tempValue > 0u);

		// Resulting string length
		auto strLength = (KITOA_BUFF_LEN - ++pos);
		// Check size fit
		if (strLength > tempSize) {
			return buffer;
		}
		// Revert temporary buffer we created from value to src buffer
		return kstrcpy(&tempBuffer[pos], buffer, strLength);

	}

	// Kernel integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const sdword_t value, const radix_t radix) noexcept {

		// Temporary copy of value
		auto tempValue	= value;
		auto tempSize	= size;

		// Check if sign is negative and value should be represented
		// as decimal (binary, octal and hexidemical values have no sign)
		if (	(value < 0)
			&& (radix_t::DEC == radix)) {
			// Write minus sign to buffer
			*buffer++ = u8'-';
			// Decrement size counter
			--tempSize;
			// Made value positive
			tempValue = -tempValue;
		}

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN];
		// Zero-initialize
		kmemset(tempBuffer, u8'\0');

		// Setup counter to last - 1 position in temporary buffer
		auto pos = KITOA_BUFF_LEN - 2u;
		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate digit index
			auto digitIndex		= tempValue % static_cast<byte_t>(radix);
			// Calculate new value
			auto newValue		= tempValue / static_cast<byte_t>(radix);
			// Save current digit to temporary buffer
			tempBuffer[pos--]	= KITOA_CONST_BUFFER[digitIndex];
			// Divide value by base to remove current digit
			tempValue		= newValue;
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

#if defined(IGROS_ARCH_i386)

		return kitoa(buffer, size, static_cast<dword_t>(value), radix);

#elif defined(IGROS_ARCH_x86_64)

		return kitoa(buffer, size, static_cast<quad_t>(value), radix);

#else

		return nullptr;

#endif

	}


	// Kernel vsnprintf function
	void kvsnprintf(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, va_list list) noexcept {

		// Foramt string iterator
		auto fmtIterator = 0ull;
		// Resulting string iterator
		auto strIterator = buffer;

		// String pointer holder
		auto str = static_cast<sbyte_t*>(nullptr);

		// Number holder
		sbyte_t number[KITOA_BUFF_LEN];
		// Zero-initialize
		kmemset(number, u8'\0');

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
			static sbyte_t number[KITOA_BUFF_LEN];
			// Zero-initialize
			kmemset(number, u8'\0');
			// Check argument size specifier
			switch (type) {
				// Convert quad word to string
				case argType_t::QUAD:
					// Signed or unsigned
					if (sign) {
						kitoa(number, (sizeof(quad_t) << 3) + 1ull, static_cast<squad_t>(va_arg(list, squad_t)), radix);
					} else {
						kitoa(number, (sizeof(quad_t) << 3) + 1ull, static_cast<quad_t>(va_arg(list, quad_t)), radix);
					}
					break;
				// Convert word to string
				case argType_t::WORD:
					// Signed or unsigned
					if (sign) {
						kitoa(number, (sizeof(word_t) << 1) + 1ull, static_cast<sword_t>(va_arg(list, sdword_t)), radix);
					} else {
						kitoa(number, (sizeof(word_t) << 1) + 1ull, static_cast<word_t>(va_arg(list, dword_t)), radix);
					}
					break;
				// Convert byte to string
				case argType_t::BYTE:
					// Signed or unsigned
					if (sign) {
						kitoa(number, sizeof(byte_t) + 1ull, static_cast<sbyte_t>(va_arg(list, sdword_t)), radix);
					} else {
						kitoa(number, sizeof(byte_t) + 1ull, static_cast<byte_t>(va_arg(list, dword_t)), radix);
					}
					break;
				// Convert double word to string
				case argType_t::DWORD:
					// Signed or unsigned
					if (sign) {
						kitoa(number, (sizeof(dword_t) << 2) + 1ull, static_cast<sdword_t>(va_arg(list, sdword_t)), radix);
					} else {
						kitoa(number, (sizeof(dword_t) << 2) + 1ull, static_cast<dword_t>(va_arg(list, dword_t)), radix);
					}
					break;
				// Unknown
				default:
					break;
			}
			// Get string length
			auto len = kstrlen(number);
			// Fill with preceding symbols
			fillPreceding(str, len, width, fill);
			// Copy string
			kstrcpy(number, str, len);
			// Move iterator to string's end
			str += len;
		};

		// Iterate through format string
		while (	(fmtIterator < size)
			&& (u8'\0' != format[fmtIterator])) {

			// If symbol is not placeholder symbol '%'
			if (u8'%' != format[fmtIterator]) {
				// Copy string till the next placeholder symbol '%'
				*strIterator++ = format[fmtIterator++];
			// Placeholder symbol '%' received
			} else {

				// Fill character
				auto fillChar	= u8' ';
				auto fillWidth	= 0u;
				// Check format fill char
				if (u8'0' == format[fmtIterator + 1u]) {
					// Set fillchar to '0'
					fillChar = u8'0';
					// Adjust format iterator
					++fmtIterator;
				}

				// Check format fill width
				if (u8'1'	<= format[fmtIterator + 1u]
				    && u8'9'	>= format[fmtIterator + 1u]) {
					// Set fill width to value
					fillWidth = static_cast<dword_t>(format[fmtIterator + 1u] - u8'0');
					// Adjust format iterator
					++fmtIterator;
				}

				// Argument type (dword by default)
				auto argType = argType_t::DWORD;
				// Check if quad specifier
				if (u8'l' == format[fmtIterator + 1u]) {
					// Check if quad specifier
					if (u8'l' == format[fmtIterator + 2u]) {
						// Quad argument
						argType = argType_t::QUAD;
						// Adjust format iterator
						fmtIterator += 2u;
					}
				// Otherwise it could be word
				} else if (u8'h' == format[fmtIterator + 1u]) {
					// Or even byte
					if (u8'h' == format[fmtIterator + 2u]) {
						// Byte argument
						argType = argType_t::BYTE;
						// Adjust format iterator
						fmtIterator += 2u;
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
					case u8'%':
						// Copy placeholder symbol '%'
						*strIterator++ = format[fmtIterator];
						break;

					// Character
					case u8'c':
						// Copy character to resulting string
						*strIterator++ = static_cast<sbyte_t>(va_arg(list, dword_t));
						break;

					// Binary integer
					case u8'b':
						// Print integer
						printInteger(strIterator, radix_t::BIN, argType, fillWidth, fillChar, false);
						break;

					// Octal integer
					case u8'o':
						// Print integer
						printInteger(strIterator, radix_t::OCT, argType, fillWidth, fillChar, false);
						break;

					// Integer
					case u8'd':
					[[fallthrough]];
					// Integer too
					case u8'i':
						// Print integer
						printInteger(strIterator, radix_t::DEC, argType, fillWidth, fillChar, true);
						break;

					// Unsigned integer
					case u8'u':
						// Print integer
						printInteger(strIterator, radix_t::DEC, argType, fillWidth, fillChar, false);
						break;

					// Hexidemical integer
					case u8'x':
						// Print integer
						printInteger(strIterator, radix_t::HEX, argType, fillWidth, fillChar, false);
						break;

					// Address
					case u8'p': {
						// For pointers width is always size of pointer_t (aka void*) on current platform
						// And fill char is always '0'
						fillWidth	= sizeof(pointer_t) << 1;
						fillChar	= u8'0';
						// Convert pointer to string
						kptoa(number, (sizeof(pointer_t) << 3) + 1ull, static_cast<pointer_t>(va_arg(list, pointer_t)));
						// Get string length
						auto len = kstrlen(number);
						// Fill with preceding symbols
						fillPreceding(strIterator, len, fillWidth, fillChar);
						// Copy string
						kstrcpy(number, strIterator, len);
						// Move iterator to string's end
						strIterator += len;
					} break;

					// Size
					case u8'z': {
						// Convert pointer to string
						kstoa(number, (sizeof(std::size_t) << 3) + 1ull, static_cast<const std::size_t>(va_arg(list, std::size_t)));
						// Get string length
						auto len = kstrlen(number);
						// Fill with preceding symbols
						fillPreceding(strIterator, len, fillWidth, fillChar);
						// Copy string
						kstrcpy(number, strIterator, len);
						// Move iterator to string's end
						strIterator += len;
					} break;

					// String
					case u8's': {
						// Get string from args
						str = static_cast<sbyte_t*>(va_arg(list, sbyte_t*));
						// Get string length
						auto len = kstrlen(str);
						// Copy string
						kstrcpy(str, strIterator, len);
						// Move iterator to string's end
						strIterator += len;
					} break;

					// Default action
					default:
						// Copy character to resulting string
						*strIterator++ = u8'?';
						break;

				}

				// Incremet format iterator
				++fmtIterator;

				// Reset fill char
				fillChar	= u8' ';
				fillWidth	= 0u;

			}

		}

		// Insert null terminator
		*strIterator++ = u8'\0';

	}


	// Kernel snprintf function
	void ksnprintf(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, ...) noexcept {
		// Kernel variadic argument list
		va_list list;
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
		va_list list;
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		kvsnprintf(buffer, 1024u, format, list);
		// End variadic arguments list
		va_end(list);
	}


	// Kernel printf function
	void kprintf(const sbyte_t* format, ...) noexcept {
		// Text buffer
		static sbyte_t buffer[1024ull];
		// Zero-initialize
		kmemset(buffer, u8'\0');
		// Kernel variadic argument list
		va_list list;
		// Initialize variadic arguments list
		va_start(list, format);
		// Format string
		kvsnprintf(buffer, sizeof(buffer), format, list);
		// End variadic arguments list
		va_end(list);
		// Output buffer
		arch::vmemWrite(buffer);
		arch::vmemWrite(u8"\r\n");
	}


}	// namespace igros::klib

