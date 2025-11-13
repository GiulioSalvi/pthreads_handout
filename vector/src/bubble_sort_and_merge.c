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
 * \brief Provides implementation of bubble_sort_and_merge module's function.
**/

#include "config.h"
#include "bubble_sort.h"
#include "bubble_sort_and_merge.h"

void bubble_sort_and_merge(void) {
    unsigned int segment_size = VECTOR_SIZE / NTHREADS;

    printf("Starting to bubble sort and merge %u segments, whose width is %u, using %u threads...\n", NTHREADS, segment_size, NTHREADS);

    data_search_t args[NTHREADS];

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    /* Generates the ranges of each segment. */
    for(unsigned int i = 0u; i < NTHREADS; i++) 
        args[i] = (data_search_t) {
            .start_idx = i * segment_size,
            .end_idx = (i == NTHREADS - 1u) ? VECTOR_SIZE : (i + 1u) * segment_size
        };

    unsigned int end_of_sorted_area = args[0u].end_idx;

    /* Merges the segments sequentially. */
    for(unsigned int i = 1u; i < NTHREADS; i++) {
        unsigned int current_segment_end = args[i].end_idx;

        merge(0u, end_of_sorted_area - 1u, current_segment_end - 1u);

        end_of_sorted_area = current_segment_end;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time: %.4f seconds.\n\n", elapsed);
}

void merge(unsigned int left, unsigned int middle, unsigned int right) {
    unsigned int i = 0u, j = 0u, k = left, len1 = middle - left + 1u, len2 = right - middle;
    int buf1[len1], buf2[len2]; /* Unfortunately, it is not possible to initialize arrays whose length is not known at compile-time. */

    /*
      Note that the variables i below obfuscate the function-wide variable i. Hence,
      the function-wide variable i remains to 0 at least until the following while
      loop: this is important to guarantee the correct behaviour of the algorithm.
    */
    for(unsigned int i = 0u; i < len1; i++)
        buf1[i] = vector[left + i];
    for(unsigned int i = 0u; i < len2; i++)
        buf2[i] = vector[middle + i + 1u];

    while(i < len1 && j < len2)
        vector[k++] = buf1[i] <= buf2[j] ? buf1[i++] : buf2[j++];

    while(i < len1)
        vector[k++] = buf1[i++];
    while(j < len2)
        vector[k++] = buf2[j++];
}

bool run_bubble_sort_and_merge(bool validate) {
    bubble_sort_and_merge();

    return validate ? validate_bubble_sort_and_merge() : true;
}

bool validate_bubble_sort_and_merge(void) {
    return check_segment(0u, VECTOR_SIZE);
}

void ask_bsam_module_options(bsam_module_options_t* options) {
    char c1 = 0, c2 = 0;

    fprintf(stderr, "Would you like to validate the result of the bubble_sort module? (Y/N) ");
    scanf(" %c", &c1);

    fprintf(stderr, "Would you like to validate the result of the bubble_sort_and_merge module? (Y/N) ");
    scanf(" %c", &c2);

    printf("\n");

    options->validate_bs_module = tolower(c1) == 'y';
    options->validate_bsam_module = tolower(c2) == 'y';
}

void parse_bsam_module_options(int argc, char** argv, int offset, bsam_module_options_t* options) {
    options->cli_valid = false;

    if(offset < 0 || offset >= argc)
        return;

    if(argc - offset == 1)
        fprintf(stderr, "Hint: if you need to read the usage details, run with --help or -h option.\n\n");
    else if(argc - offset == 2 && (strcmp(argv[offset + 1], "-h") == 0 || strcmp(argv[offset + 1], "--help") == 0)) {
        fprintf(stderr, "%s\n\n", argv[offset]);
        fprintf(stderr, "Usage: %s [-h | --help] | [-a <-y | -n>] | [<-y | -n> <-y | -n>]\n\n", argv[offset]);

        fprintf(stderr, "Options:\n\t-h (aliases --help): shows this message.\n\t-a: indicates that the following flag applies for all the used module, if they support result validation.\n\t-y: flags to validate a module's result.\n\t-n: flags not to validate a module's result.\n\n");

        fprintf(stderr, "Synopsis:\n\tThe order of the options must follow the one specified in the usage. The options names are not case-sensitive.\n\tThe option '-a' must be followed exactly by one option, which must be either '-y' or '-n'.\n\tIf the option '-a' is not used, there may be exactly two options, so that each of them must be either '-y' or '-n'.\n\n\tIf no option is specified or the usage is not valid, the user will be asked on the standard input.\n");

        exit(1);
    } else if(argc - offset == 3) {
        if(strcmp(argv[offset + 1], "-a") == 0 || strcmp(argv[offset + 1], "-A") == 0) {
            if(strcmp(argv[offset + 2], "-y") == 0 || strcmp(argv[offset + 2], "-Y") == 0 || strcmp(argv[offset + 2], "-n") == 0 || strcmp(argv[offset + 2], "-N") == 0)
                options->cli_valid = true, options->validate_bs_module = options->validate_bsam_module = tolower(argv[offset + 2][1]) == 'y';
        } else if((strcmp(argv[offset + 1], "-y") == 0 || strcmp(argv[offset + 1], "-Y") == 0 || strcmp(argv[offset + 1], "-n") == 0 || strcmp(argv[offset + 1], "-N") == 0) &&
            (strcmp(argv[offset + 2], "-y") == 0 || strcmp(argv[offset + 2], "-Y") == 0 || strcmp(argv[offset + 2], "-n") == 0 || strcmp(argv[offset + 2], "-N") == 0))
                options->cli_valid = true, options->validate_bs_module = tolower(argv[offset + 1][1]), options->validate_bsam_module = tolower(argv[offset + 2][1]);
    }
}

#ifdef STAND_ALONE
#   include "decls.h"
#   include "fill_vector.h"

    int main(int argc, char** argv) {
        /* Declaration and initialization of the module options buffer. */
        bsam_module_options_t options = {false, false, false};

        /*
          Typically, when a POSIX program is run without parameters it either
          prints the usage details and exits, or logs an error and asks the user
          to run again with the option that shows up the usage details and exits.

          For this program, it was decided to adopt the second behaviour.

          Usually, the exit value is different from 0; in this case 1 was adopted.
        */
        parse_bsam_module_options(argc, argv, 0, &options);

        /* If the module's CLI options are not valid, it asks for them on the standard input. */
        if(!options.cli_valid)
            ask_bsam_module_options(&options);

        fprintf(stderr, "Running fill_vector module...\n\n"), run_fill_vector();

        fprintf(stderr, "Running bubble_sort module...\n\n");
        if(!run_bubble_sort(options.validate_bs_module))
            return fprintf(stderr, "The bubble_sort module failed.\n"), 1;

        fprintf(stderr, "Running bubble_sort_and_merge module...\n\n");
        if(!run_bubble_sort_and_merge(options.validate_bsam_module))
            return fprintf(stderr, "The bubble_sort_and_merge module failed.\n"), 1;

        return 0;
    }
#endif /* defined(STAND_ALONE) */
