////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.cpp
//	Date:	14 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kprint.hpp>
#include <klib/kstring.hpp>
#include <klib/memset.hpp>


// Kernel library code zone
namespace klib {


	// Default temporary buffer for kitoa
	constexpr dword_t DEF_KITOA_BUFF_LEN	= 65;


	// Kernel integer to string template function 
	template<typename T>
	inline sbyte_t* kitoaHelper(sbyte_t* buffer, dword_t size, T value, const base radix) {

		// Constant integer symbols values buffer
		static const sbyte_t	constBuffer[]			= {"0123456789ABCDEF"};
		// Temporary buffer for value text representation
		sbyte_t			tempBuffer[DEF_KITOA_BUFF_LEN]	= {};
		// Setup counter to last - 1 position in temporary buffer
		T			position			= DEF_KITOA_BUFF_LEN - 2;

		// Check if sign is negative and value should be represented
		// as decimal (binary, octal and hexidemical values have no sign)
		if ((value < 0)	&& (radix == base::DEC)) {
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
			// Save current digit to temporary buffer
			tempBuffer[position--]	=  constBuffer[value % static_cast<T>(radix)];
			// Divide value by base to remove current digit
			value			/= static_cast<T>(radix);
		} while (value > 0);

		// Resulting string length
		dword_t strLength = (DEF_KITOA_BUFF_LEN - position);

		// Check size fit
		if (strLength > size) {
			return buffer;
		}

		// Revert temporary buffer we created from value to src buffer
		return kstrcpy(&tempBuffer[++position], buffer, strLength);

	}

	// Kernel unsigned byte to string function
	sbyte_t* kitoa(sbyte_t* buffer, dword_t size, byte_t value, const base radix) {

		return kitoaHelper<byte_t>(buffer, size, value, radix);

	}

	// Kernel signed byte to string function
	sbyte_t* kitoa(sbyte_t* buffer, dword_t size, sbyte_t value, const base radix) {

		return kitoaHelper<sbyte_t>(buffer, size, value, radix);

	}


	// Kernel unsigned word to string function
	sbyte_t* kitoa(sbyte_t* buffer, dword_t size, word_t value, const base radix) {

		return kitoaHelper<word_t>(buffer, size, value, radix);

	}

	// Kernel signed word to string function
	sbyte_t* kitoa(sbyte_t* buffer, dword_t size, sword_t value, const base radix) {

		return kitoaHelper<sword_t>(buffer, size, value, radix);

	}


	// Kernel unsigned double word to string function
	sbyte_t* kitoa(sbyte_t* buffer, dword_t size, dword_t value, const base radix) {

		return kitoaHelper<dword_t>(buffer, size, value, radix);

	}

	// Kernel signed double word to string function
	sbyte_t* kitoa(sbyte_t* buffer, dword_t size, sdword_t value, const base radix) {

		return kitoaHelper<sdword_t>(buffer, size, value, radix);

	}


	// Kernel unsigned quad word to string function
	sbyte_t* kitoa(sbyte_t* buffer, dword_t size, quad_t value, const base radix) {

		return kitoaHelper<quad_t>(buffer, size, value, radix);

	}

	// Kernel signed quad word to string function
	sbyte_t* kitoa(sbyte_t* buffer, dword_t size, squad_t value, const base radix) {

		return kitoaHelper<squad_t>(buffer, size, value, radix);

	}


}	// namespace klib

