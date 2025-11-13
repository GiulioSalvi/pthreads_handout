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
 * \brief This header contains definitions of basic_primality_check module's structures and functions.
**/

#ifndef _BASIC_PRIMALITY_CHECK_H_
    /// @brief Include guard.
#   define _BASIC_PRIMALITY_CHECK_H_

#   include <stdbool.h>

    /// @brief A structure used to contain the parameters for a worker for this module.
    struct bpc_module_param {
        /// @brief The number on which the primality check has to be performed.
        unsigned long n;
        /// @brief Start index of the range, inclusive.
        unsigned long from;
        /// @brief End index of the range, exclusive.
        unsigned long to;
        /// @brief A pointer to an `unsigned long` where the first divisor that it has been found by this worker is stored.
        unsigned long* divisor;
    };

    /// @brief A structure used to contain informations about the options forwarded to this module.
    struct bpc_module_options {
        /// @brief Flags if the CLI options forwarded to this module are valid.
        bool cli_valid;
        /// @brief Flags if the module has to validate its result.
        bool validate_bpc_module;
        /// @brief The number on which the primality check has to be performed.
        unsigned long n;
    };

    /// @brief Type definition for the structure `struct bcp_module_param`.
    typedef struct bpc_module_param bpc_module_param_t;
    /// @brief Type definition for the structure `struct bpc_module_options`.
    typedef struct bpc_module_options bpc_module_options_t;

    /// @brief Dispatcher function for the main-thread for this module: this will spawn and run new workers; when a thread finds the result (whether the number is prime) it cancels all the threads, and prints the total execution time.
    /// 
    /// The first operation on `first_divisor` is to set the referenced variable to `0`.
    /// @param n The number on which the primality check has to be performed.
    /// @param first_divisor A pointer to an `unsigned long` where the first divisor that it has been found is stored.
    /// @return Returns `true` if `n` is prime, otherwise returns `false`.
    bool basic_primality_check(unsigned long n, unsigned long* first_divisor);
    /// @brief Workers entry point for this module.
    /// @param arg A pointer to a `struct bpc_module_param` containing the parameters for the worker.
    void* bpc_module_has_divisor(void* arg);

    /// @brief Prints to `out_file` file some relevant informations about the parameters for the worker identified by the id `thread_id`.
    /// @param out_file Pointer to the output file.
    /// @param params A pointer to a `struct bpc_module_param` containing the parameters for the worker.
    /// @param thread_id The worker's ID.
    void print_bpc_module_parameters(FILE* out_file, bpc_module_param_t* params, unsigned int thread_id);

    /// @brief Demo host entry point for this module.
    /// @param n The number on which the primality check has to be performed.
    /// @param validate Flags if the module has to validate its result.
    /// @return Returns the validation result if `validate` is `true`. Otherwise, returns `true`. 
    bool run_basic_primality_check(unsigned long n, bool validate);
    /// @brief Validates the results of this module.
    /// @param n The number on which the primality check had been performed.
    /// @param first_divisor The first divisor of `n` that the module had found.
    /// @return Returns `true` if the module's result is correct, otherwise returns `false`. 
    bool validate_basic_primality_check(unsigned long n, unsigned long first_divisor);

    /// @brief Asks the module options on the standard input.
    /// @param options A pointer to a `struct bpc_module_options` where the option values are stored, always overriding their content.
    void ask_bpc_module_options(bpc_module_options_t* options);
    /// @brief Parses the CLI options contained in `argv` from a given `offset`. It sets `options->cli_valid` to `true` if and only if the options are used correctly.
    /// 
    /// When only the options `--help` or `-h` are used, it exits the program with exit code `1`.
    /// @param argc The actual length of `argv`.
    /// @param argv The options buffer.
    /// @param offset The offset from which it starts to parse the CLI options. It must be between `0` and `argc`, exclusive.
    /// @param options A pointer to a `struct bpc_module_options` where the option values are stored, always overriding their content.
    void parse_bpc_module_options(int argc, char** argv, int offset, bpc_module_options_t* options);
#endif /* !defined(_BASIC_PRIMALITY_CHECK_H_) */
