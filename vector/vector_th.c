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
 * \brief Provides the implementation of the demo host.
**/

/*
  The compilation flag STAND_ALONE should not be used on this source file
  since it is not a module and it is not designed to produce object files.
*/
#ifdef STAND_ALONE
#   undef STAND_ALONE
#   error "The STAND_ALONE compilation flag can not be used on the demo host source file."
#endif /* defined(STAND_ALONE) */

#include "decls.h"
#include "config.h"
#include "fill_vector.h"
#include "find_zero.h"
#include "bubble_sort.h"
#include "bubble_sort_and_merge.h"

int main(int argc, char** argv) {
    char c = 0;

    /* Declaration and initialization of the modules options buffers. */
    bs_module_options_t bs_opt = {false, false};
    bsam_module_options_t bsam_opt = {false, false, false};

    /*
      Typically, when a POSIX program is run without parameters it either
      prints the usage details and exits, or logs an error and asks the user
      to run again with the option that shows up the usage details and exits.

      For this program, it was decided to adopt the second behaviour with the
      exception of not exiting.

      Usually, the exit value is different from 0; in this case 1 was adopted.
    */
    if(argc == 1)
        fprintf(stderr, "Hint: if you need to read the usage details, run with --help or -h option.\n\n");
    if(argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        fprintf(stderr, "%s\n\n", argv[0]);
        fprintf(stderr, "Usage: %s [-h | --help] | [<<-fv | --fill-vector> | <-fz | --find-zero> | <-bs | --bubble-sort> | <-bsam | --bubble-sort-and-merge>> [module options]]\n\n", argv[0]);

        fprintf(stderr, "Options:\n\t--help (alias -h): shows this message.\n\t--fill-vector (alias -fv): flags to run the fill_vector module.\n\t--find-zero (alias -fz): flags to run the find_zero module.\n\t--bubble-sort (alias -bs): flags to run the bubble_sort module.\n\t--bubble-sort-and-merge (alias -bsam): flags to run the bubble_sort_and_merge module.\n\tmodule options: a list of options that are forwarded to the module.\n\n");

        fprintf(stderr, "Synopsis:\n\tThe order of the options must follow the one specified in the usage. The options names, except for the module options names, are case-sensitive.\n\n\tIf no option is specified or the usage is not valid, the user will be asked on the standard input.\n");

        return 1;
    } else if(argc >= 2) {
        /*
          At this stage we are sure that the demo host's usage details are not being asked. So, it
          determines which module is asked to be run and if the module's usage details are being asked.

          If the module's usage details are asked, it concatenates this binary filename (argv[0]) with
          the corresponding module invocation option so that the module's binary filename is replaced.
        */
        bool help = argc == 3 && (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0);

        if(strcmp(argv[1], "-fv") == 0 || strcmp(argv[1], "--fill-vector") == 0)
            c = '1';
        else if(strcmp(argv[1], "-fz") == 0 || strcmp(argv[1], "--find-zero") == 0)
            c = '2';
        else if(strcmp(argv[1], "-bs") == 0 || strcmp(argv[1], "--bubble-sort") == 0)
            c = '3', parse_bs_module_options(help ? 2 : argc, help ? (char**)(char*[2]){strcat(argv[0], " --bubble-sort"), "--help"} : argv, !help, &bs_opt);
        else if(strcmp(argv[1], "-bsam") == 0 || strcmp(argv[1], "--bubble-sort-and-merge") == 0)
            c = '4', parse_bsam_module_options(help ? 2 : argc, help ? (char**)(char*[2]){strcat(argv[0], " --bubble-sort-and-merge"), "--help"} : argv, !help, &bsam_opt);
    }

    do {
        /* If no valid module invocation option has been specified in the CLI, it asks the user which module they would like to run. */
        if(!(c >= '1' && c <= '4')) {
            fprintf(stderr, "Select the module you would like to run:\n  (1)  fill_vector module\n  (2)  find_zero module\n  (3)  bubble_sort module\n  (4)  bubble_sort_and_merge module\n\nOr type 'q' for quitting.\n\n> ");

            scanf(" %c", &c);

            if(tolower(c) != 'q')
                printf("\n");
        }

        /*
          It runs the corresponding module: if the module's CLI options are not valid, it asks for them on the standard input.
          If c is 'q', then it does nothing.

          Otherwise, it prints an error asking the user to enter a valid option.
        */
        if(c == '1')
            fprintf(stderr, "Running fill_vector module...\n\n"), run_fill_vector(), fprintf(stderr, "\n");
        else if(c == '2') {
            fprintf(stderr, "Running fill_vector module...\n\n"), run_fill_vector();
            fprintf(stderr, "\nRunning find_zero module...\n\n"), run_find_zero();
            fprintf(stderr, "\n");
        } else if(c == '3') {
            if(!bs_opt.cli_valid)
                ask_bs_module_options(&bs_opt);

            fprintf(stderr, "Running fill_vector module...\n\n"), run_fill_vector();

            fprintf(stderr, "\nRunning bubble_sort module...\n\n");
            if(!run_bubble_sort(bs_opt.validate_bs_module))
                return fprintf(stderr, "The bubble_sort module failed.\n"), 1;
            else if(bs_opt.validate_bs_module)
                fprintf(stderr, "The bubble_sort module succeeded.\n\n");
            else
                fprintf(stderr, "\n");

            bs_opt = (bs_module_options_t){false, false};
        } else if(c == '4') {
                if(!bsam_opt.cli_valid)
                    ask_bsam_module_options(&bsam_opt);

                fprintf(stderr, "Running fill_vector module...\n\n"), run_fill_vector();

                fprintf(stderr, "\nRunning bubble_sort module...\n\n");
                if(!run_bubble_sort(bsam_opt.validate_bs_module))
                    return fprintf(stderr, "The bubble_sort module failed.\n"), 1;
                else if(bsam_opt.validate_bs_module)
                    fprintf(stderr, "The bubble_sort module succeeded.\n");

                fprintf(stderr, "\nRunning bubble_sort_and_merge module...\n\n");
                if(!run_bubble_sort_and_merge(bsam_opt.validate_bsam_module))
                    return fprintf(stderr, "The bubble_sort_and_merge module failed.\n"), 1;
                else if(bsam_opt.validate_bsam_module)
                    fprintf(stderr, "The bubble_sort_and_merge module succeeded.\n\n");
                else
                    fprintf(stderr, "\n");

                bsam_opt = (bsam_module_options_t){false, false, false};
        } else if(tolower(c) != 'q')
            fprintf(stderr, "\n\nYour choice is not valid, type the number for selecting the desired module or 'q' to exit.\n");

        if(tolower(c) != 'q')
            c = 0;
    } while(tolower(c) != 'q');

    return 0;
}
