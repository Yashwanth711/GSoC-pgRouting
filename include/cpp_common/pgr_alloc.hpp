/*PGR-GNU*****************************************************************
File: pgr_palloc.hpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

/*! @file */

#ifndef INCLUDE_CPP_COMMON_PGR_ALLOC_HPP_
#define INCLUDE_CPP_COMMON_PGR_ALLOC_HPP_
#pragma once


#include <string>

extern "C" {
#if 0
#if PGSQL_VERSION < 94
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-pedantic"
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-pedantic"
#endif

#if defined(WIN32) && defined(_MSC_VER)
/* disable the following warnings 
 * C4200: Non standard extension C zero sized array
 */
#pragma warning(disable:4200)  
#endif

#include <postgres.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif





#ifdef __GNUC__
#pragma GCC diagnostic ignored "-pedantic"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-pedantic"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include "executor/spi.h"


#ifdef __GNUC__
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#pragma clang diagnostic pop
#pragma clang diagnostic pop
#endif


#endif
extern void *palloc(size_t size);
void *
 SPI_palloc(size_t size);

extern void *
SPI_repalloc(void *pointer, size_t size);

extern void
SPI_pfree(void *pointer);

// #include "c_common/postgres_connection.h"
// #include "utils/palloc.h"
}


/*! \fn pgr_alloc(std::size_t size, T *ptr)

\brief allocates memory

- Does a malloc or realloc depending on the ptr value
- To be used only on C++ code
- To be used when returning results to postgres
- free must occur in the C code

\param[in] size
\param[in] ptr
\returns pointer to the first byte of allocated space

 */


template <typename T>
T*
pgr_alloc(std::size_t size, T* ptr) {
    if (!ptr) {
        ptr = static_cast<T*>(SPI_palloc(size * sizeof(T)));
    } else {
        ptr = static_cast<T*>(SPI_repalloc(ptr, size * sizeof(T)));
    }
    return ptr;
}

template <typename T>
T*
pgr_free(T* ptr) {
    if (ptr) {
        SPI_pfree(ptr);
    }
    return nullptr;
}

char *
pgr_msg(const std::string &msg);

#endif  // INCLUDE_CPP_COMMON_PGR_ALLOC_HPP_
