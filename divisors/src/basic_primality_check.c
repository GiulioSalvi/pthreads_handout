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
 * \brief Provides implementation of basic_primality_check module's functions.
**/

#include "config.h"
#include "basic_primality_check.h"

bool basic_primality_check(unsigned long n, unsigned long* first_divisor) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    *first_divisor = 0;

    /*
      Even if the code with which this function is shipped checks independently
      the base cases, since this is a public function, it handles the base cases.
    */
    if(n <= 2)
        return n == 2;
    else if(n % 2 == 0)
        return *first_divisor = 2, false;

    /*
      It can be shown that a composite integer has at least a divisor which is at most sqrt(n).
      Hence, the complete search range is between 2 and sqrt(n) which is splitted among the workers.
    */
    unsigned long upper_bound = ceil(sqrt(n));
    unsigned long breadth = upper_bound >= 2ul ? upper_bound - 1ul : 0ul;
    unsigned long delta = ceil((double)breadth / NTHREADS);

    pthread_t threads[NTHREADS];
    bpc_module_param_t params[NTHREADS];

    /* In order to avoid race conditions each worker has a cell in the array in which it puts the first divisor, if any is found. */
    unsigned long divisors[NTHREADS] = {0ul};

    /* Workers parameters initialization. Boundary checks are performed. */
    fprintf(stderr, "Setting up parameters for workers with delta %lu...\n", delta);
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        unsigned long current_from = 2ul + i * delta;
        unsigned long current_to = 2ul + (i + 1u) * delta;

        if(current_from > upper_bound)
            current_to = current_from = upper_bound + 1ul;

        if(current_to > upper_bound + 1ul)
            current_to = upper_bound + 1ul;

        params[i] = (bpc_module_param_t) {
            .n = n,
            .from = current_from,
            .to = current_to,
            .divisor = &divisors[i]
        };
    }

    /* Starts all the workers. */
    fprintf(stderr, "Starting up workers...\n\n");
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        if(pthread_create(&threads[i], NULL, bpc_module_has_divisor, &params[i]) == EAGAIN)
            fprintf(stderr, "Resources finished when starting worker %u.\n", i + 1u), exit(EXIT_FAILURE);

        print_bpc_module_parameters(stderr, &params[i], i + 1u);
    }

    /* Waits for all the workers to finish. */
    fprintf(stderr, "\nWaiting for a result...\n\n");
    for(unsigned int i = 0u; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    /* Catches the divisor found by the first worker that had found one. */
    fprintf(stderr, "Catching a result after %u worker(s) had completed...\n\n", NTHREADS);
    for(unsigned int i = 0u; i < NTHREADS && !*first_divisor; i++)
        if(divisors[i])
            *first_divisor = divisors[i];

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time: %.4f seconds.\n\n", elapsed);

    return !*first_divisor;
}

void* bpc_module_has_divisor(void* arg) {
    bpc_module_param_t* args = (bpc_module_param_t*) arg;

    for(unsigned long i = args->from; i < args->to && !atomic_load(&divisor_found); i++)
        if(args->n % i == 0ul)
            *args->divisor = i, atomic_store(&divisor_found, true);

    return NULL;
}

void print_bpc_module_parameters(FILE* out_file, bpc_module_param_t* params, unsigned int thread_id) {
    fprintf(out_file, "Worker %u started for checking primality on %lu in interval [%lu,%lu) (whose width is %lu).\n", thread_id, params->n, params->from, params->to, params->to - params->from);
}

bool run_basic_primality_check(unsigned long n, bool validate) {
    unsigned long first_divisor = 0ul;

    if(!n) {
        printf("Enter the number on which to perform the primality check: ");
        scanf("%lu", &n);

        printf("\n");
    }

    if(n == 0ul)
        printf("0 is not prime because a prime number must be strictly greater than 0.\n");
    else if(n == 1ul)
        printf("1 is not prime because it has not got two distinct divisors.\n");
    else if(basic_primality_check(n, &first_divisor))
        printf("%lu is prime.\n", n);
    else
        printf("%lu is not prime, its first divisor is %lu.\n", n, first_divisor);

    return validate ? validate_basic_primality_check(n, first_divisor) : true;
}

