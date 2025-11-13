# Copyright (C) 2025  Giulio Salvi
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# The binary files make-rules have been flagged as PHONY so that it is not
# necessary to remove them manually or to use the make-rule `clean` for recompile them.

SHELL := /usr/bin/env bash
.PHONY: all all_exec all_obj setup clear bin/divisors_th bin/basic_primality_check bin/primality_check bin/divisors_and_primality_check bin/basic_primality_check.o bin/primality_check.o bin/divisors_and_primality_check.o

all: all_exec all_obj

setup:
	mkdir bin

clear:
	rm bin/*

all_exec: bin/divisors_th bin/basic_primality_check bin/primality_check bin/divisors_and_primality_check

bin/divisors_th: bin/basic_primality_check.o bin/primality_check.o bin/divisors_and_primality_check.o h/decls.h h/config.h h/basic_primality_check.h h/primality_check.h h/divisors_and_primality_check.h divisors_th.c
	gcc bin/basic_primality_check.o bin/primality_check.o bin/divisors_and_primality_check.o divisors_th.c -o bin/divisors_th -Ih/ --std=c17 -Wall -Wextra -Wpedantic

# Executable modules

bin/basic_primality_check: h/decls.h h/config.h h/basic_primality_check.h src/basic_primality_check.c
	gcc src/basic_primality_check.c -o bin/basic_primality_check -DSTAND_ALONE -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/primality_check: h/decls.h h/config.h h/primality_check.h src/primality_check.c
	gcc src/primality_check.c -o bin/primality_check -DSTAND_ALONE -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/divisors_and_primality_check: h/decls.h h/config.h h/divisors_and_primality_check.h src/divisors_and_primality_check.c
	gcc src/divisors_and_primality_check.c -o bin/divisors_and_primality_check -DSTAND_ALONE -Ih/ --std=c17 -Wall -Wextra -Wpedantic

# Module objects

all_obj: bin/basic_primality_check.o bin/primality_check.o bin/divisors_and_primality_check.o

bin/basic_primality_check.o: h/config.h h/basic_primality_check.h src/basic_primality_check.c
	gcc -c src/basic_primality_check.c -o bin/basic_primality_check.o -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/primality_check.o: h/config.h h/primality_check.h src/primality_check.c
	gcc -c src/primality_check.c -o bin/primality_check.o -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/divisors_and_primality_check.o: h/config.h h/divisors_and_primality_check.h src/divisors_and_primality_check.c
	gcc -c src/divisors_and_primality_check.c -o bin/divisors_and_primality_check.o -Ih/ --std=c17 -Wall -Wextra -Wpedantic
