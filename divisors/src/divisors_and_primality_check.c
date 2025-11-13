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
 * \brief Provides implementation of divisors_and_primality_check module's functions.
**/

#include "divisors_and_primality_check.h"

bool divisors_and_primality_check(unsigned long n, divisor_list_t** divisors_list) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    /*
      Even if the code with which this function is shipped checks independently
      the base cases, since this is a public function, it handles the base cases.
    */
    if(n <= 2ul)
        return n == 2ul;

    /*
      It can be shown that a composite integer has at least a divisor which is at most sqrt(n).
      Hence, the complete search range is between 2 and sqrt(n) which is splitted among the workers.
    */
    bool divisor_found = false;
    unsigned long upper_bound = ceil(sqrt(n));
    unsigned long breadth = upper_bound >= 2ul ? upper_bound - 1ul : 0ul;
    unsigned long delta = ceil((double)breadth / NTHREADS);

    pthread_t threads[NTHREADS];
    dapc_module_param_t params[NTHREADS];

    /* Workers parameters initialization. Boundary checks are performed. */
    fprintf(stderr, "Setting up parameters for workers with delta %lu...\n", delta);
    for(unsigned int i = 0; i < NTHREADS; i++) {
        unsigned long current_from = 2ul + i * delta;
        unsigned long current_to = 2ul + (i + 1u) * delta;

        if(current_from > upper_bound)
            current_to = current_from = upper_bound + 1ul;

        if(current_to > upper_bound + 1ul)
            current_to = upper_bound + 1ul;

        params[i] = (dapc_module_param_t) {
            .n = n,
            .from = current_from,
            .to = current_to,
            .divisors_list = NULL
        };
    }

    /* Starts all the workers. */
    fprintf(stderr, "Starting up workers...\n\n");
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        if(pthread_create(&threads[i], NULL, dapc_module_has_divisor, &params[i]) == EAGAIN)
            fprintf(stderr, "Resources finished when starting worker %u.\n", i + 1u), exit(EXIT_FAILURE);

        print_dapc_module_parameters(stderr, &params[i], i + 1u);
    }

    /* Waits for all the workers to finish. */
    fprintf(stderr, "\nWaiting for a result...\n\n");
    for(unsigned int i = 0u; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    /* Concatenates all the workers' lists. */
    fprintf(stderr, "Catching a result after %u worker(s) had completed...\n\n", NTHREADS);
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        divisor_list_t* tail = get_tail(params[i].divisors_list);

        if(tail)
            tail->next = *divisors_list, *divisors_list = params[i].divisors_list, divisor_found = true;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time: %.4f seconds.\n\n", elapsed);

    return !divisor_found;
}

void* dapc_module_has_divisor(void* arg) {
    dapc_module_param_t* args = (dapc_module_param_t*) arg;

    for(unsigned long i = args->from; i < args->to; i++)
        if(args->n % i == 0ul) {
            prepend_divisor(&args->divisors_list, i);

            /*
              If i is not the square root of n it prepends also the divisor paired to i,
              in order not to have duplicates in the list since n/sqrt(n) = sqrt(n) = i.
            */
            if(i * i != args->n)
                prepend_divisor(&args->divisors_list, args->n / i);
        }

    return NULL;
}

void free_divisor_list(divisor_list_t** list) {
    divisor_list_t* cp = *list, *_;

    while((_ = cp))
        cp = _->next, free(_);

    *list = NULL;
}

unsigned long get_length(divisor_list_t* list) {
    unsigned long l = 0ul;

    while(list)
        l++, list = list->next;

    return l;
}

divisor_list_t* get_tail(divisor_list_t* list) {
    while(list && list->next)
        list = list->next;

    return list;
}

void print_list(FILE* out_file, divisor_list_t* list) {
    while(list) {
        fprintf(out_file, "%lu%s", list->divisor, list->next ? ", " : ".\n");
        list = list->next;
    }
}

bool is_in_the_list(divisor_list_t* list, unsigned long element) {
    while(list) {
        if(list->divisor == element)
            return true;

        list = list->next;
    }

    return false;
}

