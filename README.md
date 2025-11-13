# POSIX Threads Handout

*An handout about threading in POSIX environments*

## Welcome

This is an handout about threading in POSIX environments drafted by [Giulio Salvi](https://github.com/GiulioSalvi): it proposes two collections of exercises which were carried out during the academic [course Operating Systems [CT0670]](https://www.unive.it/data/course/521655) in the bachelor's program in [Computer Science [CT3]](https://www.unive.it/web/en/12090/home) at the [University of Venice](https://unive.it) Ca' Foscari.

## How to Use

As mentioned above, this handout proposes two collections of exercises:

* algorithms on vectors.
* primality check and divisibility algorithms.

This handout aims to propose multiprogramming problems to students, who should attempt to solve independently.

If they got stuck while developing a solution, the contents of this handout should guide them through the one proposed for the problem.

The documentation in the headers and the comments in the code may resolve any doubt regarding the adopted approach to the problem.

## Some Terminology

In this handout's contents, some terms had been used with a specific meaning. Here is a list:

* _Module_: it is a collection of routines placed in files where a problem's solution is implemented.
* _CLI_ (_Command Line Interface_): it is the set of options and flags that are passed to a program when it is invoked.
* _Entry point_: it is a _module_'s routine which executes the _module_'s solution on a specific input. It may validate the _module_'s result if requested by the user.
* _Demo Host_: it is the program's entry point routine: it parses the CLI's options, or asks them on the standard input if the CLI's ones are not valid. Then it calls the _module_'s _entry point_. _Nitpick_: even if the *Demo Host*s are usually placed into a _module_'s file, they are not considered to be part of the _module_ since they don't implement a solution but rather an interactive interface to a solution.
* _Stand alone module_: it is a binary file which contains both a single _module_ and a _demo host_ for this _module_.
* _Main thread_: it is the only thread to which we are explicitly referring to as a thread in the print-messages, comments and documentation. It is the thread which enters the _module_'s _entry point_.
* _Worker_: a _worker_ is a thread which is carrying out some computations on behalf of the _main thread_.

This list partially describes, therefore not exhaustively, the architecture of the proposed solutions. If you are interested in a more detailed explanation of the architecture, please refer to the respective files in each collection.

## How to Compile and Run

The handout's solutions are designed to be cross-platform on Windows, macOS and Linux. Note that for Windows systems you might use some [toolchain](https://en.wikipedia.org/wiki/Toolchain) that serves you a Unix-like environment, such as *[Windows Subsystem for Linux 2](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux)* (*WSL2*) (which is the most recommended and versatile), *MSYS2*, *Cygwin*, *MinGW* and many others.

The main tool to compile and run the project is the [GNU Make](https://en.wikipedia.org/wiki/Make_(software)) file that is placed in the root folder of each collection: when cloning this repository for the first time it is **mandatory** to run the target `setup` that will set up your collection's workspace for you. Then you might want to run the target `all` for compiling all the *module*s of the collection.

The *module*s compilation produces two outputs:
* a binary object file for each _module_, which has to be linked to a _demo host_'s binary file in order to be runnable.
* a runnable binary file for each _module_ which is said to be _stand alone_.

In addition the target `all` compiles a _demo host_ which can invoke every _module_ in the collection.

## Bugs or Security Issues?!

If you find out any bug or security issue in the code, please report it in the _Issues_ section. Add any relevant label to the issue in order to organize them independently.

## Need help?

If you need any help about the code and the documentation, please feel free to join [the thread](https://github.com/GiulioSalvi/pthreads_handout/discussions/categories/q-a) in the Q&A category in the _Discussions_ section.

## License

The whole content of this handout (source code and documentation) is licensed under the _GNU General Public License 3_ as published by the _Free Software Foundation_.

You can find the original and complete copy of the license in the *[COPYING](COPYING)* file or [at the original website](https://www.gnu.org/licenses/).
