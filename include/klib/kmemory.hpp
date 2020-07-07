////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	kmemset.hpp
//	Date:	07 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>

#include <types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Set required memory with specified byte
	[[maybe_unused]] pointer_t kmemset8(byte_t* dst, const std::size_t size, const byte_t val) noexcept;

	// Set required memory with specified word
	[[maybe_unused]] pointer_t kmemset16(word_t* dst, const std::size_t size, const word_t val) noexcept;

	// Set required memory with specified double word
	[[maybe_unused]] pointer_t kmemset32(dword_t* dst, const std::size_t size, const dword_t val) noexcept;

	// Set required memory with specified quad word
	[[maybe_unused]] pointer_t kmemset64(quad_t* dst, const std::size_t size, const quad_t val) noexcept;


	// Set required memory with specified byte
	[[maybe_unused]] inline pointer_t kmemset(const pointer_t dst, const std::size_t size, const byte_t val) noexcept {
		return kmemset8(static_cast<byte_t*>(dst), size, val);
	}

	// Set required memory with specified word
	[[maybe_unused]] inline pointer_t kmemset(const pointer_t dst, const std::size_t size, const word_t val) noexcept {
		return kmemset16(static_cast<word_t*>(dst), size, val);
	}

	// Set required memory with specified double word
	[[maybe_unused]] inline pointer_t kmemset(const pointer_t dst, const std::size_t size, const dword_t val) noexcept {
		return kmemset32(static_cast<dword_t*>(dst), size, val);
	}

	// Set required memory with specified quad word
	[[maybe_unused]] inline pointer_t kmemset(const pointer_t dst, const std::size_t size, const quad_t val) noexcept {
		return kmemset64(static_cast<quad_t*>(dst), size, val);
	}


	// Set required memory with specified signed byte
	[[maybe_unused]] inline pointer_t kmemset(const pointer_t dst, const std::size_t size, const sbyte_t val) noexcept {
		return kmemset8(static_cast<byte_t*>(dst), size, static_cast<byte_t>(val));
	}

	// Set required memory with specified signed word
	[[maybe_unused]] inline pointer_t kmemset(const pointer_t dst, const std::size_t size, const sword_t val) noexcept {
		return kmemset16(static_cast<word_t*>(dst), size, static_cast<word_t>(val));
	}

	// Set required memory with specified signed double word
	[[maybe_unused]] inline pointer_t kmemset(const pointer_t dst, const std::size_t size, const sdword_t val) noexcept {
		return kmemset32(static_cast<dword_t*>(dst), size, static_cast<dword_t>(val));
	}

	// Set required memory with specified signed quad word
	[[maybe_unused]] inline pointer_t kmemset(const pointer_t dst, const std::size_t size, const squad_t val) noexcept {
		return kmemset64(static_cast<quad_t*>(dst), size, static_cast<quad_t>(val));
	}


	// Set required memory with specified data
	template<std::size_t S, typename T>
	[[maybe_unused]] inline pointer_t kmemset(T (&arr)[S], const T val) noexcept {
		return kmemset(arr, S, val);
	}


	// Copy memory
	[[maybe_unused]] pointer_t kmemcpy(pointer_t dst, const pointer_t src, const std::size_t size) noexcept;


}	// namespace igros::arch

