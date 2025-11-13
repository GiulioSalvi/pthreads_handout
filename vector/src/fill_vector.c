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
 * \brief Provides implementation of fill_vector module's functions.
**/

#include "config.h"
#include "fill_vector.h"

void fill_vector(void) {
    printf("Starting to populate the vector with %u random numbers using %u threads...\n", VECTOR_SIZE, NTHREADS);

    pthread_t threads[NTHREADS];
    data_t thread_data[NTHREADS];

    unsigned int segment_size = VECTOR_SIZE / NTHREADS;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    /* Workers creations with parameters assignment. */
    for(unsigned int i = 0u; i < NTHREADS; i++) {
        thread_data[i].start_idx = i * segment_size;
        thread_data[i].end_idx = (i == NTHREADS - 1u) ? VECTOR_SIZE : (i + 1u) * segment_size;
        thread_data[i].seed = time(NULL) ^ (i * 123456789); /* unique seed per thread, not a good idea to get them this way! */

        pthread_create(&threads[i], NULL, fill_segment, &thread_data[i]);
    }

    /* Waits for all the workers to finish. */
    for(unsigned int i = 0u; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time: %.4f seconds.\n\n", elapsed);
}

void* fill_segment(void* arg) {
    data_t* args = (data_t*) arg;

    for(unsigned int i = args->start_idx; i < args->end_idx; i++)
        vector[i] = rand_r(&args->seed) % 100000;

    pthread_exit(NULL);
}

void run_fill_vector(void) {
    fill_vector();

    printf("First 10 numbers: ");
    for(int i = 0; i < 10; i++)
        printf("%d ", vector[i]);
    printf("\n");
}

#ifdef STAND_ALONE
#   include "decls.h"

    int main(int argc, char** argv) {
        /* This message is printed for letting the user know that if this module is invoked with parameters, they are ignored. */
        if(argc > 1)
            fprintf(stderr, "%s\n\nUsage: %s\n\nFurther parameters are ignored.\n\n", argv[0], argv[0]);

        fprintf(stderr, "Running fill_vector module...\n\n"), run_fill_vector();

        return 0;
    }
#endif /* defined(STAND_ALONE) */
