////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Interrupt descriptor table low-level operations
///	@date		22 Dec 2023
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <array>
#include <bit>
#include <cstdint>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch x86
#include <arch/x86/exceptions.hpp>
#include <arch/x86/irq.hpp>


// i386 namespace
namespace igros::i386 {


	/// @brief IDT structure
	class idt final {

		/// @brief Number of IDT entries
		constexpr static auto IDT_SIZE {256_usize};


	public:

		/// @brief Copy c-tor
		idt(const idt &other) = delete;
		/// @brief Copy assignment
		auto	operator=(const idt &other) -> idt& = delete;
		/// @brief Move c-tor
		idt(idt &&other) = delete;
		/// @brief Move assignment
		auto	operator=(idt &&other) -> idt& = delete;


#pragma pack(push, 1)

		/// @brief IDT table entry
		struct entry_t {
			igros_word_t	offsetLow;	///< Offset lower part (0..15)
			igros_word_t	selector;	///< Selectod from GDT/LDT
			igros_byte_t	reserved;	///< Must be zero
			igros_byte_t	type;		///< Type attributes
			igros_word_t	offsetHigh;	///< Offset higher part (16..31)
		};

		/// @brief IDT table pointer
		// cppcheck-suppress-begin unusedStructMember ; fixed layout read by the CPU, not by C++ code
		struct pointer_t {
			igros_word_t	size;		///< IDT size
			const entry_t*	pointer;	///< IDT pointer
		};
		// cppcheck-suppress-end unusedStructMember

#pragma pack(pop)

		/// @brief IDT table type
		using table_t	= std::array<entry_t, IDT_SIZE>;

		/// @brief IDT offset type
		using offset_t	= std::add_pointer_t<void ()>;

		/// @brief Default c-tor
		idt() noexcept = default;
		/// @brief D-tor
		~idt() noexcept = default;

		/// @brief Make IDT entry
		/// @param handle Handler address
		/// @param selector Code segment selector
		/// @param type Gate type and attributes
		/// @return IDT entry
		[[nodiscard]]
		static auto	setEntry(const offset_t handle, const igros_word_t selector, const igros_byte_t type) noexcept -> entry_t;
		/// @brief Calc IDT size
		/// @param table IDT
		/// @return IDT size in bytes - 1 (IDT pointer limit)
		[[nodiscard]]
		constexpr static auto	calcSize(const table_t &table) noexcept -> igros_word_t;

		/// @brief Set IDT pointer
		/// @param pointer IDT pointer to load
		static void		setPointer(const pointer_t &pointer) noexcept;

		/// @brief Init IDT table
		static void		init() noexcept;


	};


	/// @brief Make IDT entry
	[[nodiscard]]
	inline auto idt::setEntry(const offset_t handle, const igros_word_t selector, const igros_byte_t type) noexcept -> entry_t {
		// Handler address
		const auto address {std::bit_cast<igros_usize_t>(handle)};
		return entry_t {
			.offsetLow	= static_cast<igros_word_t>(address & 0xFFFF_usize),
			.selector	= selector,
			.reserved	= 0_u8,
			.type		= type,
			.offsetHigh	= static_cast<igros_word_t>((address >> 16) & 0xFFFF_usize),
		};
	}

	/// @brief Calculate IDT size
	[[nodiscard]]
	constexpr auto idt::calcSize(const table_t &table) noexcept -> igros_word_t {
		return static_cast<igros_word_t>(table.size() * sizeof(entry_t)) - 1_u16;
	}


}	// namespace igros::i386

