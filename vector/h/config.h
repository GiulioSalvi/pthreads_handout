// Copyright (C) 2025  Giulio Salvi
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

/**
 * \file
 * \brief This header contains definitions of macros, types, global variables and includes libraries used by standalone modules or by the demo host.
**/

#ifndef _CONFIG_H_
    /// @brief Include guard.
#   define _CONFIG_H_

#   include <time.h>
#   include <stdio.h>
#   include <ctype.h>
#   include <stdlib.h>
#   include <string.h>
#   include <pthread.h>
#   include <stdbool.h>

    /// @brief The number of threads used by the application.
#   define NTHREADS 8u
    /// @brief The size of the vector.
#   define VECTOR_SIZE 1000000u

    /// @brief Definition of the vector.
    extern int vector[VECTOR_SIZE];
    /// @brief Definition of a boolean flag used by the find_zero module.
    extern volatile bool zero_found;

    /// @brief A structure used by the fill_vector module when assigning the segment to populate to a thread.
    struct data {
        /// @brief Start index of the range, inclusive.
        unsigned int start_idx;
        /// @brief End index of the range, exclusive.
        unsigned int end_idx;
        /// @brief The seed for generating random numbers (segment-wide).
        /// 
        /// Each segment has a different random generator seed.
        unsigned int seed;
    };

    /// @brief A structure used by some modules when assigning read-only operations on a vector's segment to a thread.
    struct data_search {
        /// @brief Start index of the range, inclusive.
        unsigned int start_idx;
        /// @brief End index of the range, exclusive.
        unsigned int end_idx;
    };

    /// @brief Type definition for the structure `struct data`.
    typedef struct data data_t;
    /// @brief Type definition for the structure `struct data_search`.
    typedef struct data_search data_search_t;
#endif /* !defined(_CONFIG_H_) */
