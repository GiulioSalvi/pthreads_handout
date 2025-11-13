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
 * \brief This header contains definitions of find_zero module's functions.
**/

#ifndef _FIND_ZERO_H_
    /// @brief Include guard.
#   define _FIND_ZERO_H_

#   include <stdbool.h>

    /// @brief Dispatcher function for the main-thread for this module: this will spawn and run new workers; when a thread finds the result (whether zero is in the vector) it cancels all the threads, and prints the total execution time.
    /// @return Returns `true` if zero is in the vector, otherwise returns `false`.
    bool find_zero(void);
    /// @brief Workers entry point for this module.
    /// @param arg A pointer to a `struct data_search` containing the parameters for the worker.
    void* search_zero(void* arg);

    /// @brief Demo host entry point for this module.
    void run_find_zero(void);
#endif /* !defined(_FIND_ZERO_H_) */
