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
 * \brief Provides implementation of bubble_sort module's function.
**/

#include "config.h"
#include "bubble_sort.h"

void bubble_sort(void) {
    unsigned int segment_size = VECTOR_SIZE / NTHREADS;

    printf("Starting to bubble sort %u segments, whose width is %u, using %u threads...\n", NTHREADS, segment_size, NTHREADS);

    pthread_t threads[NTHREADS];
    data_search_t args[NTHREADS];

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    /* Workers creation with their ranges assignment. */
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        args[i].start_idx = i * segment_size;
        args[i].end_idx = (i == NTHREADS - 1u) ? VECTOR_SIZE : (i + 1u) * segment_size;

        pthread_create(&threads[i], NULL, bubble_sort_segment, &args[i]);
    }

    /* Waits for all the workers to finish. */
    for(unsigned int i = 0u; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time: %.4f seconds.\n\n", elapsed);
}

void* bubble_sort_segment(void* arg) {
    data_search_t* args = (data_search_t*)arg;
    unsigned int size = args->end_idx - args->start_idx;

    for(unsigned int i = 0u; i < size - 1u; i++) {
        bool swapped = false;

        for(unsigned int j = args->start_idx; j < args->end_idx - i - 1u; j++)
            if(vector[j] > vector[j + 1u]) {
                int _ = vector[j + 1u];
                vector[j + 1u] = vector[j];
                vector[j] = _;
                swapped = true;
            }

        if(!swapped)
            break;
    }

    return NULL;
}

bool run_bubble_sort(bool validate) {
    bubble_sort();

    return validate ? validate_bubble_sort() : true;
}

bool validate_bubble_sort(void) {
    unsigned int segment_size = VECTOR_SIZE / NTHREADS;

    for(unsigned int i = 0u; i < NTHREADS; i++)
        if(!check_segment(i * segment_size, i == NTHREADS - 1 ? VECTOR_SIZE : (i + 1) * segment_size))
            return false;

    return true;
}

bool check_segment(unsigned int start_idx, unsigned int end_idx) {
    for(unsigned int i = start_idx + 1u; i < end_idx; i++)
        if(vector[i] < vector[i - 1u])
            return false;

    return true;
}

void ask_bs_module_options(bs_module_options_t* options) {
    char c = 0;

    fprintf(stderr, "Would you like to validate the result of the bubble_sort module? (Y/N) ");
    scanf(" %c", &c);

    printf("\n");

    options->validate_bs_module = tolower(c) == 'y';
}

void parse_bs_module_options(int argc, char** argv, int offset, bs_module_options_t* options) {
    options->cli_valid = false;

    if(offset < 0 || offset >= argc)
        return;

    if(argc - offset == 1)
        fprintf(stderr, "Hint: if you need to read the usage details, run with --help or -h option.\n\n");
    else if(argc - offset == 2 && (strcmp(argv[offset + 1], "-h") == 0 || strcmp(argv[offset + 1], "--help") == 0)) {
        fprintf(stderr, "%s\n\n", argv[offset]);
        fprintf(stderr, "Usage: %s [-h | --help] | [-y | -n]\n\n", argv[offset]);

        fprintf(stderr, "Options:\n\t-h (aliases --help): shows this message.\n\t-y: flags to validate a module's result.\n\t-n: flags not to validate a module's result.\n\n");

        fprintf(stderr, "Synopsis:\n\tThe order of the options must follow the one specified in the usage. The options names are not case-sensitive.\n\tIf one option is specified, it must be exactly one between '-y' and '-n'.\n\n\tIf no option is specified or the usage is not valid, the user will be asked on the standard input.\n");

        exit(1);
    } else if(argc - offset == 2)
        if(strcmp(argv[offset + 1], "-y") == 0 || strcmp(argv[offset + 1], "-Y") == 0 || strcmp(argv[offset + 1], "-n") == 0 || strcmp(argv[offset + 1], "-N") == 0)
            options->cli_valid = true, options->validate_bs_module = tolower(argv[offset + 1][1]) == 'y';
}

#ifdef STAND_ALONE
#   include "decls.h"
#   include "fill_vector.h"

    int main(int argc, char** argv) {
        /* Declaration and initialization of the module options buffer. */
        bs_module_options_t options = {false, false};

        /*
          Typically, when a POSIX program is run without parameters it either
          prints the usage details and exits, or logs an error and asks the user
          to run again with the option that shows up the usage details and exits.

          For this program, it was decided to adopt the second behaviour.

          Usually, the exit value is different from 0; in this case 1 was adopted.
        */
        parse_bs_module_options(argc, argv, 0, &options);

        /* If the module's CLI options are not valid, it asks for them on the standard input. */
        if(!options.cli_valid)
            ask_bs_module_options(&options);

        fprintf(stderr, "Running fill_vector module...\n\n"), run_fill_vector();

        fprintf(stderr, "Running bubble_sort module...\n\n");
        if(!run_bubble_sort(options.validate_bs_module))
            return fprintf(stderr, "The bubble_sort module failed.\n"), 1;
        else if(options.validate_bs_module)
            fprintf(stderr, "The bubble_sort module succeeded.\n");

        return 0;
    }
#endif /* defined(STAND_ALONE) */
