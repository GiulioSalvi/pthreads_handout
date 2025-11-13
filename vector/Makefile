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
.PHONY: all all_obj all_exec setup clear bin/vector_th bin/fill_vector bin/find_zero bin/bubble_sort bin/bubble_sort_and_merge bin/fill_vector.o bin/find_zero.o bin/bubble_sort.o bin/bubble_sort_and_merge.o

all: all_obj all_exec

setup:
	mkdir bin

clear:
	rm bin/*

all_exec: bin/vector_th bin/fill_vector bin/find_zero bin/bubble_sort bin/bubble_sort_and_merge

bin/vector_th: bin/fill_vector.o bin/find_zero.o bin/bubble_sort.o bin/bubble_sort_and_merge.o h/decls.h h/config.h h/fill_vector.h h/find_zero.h h/bubble_sort.h h/bubble_sort_and_merge.h vector_th.c
	gcc bin/fill_vector.o bin/find_zero.o bin/bubble_sort.o bin/bubble_sort_and_merge.o vector_th.c -o bin/vector_th -Ih/ --std=c17 -Wall -Wextra -Wpedantic

# Executable modules

bin/fill_vector: h/decls.h h/config.h h/fill_vector.h src/fill_vector.c
	gcc src/fill_vector.c -o bin/fill_vector -DSTAND_ALONE -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/find_zero: bin/fill_vector.o h/decls.h h/config.h h/fill_vector.h h/find_zero.h src/find_zero.c
	gcc bin/fill_vector.o src/find_zero.c -o bin/find_zero -DSTAND_ALONE -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/bubble_sort: bin/fill_vector.o h/decls.h h/config.h h/fill_vector.h h/bubble_sort.h src/bubble_sort.c
	gcc bin/fill_vector.o src/bubble_sort.c -o bin/bubble_sort -DSTAND_ALONE -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/bubble_sort_and_merge: bin/fill_vector.o bin/bubble_sort.o h/decls.h h/config.h h/fill_vector.h h/bubble_sort.h h/bubble_sort_and_merge.h src/bubble_sort_and_merge.c
	gcc bin/fill_vector.o bin/bubble_sort.o src/bubble_sort_and_merge.c -o bin/bubble_sort_and_merge -DSTAND_ALONE -Ih/ --std=c17 -Wall -Wextra -Wpedantic

# Module objects

all_obj: bin/fill_vector.o bin/find_zero.o bin/bubble_sort.o bin/bubble_sort_and_merge.o

bin/fill_vector.o: h/config.h h/fill_vector.h src/fill_vector.c
	gcc -c src/fill_vector.c -o bin/fill_vector.o -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/find_zero.o: h/config.h h/find_zero.h src/find_zero.c
	gcc -c src/find_zero.c -o bin/find_zero.o -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/bubble_sort.o: h/config.h h/bubble_sort.h src/bubble_sort.c
	gcc -c src/bubble_sort.c -o bin/bubble_sort.o -Ih/ --std=c17 -Wall -Wextra -Wpedantic

bin/bubble_sort_and_merge.o: h/config.h h/bubble_sort.h h/bubble_sort_and_merge.h
	gcc -c src/bubble_sort_and_merge.c -o bin/bubble_sort_and_merge.o -Ih/ --std=c17 -Wall -Wextra -Wpedantic
