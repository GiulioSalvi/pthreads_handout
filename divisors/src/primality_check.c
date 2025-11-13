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
 * \brief Provides implementation of primality_check module's functions.
**/

#include "primality_check.h"

bool primality_check(unsigned long n, unsigned long* first_divisor) {
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
      Hence, the complete search range is between 3 and sqrt(n) which is splitted among the workers.

      Here, the even numbers are not checked because, at this stage, we are sure that the number is not even.
    */
    unsigned long upper_bound = ceil(sqrt(n));
    unsigned long breadth = upper_bound >= 3ul ? (upper_bound - 3ul) / 2ul + 1ul : 0ul;
    unsigned long delta = ceil((double)breadth / NTHREADS);
    if(delta == 0ul)
        delta = 1ul;

    pthread_t threads[NTHREADS];
    pc_module_param_t params[NTHREADS];

    /* Initializes the shared data among the workers. */
    pc_module_shared_t shared_data = (pc_module_shared_t) {
        .first_divisor = 0ul,
        .completed_workers = 0ul
    };
    pthread_cond_init(&shared_data.cond, NULL);
    pthread_mutex_init(&shared_data.mutex, NULL);

    /* Workers parameters initialization. Boundary checks are performed. */
    fprintf(stderr, "Setting up parameters for workers with delta %lu...\n", delta);
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        unsigned long current_from = 3ul + i * delta * 2ul;
        unsigned long current_to = 3ul + (i + 1u) * delta * 2ul;

        if(current_from > upper_bound)
            current_to = current_from = upper_bound + 1ul;

        if(current_to > upper_bound + 1ul)
            current_to = upper_bound + 1ul;

        params[i] = (pc_module_param_t) {
            .n = n,
            .from = current_from,
            .to = current_to,
            .shared_data = &shared_data
        };
    }

    /* Starts all the workers. */
    fprintf(stderr, "Starting up workers...\n\n");
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        if(pthread_create(&threads[i], NULL, pc_module_has_divisor, &params[i]) == EAGAIN)
            fprintf(stderr, "Resources finished when starting worker %u.\n", i + 1u), exit(EXIT_FAILURE);

        print_pc_module_parameters(stderr, &params[i], i + 1u);
    }

    /* Waits for a worker to signal that it had found a divisor, until all the workers had finished. */
    fprintf(stderr, "\nWaiting for a signal...\n\n");
    pthread_mutex_lock(&shared_data.mutex);

    while(shared_data.first_divisor == 0ul && shared_data.completed_workers < NTHREADS)
        pthread_cond_wait(&shared_data.cond, &shared_data.mutex);

    fprintf(stderr, "Catching a result after %u worker(s) had completed...\n\n", shared_data.completed_workers);
    *first_divisor = shared_data.first_divisor;

    pthread_mutex_unlock(&shared_data.mutex);

    /* Cancels all the alive workers */
    fprintf(stderr, "Cancelling all the workers...\n");
    for(unsigned int i = 0u; i < NTHREADS; i++)
        pthread_cancel(threads[i]), pthread_join(threads[i], NULL), fprintf(stderr, "Worker %u cancelled.\n", i + 1u);

    /* Frees the shared data. */
    pthread_cond_destroy(&shared_data.cond);
    pthread_mutex_destroy(&shared_data.mutex);

    fprintf(stderr, "\n");

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time: %.4f seconds.\n\n", elapsed);

    return !*first_divisor;
}

void* pc_module_has_divisor(void* arg) {
    bool found = false;
    pc_module_param_t* args = (pc_module_param_t*) arg;

    for(unsigned long i = args->from; i < args->to && !found; i += 2ul) {
        /* Checks if the worker has been cancelled. */
        pthread_testcancel();

        if(args->n % i == 0ul) {
            pthread_mutex_lock(&args->shared_data->mutex);

            /* Stores the divisor if none have not been found yet. */
            if(args->shared_data->first_divisor == 0ul)
                args->shared_data->first_divisor = i, found = true;

            pthread_cond_signal(&args->shared_data->cond);
            pthread_mutex_unlock(&args->shared_data->mutex);
        }
    }

    if(found)
        return NULL;

    /*
      Increases the counter of the completed workers. If it is the
      last, it signals to the main thread that they have finished.
    */
    pthread_mutex_lock(&args->shared_data->mutex);
    args->shared_data->completed_workers++;

    if(args->shared_data->first_divisor == 0ul && args->shared_data->completed_workers == NTHREADS)
        pthread_cond_signal(&args->shared_data->cond);

    pthread_mutex_unlock(&args->shared_data->mutex);

    return NULL;
}

void print_pc_module_parameters(FILE* out_file, pc_module_param_t* params, unsigned int thread_id) {
    fprintf(out_file, "Worker %u started for checking primality on %lu in interval [%lu,%lu) (whose width is %lu).\n", thread_id, params->n, params->from, params->to, params->to - params->from);
}

bool run_primality_check(unsigned long n, bool validate) {
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
    else if(primality_check(n, &first_divisor))
        printf("%lu is prime.\n", n);
    else
        printf("%lu is not prime, its first divisor is %lu.\n", n, first_divisor);

    return validate ? validate_primality_check(n, first_divisor) : true;
}

bool validate_primality_check(unsigned long n, unsigned long first_divisor) {
    for(unsigned long i = 2ul, upper_bound = ceil(sqrt(n)); i <= upper_bound; i++)
        if(n % i == 0ul)
            return i == first_divisor;

    return !first_divisor;
}

void ask_pc_module_options(pc_module_options_t* options) {
    char c = 0;

    fprintf(stderr, "Would you like to validate the result of the primality_check module? (Y/N) ");
    scanf(" %c", &c);

    printf("\n");

    options->validate_pc_module = tolower(c) == 'y';
}

void parse_pc_module_options(int argc, char** argv, int offset, pc_module_options_t* options) {
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
            options->cli_valid = true, options->validate_pc_module = tolower(argv[offset + 1][1]) == 'y';

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
        pc_module_options_t options = {false, false, 0lu};

        /*
          Typically, when a POSIX program is run without parameters it either
          prints the usage details and exits, or logs an error and asks the user
          to run again with the option that shows up the usage details and exits.

          For this program, it was decided to adopt the second behaviour.

          Usually, the exit value is different from 0; in this case 1 was adopted.
        */
        parse_pc_module_options(argc, argv, 0, &options);

        /* If the module's CLI options are not valid, it asks for them on the standard input. */
        if(!options.cli_valid)
            ask_pc_module_options(&options);

        fprintf(stderr, "Running the primality_check module...\n\n");
        if(!run_primality_check(options.n, options.validate_pc_module))
            return fprintf(stderr, "The primality_check module failed.\n"), 1;
        else if(options.validate_pc_module)
            fprintf(stderr, "The primality_check module succeeded.\n");

        return 0;
    }
#endif /* defined(STAND_ALONE) */
