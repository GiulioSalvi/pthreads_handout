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
 * \brief This header contains definitions of bubble_sort module's structures and functions.
**/

#ifndef _BUBBLE_SORT_H_
    /// @brief Include guard.
#   define _BUBBLE_SORT_H_

#   include <stdbool.h>

    /// @brief A structure used to contain informations about the options forwarded to this module.
    struct bs_module_options {
        /// @brief Flags if the CLI options forwarded to this module are valid.
        bool cli_valid;
        /// @brief Flags if the module has to validate its result.
        bool validate_bs_module;
    };

    /// @brief Type definition for the structure `struct bs_module_options`.
    typedef struct bs_module_options bs_module_options_t;

    /// @brief Dispatcher function for the main-thread for this module: this will spawn and run new workers, waiting for all of them to exit, printing the total execution time.
    void bubble_sort(void);
    /// @brief Workers entry point for this module.
    /// @param arg A pointer to a `struct data_search` containing the parameters for the worker.
    void* bubble_sort_segment(void* arg);

    /// @brief Demo host entry point for this module.
    /// @param validate Flags if the module has to validate its result.
    /// @return Returns the validation result if `validate` is `true`. Otherwise, returns `true`.
    bool run_bubble_sort(bool validate);
    /// @brief Validates the results of this module.
    /// @return Returns `true` if the module's result is correct, otherwise returns `false`.
    bool validate_bubble_sort(void);

    /// @brief Checks if a segment of the vector is increasingly ordered.
    /// @param start_idx Start index of the segment, inclusive.
    /// @param end_idx End index of the segment, exclusive.
    /// @return Returns `true` if the segment is increasingly ordered, otherwise returns `false`.
    bool check_segment(unsigned int start_idx, unsigned int end_idx);

    /// @brief Asks the module options on the standard input.
    /// @param options A pointer to a `struct bs_module_options` where the option values are stored, always overriding their content.
    void ask_bs_module_options(bs_module_options_t* options);
    /// @brief Parses the CLI options contained in `argv` from a given `offset`. It sets `options->cli_valid` to `true` if and only if the options are used correctly.
    /// 
    /// When only the options `--help` or `-h` are used, it exits the program with exit code `1`.
    /// @param argc The actual length of `argv`.
    /// @param argv The options buffer.
    /// @param offset The offset from which it starts to parse the CLI options. It must be between `0` and `argc`, exclusive.
    /// @param options A pointer to a `struct bs_module_options` where the option values are stored, always overriding their content.
    void parse_bs_module_options(int argc, char** argv, int offset, bs_module_options_t* options);
#endif /* !defined(_BUBBLE_SORT_H_) */