bool validate_basic_primality_check(unsigned long n, unsigned long first_divisor) {
    for(unsigned long i = 2ul, upper_bound = ceil(sqrt(n)); i <= upper_bound; i++)
        if(n % i == 0ul)
            return i == first_divisor;

    return !first_divisor;
}

void ask_bpc_module_options(bpc_module_options_t* options) {
    char c = 0;

    fprintf(stderr, "Would you like to validate the result of the basic_primality_check module? (Y/N) ");
    scanf(" %c", &c);

    printf("\n");

    options->validate_bpc_module = tolower(c) == 'y';
}

void parse_bpc_module_options(int argc, char** argv, int offset, bpc_module_options_t* options) {
    options->cli_valid = false;

    if(offset < 0 || offset >= argc)
        return;

    if(argc - offset == 1)
        fprintf(stderr, "Hint: if you need to read the usage details, run with --help or -h option.\n\n");
    else if(argc - offset == 2 && (strcmp(argv[offset + 1], "-h") == 0 || strcmp(argv[offset + 1], "--help") == 0)) {
        fprintf(stderr, "%s\n\n", argv[offset]);
        fprintf(stderr, "Usage: %s [-h | --help] | [[-y | -n] [number]]\n\n", argv[offset]);

        fprintf(stderr, "Options:\n\t-h (aliases --help): shows this message.\n\t-y: flags to validate a module's result.\n\t-n: flags not to validate a module's result.\n\tnumber: the number on which to perform the primality check.\n\n");

        fprintf(stderr, "Synopsis:\n\tThe order of the options must follow the one specified in the usage. The options names are not case-sensitive.\n\tIf one option is specified, it must be exactly one between '-y' and '-n'.\n\n\tIf no option is specified or the usage is not valid, the user will be asked on the standard input.\n");

        exit(1);
    } else if(argc - offset >= 2) {
        if(strcmp(argv[offset + 1], "-y") == 0 || strcmp(argv[offset + 1], "-Y") == 0 || strcmp(argv[offset + 1], "-n") == 0 || strcmp(argv[offset + 1], "-N") == 0)
            options->cli_valid = true, options->validate_bpc_module = tolower(argv[offset + 1][1]) == 'y';

        if(!options->cli_valid || argc - offset == 3) {
            errno = 0;
            char* end = NULL;

            options->n = strtoul(argv[offset + 1 + (unsigned int)(argc - offset == 3)], &end, 10);

            /* If no valid number had been passed, it sets `options->n` to 0. */
            if(*end)
                options->n = 0, fprintf(stderr, "CLI error: the input provided as the number in base 10 is not valid.\n");
            else if((options->n == ULONG_MAX && errno == ERANGE))
                options->n = 0, fprintf(stderr, "CLI error: the provided number in base 10 as the first parameter exceeds the maximum value.\n");
            else
                options->cli_valid = true;
        }
    }
}

#ifdef STAND_ALONE
#   include "decls.h"

    int main(int argc, char** argv) {
        /* Declaration and initialization of the module options buffer. */
        bpc_module_options_t options = {false, false, 0lu};

        /*
          Typically, when a POSIX program is run without parameters it either
          prints the usage details and exits, or logs an error and asks the user
          to run again with the option that shows up the usage details and exits.

          For this program, it was decided to adopt the second behaviour.

          Usually, the exit value is different from 0; in this case 1 was adopted.
        */
        parse_bpc_module_options(argc, argv, 0, &options);

        /* If the module's CLI options are not valid, it asks for them on the standard input. */
        if(!options.cli_valid)
            ask_bpc_module_options(&options);

        fprintf(stderr, "Running the basic_primality_check module...\n\n");
        if(!run_basic_primality_check(options.n, options.validate_bpc_module))
            return fprintf(stderr, "The basic_primality_check module failed.\n"), 1;
        else if(options.validate_bpc_module)
            fprintf(stderr, "The basic_primality_check module succeeded.\n");

        return 0;
    }
#endif /* defined(STAND_ALONE) */
