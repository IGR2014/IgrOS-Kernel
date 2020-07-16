////////////////////////////////////////////////////////////////
//
//	IgrOS singleton implementation
//
//	File:	singleton.hpp
//	Date:	16 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// OS namespace
namespace igros {


	// Singleton
	template<typename T>
	class singleton {

		// Default c-tor
		singleton() noexcept {}

		// No copy construction
		singleton(const singleton &other) noexcept = delete;
		// No copy assignment
		singleton& operator=(const singleton &other) noexcept = delete;

		// No move construction
		singleton(singleton &&other) noexcept = delete;
		// No move assignment
		singleton& operator=(singleton &&other) noexcept = delete;


	public:

		// Get instance function
		static T& get() noexcept;


	};

	// Get instance function
	template<typename T>
	T& singleton<T>::get() noexcept {
		// Create static object (if not yet created)
		static T s;
		// Return reference to it
		return s;
	}


}	// namespace igros

