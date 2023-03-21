////////////////////////////////////////////////////////////////
///
///	@brief		IgrOS singleton implementation
///
///	@file		kSingleton.hpp
///	@date		21 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2022,
///			All rights reserved.
///	@author		Igor Baklykov
///
///


#pragma once


// C++
#include <utility>


////////////////////////////////////////////////////////////////
///
/// @brief IgrOS Kernel Library namespace
/// @namespace igros::klib
///
namespace igros::klib {


	////////////////////////////////////////////////////////////////
	///
	/// @brief Kernel singleton implementation
	/// @class kSingleton
	/// @tparam T Wrapped CRTP class
	///
	template<class T>
	class kSingleton {

		/// @brief No copy construction
		constexpr kSingleton(const kSingleton &other) = delete;
		/// @brief No copy assignment
		constexpr kSingleton& operator=(const kSingleton &other) = delete;

		/// @brief No move construction
		constexpr kSingleton(kSingleton &&other) = delete;
		/// @brief No move assignment
		constexpr kSingleton& operator=(kSingleton &&other) = delete;


	protected:

		/// @brief Default c-tor
		constexpr kSingleton() noexcept = default;


	public:

		/// @brief Get instance function
		template<class ...Args>
		[[nodiscard]]
		static auto	get(Args &&...args) noexcept -> T&;


	};


	////////////////////////////////////////////////////////////////
	///
	/// @brief Get instance function
	/// @tparam Args Singleton object class constructor arguments types
	/// @param[in] args Singleton object class constructor arguments values
	/// @return Reference to static instance ot type @c T
	///
	template<class T>
	template<class ...Args>
	[[nodiscard]]
	auto kSingleton<T>::get(Args &&...args) noexcept -> T& {
		// Create instance
		static T instance {
			std::forward<Args>(args)...
		};
		// Return instance reference
		return instance;
	}


}	// namespace igros::klib

