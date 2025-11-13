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
 * \brief This header contains declarations and instantiations of global variables used by standalone modules or by the demo host.
**/

#ifndef _DECLS_H_
    /// @brief Include guard.
#   define _DECLS_H_

#   include "config.h"

    /// @brief Declaration of an atomic boolean flag used by the basic_primality_check module.
    atomic_bool divisor_found = false;
#endif /* !defined(_DECLS_H_) */
