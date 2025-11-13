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
 * \brief This header contains definitions of the fill_vector module's functions.
**/

#ifndef _FILL_VECTOR_H_
    /// @brief Include guard.
#   define _FILL_VECTOR_H_

    /// @brief Dispatcher function for the main-thread for this module: this will spawn and run new workers, waiting for all of them to exit, printing the total execution time.
    void fill_vector(void);
    /// @brief Workers entry point for this module.
    /// @param arg A pointer to a `struct data` containing the parameters for the worker.
    void* fill_segment(void* arg);

    /// @brief Demo host entry point for this module.
    void run_fill_vector(void);
#endif /* !defined(_FILL_VECTOR_H_) */
