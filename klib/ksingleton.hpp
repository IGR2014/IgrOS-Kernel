////////////////////////////////////////////////////////////////
//
//	IgrOS singleton implementation
//
//	File:	singleton.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// OS namespace
namespace igros {


	// Singleton
	template<typename T>
	class singleton {

		// No copy construction
		singleton(const singleton &other) = delete;
		// No copy assignment
		singleton& operator=(const singleton &other) = delete;

		// No move construction
		singleton(singleton &&other) = delete;
		// No move assignment
		singleton& operator=(singleton &&other) = delete;


	protected:

		// Default c-tor
		singleton() noexcept = default;


	public:

		// Get instance function
		[[nodiscard]]
		static T&	get() noexcept;


	};

	// Get instance function
	template<typename T>
	[[nodiscard]]
	T& singleton<T>::get() noexcept {
		// Create static object (if not yet created)
		static T s;
		// Return reference to it
		return s;
	}


}	// namespace igros

