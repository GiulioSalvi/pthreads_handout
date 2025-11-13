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
 * \brief This header contains definitions of bubble_sort_and_merge module's structures and functions.
**/

#ifndef _BUBBLE_SORT_AND_MERGE_H_
    /// @brief Include guard.
#   define _BUBBLE_SORT_AND_MERGE_H_

#   include <stdbool.h>

    /// @brief A structure used to contain informations about the options forwarded to this module.
    struct bsam_module_options {
        /// @brief Flags if the CLI options forwarded to this module are valid.
        bool cli_valid;
        /// @brief Flags if the module has to validate bubble_sort module result.
        bool validate_bs_module;
        /// @brief Flags if the module has to validate its result.
        bool validate_bsam_module;
    };

    /// @brief Type definition for the structure `struct bsam_module_options`.
    typedef struct bsam_module_options bsam_module_options_t;

    /// @brief Merges the vector's segment without bubble sorting them.
    /// 
    /// The segment ranges are computed as if this module was the bubble_sort module.
    void bubble_sort_and_merge(void);
    /// @brief Merges two vector's segments.
    /// @param left The start index of the first segment, inclusive.
    /// @param middle The end index of the first segment, exclusive, which corresponds to the start index of the second segment, inclusive.
    /// @param right The end index of the second segment, exclusive.
    void merge(unsigned int left, unsigned int middle, unsigned int right);

    /// @brief Demo host entry point for this module.
    /// @param validate Flags if the module has to validate its result.
    /// @return Returns the validation result if `validate` is `true`. Otherwise, returns `true`.
    bool run_bubble_sort_and_merge(bool validate);
    /// @brief Validates the results of this module.
    /// @return Returns `true` if the module's result is correct, otherwise returns `false`.
    bool validate_bubble_sort_and_merge(void);

    /// @brief Asks the module options on the standard input.
    /// @param options A pointer to a `struct bsam_module_options` where the option values are stored, always overriding their content.
    void ask_bsam_module_options(bsam_module_options_t* options);
    /// @brief Parses the CLI options contained in `argv` from a given `offset`. It sets `options->cli_valid` to `true` if and only if the options are used correctly.
    /// 
    /// When only the options `--help` or `-h` are used, it exits the program with exit code `1`.
    /// @param argc The actual length of `argv`.
    /// @param argv The options buffer.
    /// @param offset The offset from which it starts to parse the CLI options. It must be between `0` and `argc`, exclusive.
    /// @param options A pointer to a `struct bsam_module_options` where the option values are stored, always overriding their content.
    void parse_bsam_module_options(int argc, char** argv, int offset, bsam_module_options_t* options);
#endif /* !defined(_BUBBLE_SORT_AND_MERGE_H_) */
