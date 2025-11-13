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
#include "basic_primality_check.h"
#include "primality_check.h"
#include "divisors_and_primality_check.h"

int main(int argc, char** argv) {
    char c = 0;
    char buffer[1024]; strcpy(buffer, argv[0]);

    /* Declaration and initialization of the modules options buffers. */
    bpc_module_options_t bpc_opt = {false, false, 0lu};
    pc_module_options_t pc_opt = {false, false, 0lu};
    dapc_module_options_t dapc_opt = {false, false, 0lu};

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
        fprintf(stderr, "Usage: %s [-h | --help] | [<<-bpc | --basic-primality-check> | <-pc | --primality-check> | <-dapc | --divisors-and-primality-check>> [module options]]\n\n", argv[0]);

        fprintf(stderr, "Options:\n\t--help (alias -h): shows this message.\n\t--basic-primality-check (alias -bpc): flags to run the basic_primality_check module.\n\t--primality-check (alias -pc): flags to run the primality_check module.\n\t--divisors-and-primality-check (alias -dapc): flags to run the divisors_and_primality_check module.\n\tmodule options: a list of options that are forwarded to the module.\n\n");

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

        if(strcmp(argv[1], "-bpc") == 0 || strcmp(argv[1], "--basic-primality-check") == 0)
            c = '1', parse_bpc_module_options(help ? 2 : argc, help ? (char**)(char*[2]){strcat(buffer, " --basic-primality-check"), "--help"} : argv, !help, &bpc_opt);
        else if(strcmp(argv[1], "-pc") == 0 || strcmp(argv[1], "--primality-check") == 0)
            c = '2', parse_pc_module_options(help ? 2 : argc, help ? (char**)(char*[2]){strcat(buffer, " --primality-check"), "--help"} : argv, !help, &pc_opt);
        else if(strcmp(argv[1], "-dapc") == 0 || strcmp(argv[1], "--divisors-and-primality-check") == 0)
            c = '3', parse_dapc_module_options(help ? 2 : argc, help ? (char**)(char*[2]){strcat(buffer, " --divisors-and-primality-check"), "--help"} : argv, !help, &dapc_opt);
    }

    do {
        /* If no valid module invocation option has been specified in the CLI, it asks the user which module they would like to run. */
        if(!(c >= '1' && c <= '3')) {
            fprintf(stderr, "Select the module you would like to run:\n  (1)  basic_primality_check module\n  (2)  primality_check module\n  (3)  divisors_and_primality_check module\n\nOr type 'q' for quitting.\n\n> ");

            scanf(" %c", &c);

            if(tolower(c) != 'q')
                printf("\n");
        }

        /*
          It runs the corresponding module: if the module's CLI options are not valid, it asks for them on the standard input.
          If c is 'q', then it does nothing.

          Otherwise, it prints an error asking the user to enter a valid option.
        */
        if(c == '1') {
            if(!bpc_opt.cli_valid)
                ask_bpc_module_options(&bpc_opt);

            fprintf(stderr, "Running basic_primality_check module...\n\n"); 
            if(!run_basic_primality_check(bpc_opt.n, bpc_opt.validate_bpc_module))
                return fprintf(stderr, "The basic_primality_check module failed.\n"), 1;
            else if(bpc_opt.validate_bpc_module)
                fprintf(stderr, "The basic_primality_check module succeeded.\n\n");
            else
                fprintf(stderr, "\n");

            bpc_opt = (bpc_module_options_t){false, false, 0lu};
        }
        else if(c == '2') {
            if(!pc_opt.cli_valid)
                ask_pc_module_options(&pc_opt);

            fprintf(stderr, "Running primality_check module...\n\n");
            if(!run_primality_check(pc_opt.n, pc_opt.validate_pc_module))
                return fprintf(stderr, "The primality_check module failed.\n"), 1;
            else if(pc_opt.validate_pc_module)
                fprintf(stderr, "The primality_check module succeeded.\n\n");
            else
                fprintf(stderr, "\n");

            pc_opt = (pc_module_options_t){false, false, 0lu};
        } else if(c == '3') {
                if(!dapc_opt.cli_valid)
                    ask_dapc_module_options(&dapc_opt);

                fprintf(stderr, "Running divisors_and_primality_check module...\n\n");
                if(!run_divisors_and_primality_check(dapc_opt.n, dapc_opt.validate_dapc_module))
                    return fprintf(stderr, "The divisors_and_primality_check module failed.\n"), 1;
                else if(dapc_opt.validate_dapc_module)
                    fprintf(stderr, "The divisors_and_primality_check module succeeded.\n\n");
                else
                    fprintf(stderr, "\n");

                dapc_opt = (dapc_module_options_t){false, false, 0lu};
        } else if(tolower(c) != 'q')
            fprintf(stderr, "\n\nYour choice is not valid, type the number for selecting the desired module or 'q' to exit.\n");

        if(tolower(c) != 'q')
            c = 0;
    } while(tolower(c) != 'q');

    return 0;
}
