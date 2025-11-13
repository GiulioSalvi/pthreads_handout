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
 * \brief This header contains definitions of divisors_and_primality_check module's structures and functions.
**/

#ifndef _DIVISORS_AND_PRIMALITY_CHECK_H_
    /// @brief Include guard.
#   define _DIVISORS_AND_PRIMALITY_CHECK_H_

#   include "config.h"

    /// @brief A structure used to contain a single-linked list of divisors.
    struct divisor_list {
        /// @brief A divisor in the list.
        unsigned long divisor;
        /// @brief A pointer to a `struct divisor_list` which is the next cell of the list.
        struct divisor_list* next;
    };

    /// @brief A structure used to contain the parameters for a worker for this module.
    struct dapc_module_param {
        /// @brief The number on which the primality check has to be performed.
        unsigned long n;
        /// @brief Start index of the range, inclusive.
        unsigned long from;
        /// @brief End index of the range, exclusive.
        unsigned long to;
        /// @brief A pointer to a `struct divisor_list` which is the head of the divisors list owned by this worker.
        struct divisor_list* divisors_list;
    };

    /// @brief A structure used to contain informations about the options forwarded to this module.
    struct dapc_module_options {
        /// @brief Flags if the CLI options forwarded to this module are valid.
        bool cli_valid;
        /// @brief Flags if the module has to validate its result.
        bool validate_dapc_module;
        /// @brief The number on which the primality check has to be performed.
        unsigned long n;
    };

    /// @brief Type definition for the structure `struct divisor_list`.
    typedef struct divisor_list divisor_list_t;
    /// @brief Type definition for the structure `struct dapc_module_param`.
    typedef struct dapc_module_param dapc_module_param_t;
    /// @brief Type definition for the structure `struct dapc_module_options`.
    typedef struct dapc_module_options dapc_module_options_t;

    /// @brief Dispatcher function for the main-thread for this module: this will spawn and run new workers; when a thread finds the result (whether the number is prime) it cancels all the threads, and prints the total execution time.
    /// 
    /// The first operation on `first_divisor` is to set the referenced variable to `0`.
    /// @param n The number on which the primality check has to be performed. 
    /// @param divisors_list A pointer to a `struct divisor_list*` where the head of the list is going to be stored.
    /// @return Returns `true` if `n` is prime, otherwise returns `false`.
    bool divisors_and_primality_check(unsigned long n, divisor_list_t** divisors_list);
    /// @brief Workers entry point for this module.
    /// @param arg A pointer to a `struct dapc_module_param` containing the parameters for the worker.
    void* dapc_module_has_divisor(void* arg);

    /// @brief Frees the list and sets `*list` to `NULL`.
    /// @param list A pointer to `struct divisor_list*`, which is a pointer to the head of the list.
    void free_divisor_list(divisor_list_t** list);
    /// @brief Computes the length of the list.
    /// @param list A pointer to `struct divisor_list`, which is the head of the list.
    /// @return The length of the list.
    unsigned long get_length(divisor_list_t* list);
    /// @brief Gets the tail (the last cell) of the list.
    /// @param list A pointer to `struct divisor_list`, which is the head of the list.
    /// @return A pointer to the tail of the list.
    divisor_list_t* get_tail(divisor_list_t* list);
    /// @brief Prints the list to `out_file` file.
    /// @param out_file Pointer to the output file.
    /// @param list A pointer to `struct divisor_list`, which is the head of the list.
    void print_list(FILE* out_file, divisor_list_t* list);
    /// @brief Checks whether `element` is present in the list.
    /// @param list A pointer to `struct divisor_list`, which is the head of the list.
    /// @param element The element to look for in the list.
    /// @return Returns `true` if `element` is present, otherwise returns `false`.
    bool is_in_the_list(divisor_list_t* list, unsigned long element);
    /// @brief Prepends `divisor` at the head of the list.
    /// @param list A pointer to `struct divisor_list*`, which is a pointer to the head of the list.
    /// @param divisor The element to prepend in the list.
    void prepend_divisor(divisor_list_t** list, unsigned long divisor);

    /// @brief Prints to `out_file` file some relevant informations about the parameters for the worker identified by the id `thread_id`.
    /// @param out_file Pointer to the output file.
    /// @param params Pointer to a `struct dapc_module_param` containing the parameters for the worker.
    /// @param thread_id The worker's ID.
    void print_dapc_module_parameters(FILE* out_file, dapc_module_param_t* params, unsigned int thread_id);

    /// @brief Demo host entry point for this module.
    /// @param n The number on which the primality check has to be performed.
    /// @param validate Flags if the module has to validate its result.
    /// @return Returns the validation result if `validate` is `true`. Otherwise, returns `true`. 
    bool run_divisors_and_primality_check(unsigned long n, bool validate);
    /// @brief Validates the results of this module.
    /// @param n The number on which the primality check had been performed.
    /// @param divisors_list The list of divisors of `n` that the module had found.
    /// @return Returns `true` if the module's result is correct, otherwise returns `false`. 
    bool validate_divisors_and_primality_check(unsigned long n, divisor_list_t* divisors_list);

    /// @brief Asks the module options on the standard input.
    /// @param options A pointer to a `struct dapc_module_options` where the option values are stored, always overriding their content.
    void ask_dapc_module_options(dapc_module_options_t* options);
    /// @brief Parses the CLI options contained in `argv` from a given `offset`. It sets `options->cli_valid` to `true` if and only if the options are used correctly.
    /// 
    /// When only the options `--help` or `-h` are used, it exits the program with exit code `1`.
    /// @param argc The actual length of `argv`.
    /// @param argv The options buffer.
    /// @param offset The offset from which it starts to parse the CLI options. It must be between `0` and `argc`, exclusive.
    /// @param options A pointer to a `struct dapc_module_options` where the option values are stored, always overriding their content.
    void parse_dapc_module_options(int argc, char** argv, int offset, dapc_module_options_t* options);
#endif /* !defined(_DIVISORS_AND_PRIMALITY_CHECK_H_) */