void prepend_divisor(divisor_list_t** list, unsigned long divisor) {
    /*
      Even if the code with which this function is shipped is memory-safe,
      since this is a public function, it performs memory-safety checks.
    */
    if(!list)
        fprintf(stderr, "Fatal error: invalid pointer to divisor list.\n"), exit(EXIT_FAILURE);

    divisor_list_t* new_el = (divisor_list_t*) malloc(sizeof(divisor_list_t));
    if(!new_el)
        fprintf(stderr, "Fatal error: failed to allocate memory for a new divisor.\n"), exit(EXIT_FAILURE);

    new_el->divisor = divisor;
    new_el->next = *list;

    *list = new_el;
}

void print_dapc_module_parameters(FILE* out_file, dapc_module_param_t* params, unsigned int thread_id) {
    fprintf(out_file, "Thread %u started for checking primality on %lu in interval [%lu,%lu) (whose width is %lu).\n", thread_id, params->n, params->from, params->to, params->to - params->from);
}

bool run_divisors_and_primality_check(unsigned long n, bool validate) {
    bool res = false;
    divisor_list_t* divisors = NULL;

    if(!n) {
        printf("Enter the number on which to perform the primality check and whose divisors must be found: ");
        scanf("%lu", &n);

        printf("\n");
    }

    if(n == 0ul)
        printf("0 is not prime because a prime number must be strictly greater than 0.\n");
    else if(n == 1ul)
        printf("1 is not prime because it has not got two distinct divisors.\n");
    else if(divisors_and_primality_check(n, &divisors))
        printf("%lu is prime.\n", n);
    else
        printf("%lu is not prime, its divisors are ", n), print_list(stdout, divisors);

    if(validate)
        res = validate_divisors_and_primality_check(n, divisors);

    free_divisor_list(&divisors);
    return validate ? res : true;
}

bool validate_divisors_and_primality_check(unsigned long n, divisor_list_t* divisors_list) {
    unsigned long divisors_number = 0ul, list_length = get_length(divisors_list);

    for(unsigned long i = 2ul, upper_bound = ceil(sqrt(n)); i <= upper_bound; i++)
        if(n % i == 0ul) {
            divisors_number++;

            if(!is_in_the_list(divisors_list, i))
                return false;

            if(i * i != n) {
                divisors_number++;

                if(!is_in_the_list(divisors_list, n / i))
                    return false;
            }
        }

    return divisors_number == list_length;
}

void ask_dapc_module_options(dapc_module_options_t* options) {
    char c = 0;

    fprintf(stderr, "Would you like to validate the result of the divisors_and_primality_check module? (Y/N) ");
    scanf(" %c", &c);

    printf("\n");

    options->validate_dapc_module = tolower(c) == 'y';
}

void parse_dapc_module_options(int argc, char** argv, int offset, dapc_module_options_t* options) {
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
            options->cli_valid = true, options->validate_dapc_module = tolower(argv[offset + 1][1]) == 'y';

        if(!options->cli_valid || argc - offset == 3) {
            errno = 0;
            char* end = NULL;

            options->n = strtoul(argv[offset + 1 + (unsigned int)(argc - offset == 3)], &end, 10);

            if(*end)
                options->n = 0, fprintf(stderr, "CLI Error: the input provided as the number in base 10 is not valid.\n");
            else if((options->n == ULONG_MAX && errno == ERANGE))
                options->n = 0, fprintf(stderr, "CLI Error: the provided number in base 10 as the first parameter exceeds the maximum value.\n");
            else
                options->cli_valid = true;
        }
    }
}

#ifdef STAND_ALONE
#   include "decls.h"

    int main(int argc, char** argv) {
        /* Declaration and initialization of the module options buffer. */
        dapc_module_options_t options = {false, false, 0lu};

        /*
          Typically, when a POSIX program is run without parameters it either
          prints the usage details and exits, or logs an error and asks the user
          to run again with the option that shows up the usage details and exits.

          For this program, it was decided to adopt the second behaviour.

          Usually, the exit value is different from 0; in this case 1 was adopted.
        */
        parse_dapc_module_options(argc, argv, 0, &options);

        /* If the module's CLI options are not valid, it asks for them on the standard input. */
        if(!options.cli_valid)
            ask_dapc_module_options(&options);

        fprintf(stderr, "Running the divisors_and_primality_check module...\n\n");
        if(!run_divisors_and_primality_check(options.n, options.validate_dapc_module))
            return fprintf(stderr, "The divisors_and_primality_check module failed.\n"), 1;
        else if(options.validate_dapc_module)
            fprintf(stderr, "The divisors_and_primality_check module succeeded.\n");

        return 0;
    }
#endif /* defined(STAND_ALONE) */
