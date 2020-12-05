# Skip list

* Work in progress
* Implementation of [Skip list](https://en.wikipedia.org/wiki/Skip_list) data structure

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: `make` is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 8.0
  * Linux: `gcc` / `g++` is installed by default on most Linux distros
  * Mac: same deal as `make` - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Cloning 

When cloning this project, be sure to use the --recurse-submodules flag. 

`git clone https://github.com/SandroWissmann/Skip-list.git --recurse-submodules`

This makes sure you also clone the submodule for `googletest` which is needed to run the tests

### Using the Skip list

* Just copy `skip_list.h`. No compilation required


### Running the tests

1. Go to folder `Skip-list`
2. Run `make build`
3. `cd build`
4. `./test`

### Additional Commands from Makefile

* `make debug` -> builds with debug information
* `make format` -> runs [clangFormat](https://clang.llvm.org/docs/ClangFormat.html) on project
* `make clean` -> deletes build folder

for more information see Makefile in Skip-list folder

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details