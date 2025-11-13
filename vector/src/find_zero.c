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
 * \brief Provides implementation of find_zero module's functions.
**/

#include "config.h"
#include "find_zero.h"

bool find_zero(void) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    printf("Starting to look up for 0 among %u random numbers using %u threads...\n", VECTOR_SIZE, NTHREADS);

    pthread_t threads[NTHREADS];
    data_search_t args[NTHREADS];

    unsigned int segment_size = VECTOR_SIZE / NTHREADS;

    /* Workers creation with their range assignment. */
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        args[i].start_idx = i * segment_size;
        args[i].end_idx = (i == NTHREADS - 1u) ? VECTOR_SIZE : (i + 1u) * segment_size;

        pthread_create(&threads[i], NULL, search_zero, &args[i]);
    }

    /* Waits for all the workers to finish. */
    for(unsigned int i = 0u; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time: %.4f seconds.\n\n", elapsed);

    return zero_found;
}

void* search_zero(void* arg) {
    data_search_t* args = (data_search_t*) arg;

    /*
      The worker looks for zero in the vector; if zero has already been found in the vector, it
      exits. Please note that it is not relevant whether zero_found is atomic, i.e. not subject
      to race conditions, since the workers will perform the same write operation on zero_found.
    */
    for(unsigned int i = args->start_idx; i < args->end_idx && !zero_found; i++)
        if(!vector[i])
            zero_found = true;

    return NULL;
}

void run_find_zero(void) {
    printf("%s\n", find_zero() ? "0 is in the vector." : "0 is not in the vector.");
}

#ifdef STAND_ALONE
#   include "decls.h"
#   include "fill_vector.h"

    int main(int argc, char** argv) {
        /* This message is printed for letting the user know that if this module is invoked with parameters, they are ignored. */
        if(argc > 1)
            fprintf(stderr, "%s\n\nUsage: %s\n\nFurther parameters are ignored.\n\n", argv[0], argv[0]);

        fprintf(stderr, "Running fill_vector module...\n\n"), run_fill_vector();
        fprintf(stderr, "Running find_zero module...\n\n"), run_find_zero();

        return 0;
    }
#endif /* defined(STAND_ALONE) */
