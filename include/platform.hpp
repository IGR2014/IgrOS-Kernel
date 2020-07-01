////////////////////////////////////////////////////////////////
//
//	IgrOS platform description
//
//	File:	platform.hpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>

#include <types.hpp>


// OS namespace
namespace igros {


        // Platform init function pointer type
        using funcInit_t	= std::add_pointer_t<void()>;
        // Platform shutdown function pointer type
        using funcShutdown_t	= std::add_pointer_t<void()>;
        // Platform reboot function pointer type
        using funcReboot_t	= std::add_pointer_t<void()>;


        // Platform desciption structure
        struct platform {

                sbyte_t*		name;			// Platform name

                funcInit_t		init;			// Init function
                funcShutdown_t		shutdown;		// Shutdown function
                funcReboot_t		reboot;			// Reboot function

        };


        // Platform description
        extern const platform	platformDescription;


}       // namespace igros

