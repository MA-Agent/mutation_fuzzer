# mutation_fuzzer

Mutation based fuzzer using C language for the "Computer System Security" course at UCLouvain.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

The mutation based fuzzer only works on Linux operating systems because of the use of the following libraries
```
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
```

### Installing

You will first of all need to compile the program with the following command

```
/usr/bin/gcc -g /home/user/Desktop/fuzzer_proj/code/src/main.c -o /home/user/Desktop/fuzzer_proj/code/src/main
```

Make sure you are in the right directory before compiling


### Running

The executable file can be ran using the following command
```
./main testinput.img 100 0.02
```
testinput.img   is a input file that doesn't crash the converter program
100             is the number of test runs to make
0.02            means that the fuzzer randomly changes 1% of the bytes in the input file in order to
                create the next input file for the current test run.


### Explanation of the crash files
the following files can be found in the "crash" folder
1) 1201_14ff_15ff_crash.img
when the  12th byte is set to 01
the 14th to ff
and the 15th to ff (starting at 0)
the converter crashes
2) colortable.img
whe the 22th byte is changed (one of the colortable bytes), the converter crashes





## Authors

* **Zeyd Salef** - *Initial work* - [MA-Agentz](https://github.com/MA-Agent/)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
