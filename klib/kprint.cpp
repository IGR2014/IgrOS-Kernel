////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.cpp
//	Date:	06 Jun 2019
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
	constexpr dword_t DEFAULT_KITOA_BUFFER_LENGTH	= 65;


	// Kernel integer to string template function 
	template<typename T>
	inline void kitoaHelper(sbyte_t* buffer, dword_t size, T value, const base radix) {

		// Constant integer symbols values buffer
		static const sbyte_t	constBuffer[]				= {"0123456789ABCDEF"};
		// Temporary buffer for value text representation
		sbyte_t			tempBuffer[DEFAULT_KITOA_BUFFER_LENGTH]	= {};
		// Setup counter to last - 1 position in temporary buffer
		T			position				= DEFAULT_KITOA_BUFFER_LENGTH - 2;

		// Check if sign is negative and value should be represented
		// as decimal or octal (binary and hexidemical values have no sign)
		if ((value < 0)	&& ((radix == base::DEC) || (radix == base::OCT))) {

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
		dword_t strLength = (DEFAULT_KITOA_BUFFER_LENGTH - position);

		// Check size fit
		if (strLength > size) {

			return;

		}

		// Revert temporary buffer we created from value to src buffer
		kstrcpy(&tempBuffer[++position], buffer, strLength);

	}

	// Kernel unsigned byte to string function
	void kitoa(sbyte_t* buffer, dword_t size, byte_t value, const base radix) {

		kitoaHelper<byte_t>(buffer, size, value, radix);

	}

	// Kernel signed byte to string function
	void kitoa(sbyte_t* buffer, dword_t size, sbyte_t value, const base radix) {

		kitoaHelper<sbyte_t>(buffer, size, value, radix);

	}


	// Kernel unsigned word to string function
	void kitoa(sbyte_t* buffer, dword_t size, word_t value, const base radix) {

		kitoaHelper<word_t>(buffer, size, value, radix);

	}

	// Kernel signed word to string function
	void kitoa(sbyte_t* buffer, dword_t size, sword_t value, const base radix) {

		kitoaHelper<sword_t>(buffer, size, value, radix);

	}


	// Kernel unsigned double word to string function
	void kitoa(sbyte_t* buffer, dword_t size, dword_t value, const base radix) {

		kitoaHelper<dword_t>(buffer, size, value, radix);

	}

	// Kernel signed double word to string function
	void kitoa(sbyte_t* buffer, dword_t size, sdword_t value, const base radix) {

		kitoaHelper<sdword_t>(buffer, size, value, radix);

	}


	// Kernel unsigned quad word to string function
	void kitoa(sbyte_t* buffer, dword_t size, quad_t value, const base radix) {

		//kitoaHelper<quad_t>(buffer, size, value, radix);

	}

	// Kernel signed quad word to string function
	void kitoa(sbyte_t* buffer, dword_t size, squad_t value, const base radix) {

		//kitoaHelper<squad_t>(buffer, size, value, radix);

	}


}	// namespace klib

