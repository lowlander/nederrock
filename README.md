[![Build Status](https://travis-ci.com/lowlander/nederrock.svg?branch=master)](https://travis-ci.com/lowlander/nederrock)

# NederRock

Nederrock is a [Rockstar](https://github.com/dylanbeattie/rockstar) to C++ transpiler written in C++.
It doesn't need any external libraries or tools (like flex, bison, or yacc), and generates code that can be compiled with a C++17 compiler like GCC or MSVC.

# Building nederrock

First clone to project from github;

`git clone git://github.com/lowlander/nederrock.git`

Create a directory to build in;

`mkdir build`

Enter the directory;

`cd build`

Execute cmake;

`cmake ../nederrock/ -DBUILD_TESTING=On`

Execute make to build everything;

`make -j all`

To run the included tests execute the following make command;

`make -j test`

To compile a .rock file use the following command;

```
build/src/nederrock \
  -i nederrock/tests/fixtures/examples/fizzbuzz-idiomatic.rock \
  -o rock.out.cpp
```
That will generate a rock.out.cpp file that can than be compiled into an executable;

`g++ -std=c++17 -Wall -O3  rock.out.cpp -o rock.out`

The resulting executable can than be run;

`./rock.out`
