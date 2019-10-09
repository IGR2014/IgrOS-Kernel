////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.cpp
//	Date:	09 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdarg>

#include <klib/kprint.hpp>
#include <klib/kstring.hpp>
#include <klib/kmath.hpp>


// Kernel library code zone
namespace klib {


	// Default temporary buffer for kitoa
	constexpr std::size_t	KITOA_BUFF_LEN		= 65;
	// Constant integer symbols values buffer
	static const sbyte_t	KITOA_CONST_BUFFER[]	= {"0123456789ABCDEF"};


	// Kernel integer to string template function
	inline static sbyte_t* kitoaHelper(sbyte_t* buffer, std::size_t size, dword_t value, const base radix) {

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN] {};
		// Setup counter to last - 1 position in temporary buffer
		dword_t	pos = KITOA_BUFF_LEN - 2;
		// Check if sign is negative and value should be represented
		// as decimal (binary, octal and hexidemical values have no sign)
		if ((value < 0)	&& (base::DEC == radix)) {
			// Write minus sign to buffer
			*buffer++ = '-';
			// Decrement size counter
			--size;
			// Made value positive
			value = -value;
		}

		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate digit index
			auto digitIndex		= value % byte_t(radix);
			// Calculate new value
			auto newValue		= value	/ byte_t(radix);
			// Save current digit to temporary buffer
			tempBuffer[pos--]	= KITOA_CONST_BUFFER[digitIndex];
			// Divide value by base to remove current digit
			value			= newValue;
		} while (value > 0);

		// Resulting string length
		std::size_t strLength = (KITOA_BUFF_LEN - pos);
		// Check size fit
		if (strLength > size) {
			return buffer;
		}
		// Revert temporary buffer we created from value to src buffer
		return kstrcpy(&tempBuffer[++pos], buffer, strLength);

	}


	// Kernel long integer to string template function 
	inline static sbyte_t* kltoaHelper(sbyte_t* buffer, std::size_t size, quad_t value, const base radix) {

		// Temporary buffer for value text representation
		sbyte_t tempBuffer[KITOA_BUFF_LEN] {};
		// Setup counter to last - 1 position in temporary buffer
		quad_t	pos = KITOA_BUFF_LEN - 2;
		// Check if sign is negative and value should be represented
		// as decimal (binary, octal and hexidemical values have no sign)
		if ((value < 0)	&& (base::DEC == radix)) {
			// Write minus sign to buffer
			*buffer++ = '-';
			// Decrement size counter
			--size;
			// Made value positive
			value = -value;
		}

		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate digit index
			udivmod_t divres	= kudivmod(value, dword_t(radix));
			// Save current digit to temporary buffer
			tempBuffer[pos--]	= KITOA_CONST_BUFFER[divres.reminder];
			// Divide value by base to remove current digit
			value			= divres.quotient;
		} while (value > 0);

		// Resulting string length
		std::size_t strLength = (KITOA_BUFF_LEN - pos);
		// Check size fit
		if (strLength > size) {
			return buffer;
		}
		// Revert temporary buffer we created from value to src buffer
		return kstrcpy(&tempBuffer[++pos], buffer, strLength);

	}


	// Kernel unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const dword_t value, const base radix) {
		return kitoaHelper(buffer, size, value, radix);
	}

	// Kernel large unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const quad_t value, const base radix) {
		return kltoaHelper(buffer, size, value, radix);
	}


	// Kernel size type to string function
	sbyte_t* kstoa(sbyte_t* buffer, std::size_t size, const std::size_t value, const base radix) {

#if defined(IGROS_ARCH_i386)

		return kitoaHelper(buffer, size, value, radix);

#else

		return kltoaHelper(buffer, size, value, radix);

#endif

	}


	// Kernel vsnprintf function
	void kvsnprint(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, va_list list/*kvaList &list*/) {

		// Foramt string iterator
		auto fmtIterator = 0ULL;
		// Resulting string iterator
		auto strIterator = buffer;

		// String pointer holder
		char*		str = nullptr;
		// Numeric value holder
		std::size_t	len = 0ULL;
		// Number holder
		sbyte_t		number[65] {};

		// Iterate through format string
		while ((fmtIterator < size) && (format[fmtIterator] != '\0')) {

			// If symbol is not placeholder symbol '%'
			if (format[fmtIterator] != '%') {
				// Copy string till the next placeholder symbol '%'
				*strIterator++ = format[fmtIterator++];
			// Placeholder symbol '%' received
			} else {

				// Quad argument flag
				bool isQuad = false;
				// Word argument flag
				bool isWord = false;
				// Byte argument flag
				bool isByte = false;
				// Check if quad specifier
				if (format[fmtIterator + 1] == 'l') {
					// Check if quad specifier
					if (format[fmtIterator + 2] == 'l') {
						// Quad argument
						isQuad = true;
						// Adjust format iterator
						fmtIterator += 2;
					}
				// Otherwise it could be word
				} else if (format[fmtIterator + 1] == 'h') {
					// Or even byte
					if (format[fmtIterator + 2] == 'h') {
						// Byte argument
						isByte = true;
						// Adjust format iterator
						fmtIterator += 2;
					} else {
						// Word argument
						isWord = true;
						// Adjust format iterator
						++fmtIterator;
					}
				}
				// Otherwise it's double word

				// Determine type
				switch (format[++fmtIterator]) {

				// Binary integer
				case 'b':
					// Check if quad specified
					if (isQuad) {
						// Get string length
						len = kstrlen(kitoa(number, 64, quad_t(va_arg(list, quad_t))/*list.arg<quad_t>()*/, base::BIN));
					} else if (isWord) {
						// Get string length
						len = kstrlen(kitoa(number, 64, word_t(va_arg(list, dword_t))/*list.arg<word_t>()*/, base::BIN));
					} else if (isByte) {
						// Get string length
						len = kstrlen(kitoa(number, 64, byte_t(va_arg(list, dword_t))/*list.arg<byte_t>()*/, base::BIN));
					} else {
						// Get string length
						len = kstrlen(kitoa(number, 64, dword_t(va_arg(list, dword_t))/*list.arg<dword_t>()*/, base::BIN));
					}
					// Copy string
					kstrcpy(number, strIterator, len);
					// Move iterator to string's end
					strIterator += len;
					break;

				// '%' character
				case '%':
					// Copy placeholder symbol '%'
					*strIterator++ = format[fmtIterator];
					break;

				// Character
				case 'c':
					// Copy character to resulting string
					*strIterator++ = sbyte_t(va_arg(list, dword_t));//list.arg<sbyte_t>();
					break;

				// Integer
				case 'd':
				case 'i':
					// Check if quad specified
					if (isQuad) {
						// Get string length
						len = kstrlen(kitoa(number, 64, quad_t(va_arg(list, squad_t))/*list.arg<squad_t>()*/));
					} else if (isWord) {
						// Get string length
						len = kstrlen(kitoa(number, 64, word_t(va_arg(list, dword_t))/*list.arg<sword_t>()*/));
					} else if (isByte) {
						// Get string length
						len = kstrlen(kitoa(number, 64, byte_t(va_arg(list, dword_t))/*list.arg<sbyte_t>()*/));
					} else {
						// Get string length
						len = kstrlen(kitoa(number, 64, dword_t(va_arg(list, sdword_t))/*list.arg<sdword_t>()*/));
					}
					// Copy string
					kstrcpy(number, strIterator, len);
					// Move iterator to string's end
					strIterator += len;
					break;

				// Octal integer
				case 'o':
					// Check if quad specified
					if (isQuad) {
						// Get string length
						len = kstrlen(kitoa(number, 64, quad_t(va_arg(list, quad_t))/*list.arg<quad_t>()*/, base::OCT));
					} else if (isWord) {
						// Get string length
						len = kstrlen(kitoa(number, 64, word_t(va_arg(list, dword_t))/*list.arg<word_t>()*/, base::OCT));
					} else if (isByte) {
						// Get string length
						len = kstrlen(kitoa(number, 64, byte_t(va_arg(list, dword_t))/*list.arg<byte_t>()*/, base::OCT));
					} else {
						// Get string length
						len = kstrlen(kitoa(number, 64, dword_t(va_arg(list, dword_t))/*list.arg<dword_t>()*/, base::OCT));
					}
					// Copy string
					kstrcpy(number, strIterator, len);
					// Move iterator to string's end
					strIterator += len;
					break;

				// Address
				case 'p':
					// Get string length
					len = kstrlen(kptoa(number, 64, pointer_t(va_arg(list, pointer_t))/*list.arg<dword_t>()*/));
					// Copy string
					kstrcpy(number, strIterator, len);
					// Move iterator to string's end
					strIterator += len;
					break;

				// String
				case 's':
					// Get string from args
					str = va_arg(list, sbyte_t*);//list.arg<sbyte_t*>();
					// Get string length
					len = kstrlen(str);
					// Copy string
					kstrcpy(str, strIterator, len);
					// Move iterator to string's end
					strIterator += len;
					break;

				// Unsigned integer
				case 'u':
					// Check if quad specified
					if (isQuad) {
						// Get string length
						len = kstrlen(kitoa(number, 64, quad_t(va_arg(list, quad_t))/*list.arg<quad_t>()*/));
					} else if (isWord) {
						// Get string length
						len = kstrlen(kitoa(number, 64, word_t(va_arg(list, dword_t))/*list.arg<word_t>()*/));
					} else if (isByte) {
						// Get string length
						len = kstrlen(kitoa(number, 64, byte_t(va_arg(list, dword_t))/*list.arg<byte_t>()*/));
					} else {
						// Get string length
						len = kstrlen(kitoa(number, 64, dword_t(va_arg(list, dword_t))/*list.arg<dword_t>()*/));
					}
					// Copy string
					kstrcpy(number, strIterator, len);
					// Move iterator to string's end
					strIterator += len;
					break;

				// Hexidemical integer
				case 'x':
					// Check if quad specified
					if (isQuad) {
						// Get string length
						len = kstrlen(kitoa(number, 64, quad_t(va_arg(list, quad_t))/*list.arg<quad_t>()*/, base::HEX));
					} else if (isWord) {
						// Get string length
						len = kstrlen(kitoa(number, 16, word_t(va_arg(list, dword_t))/*list.arg<word_t>()*/, base::HEX));
					} else if (isByte) {
						// Get string length
						len = kstrlen(kitoa(number, 8, byte_t(va_arg(list, dword_t))/*list.arg<byte_t>()*/, base::HEX));
					} else {
						// Get string length
						len = kstrlen(kitoa(number, 32, dword_t(va_arg(list, dword_t))/*list.arg<dword_t>()*/, base::HEX));
					}
					// Copy string
					kstrcpy(number, strIterator, len);
					// Move iterator to string's end
					strIterator += len;
					break;

				// Default action
				default:
					// Copy character to resulting string
					*strIterator++ = '?';
					break;

				}

				// Incremet format iterator
				++fmtIterator;

			}

		}

		// Insert null terminator
		*strIterator++ = '\0';

	}


	// Kernel snprintf function
	void ksnprint(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, ...) {
		// Kernel variadic argument list
		//kvaList list;
		va_list list;
		// Initialize variadic arguments list
		//list.start(format);
		va_start(list, format);
		// Format string
		kvsnprint(buffer, size, format, list);
		// End variadic arguments list
		//list.end();
		va_end(list);
	}

	// Kernel sprintf function
	void ksprint(sbyte_t* buffer, const sbyte_t* format, ...) {
		// Kernel variadic argument list
		//kvaList list;
		va_list list;
		// Initialize variadic arguments list
		//list.start(format);
		va_start(list, format);
		// Format string
		kvsnprint(buffer, 1024, format, list);
		// End variadic arguments list
		//list.end();
		va_end(list);
	}


}	// namespace klib

