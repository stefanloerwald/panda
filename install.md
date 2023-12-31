## Installation

PANDA supports two build systems: CMake and gnumake. We highly recommend using CMake with an out-of-source build.
### Verify installation of compatible compiler:

Tested compilers include: `g++-4.7`, `g++-4.8`, `g++-4.9`, `g++-5`, `clang++-3.4`, `clang++-3.5`, `clang++-3.6`. It is definitively incompatible with version of `g++` prior to version `4.6`.
PANDA was tested on Linux 32bit and 64bit platforms as well as Mac OS X.
### Installation using CMake
We recommend performing out-of-source builds.

```
> mkdir build
> cd build
> cmake ..
-- The C compiler identification is GNU 4.9.1
-- The CXX compiler identification is GNU 4.9.1
...
-- Setting build type to 'Release' as none was specified.
-- Configuring done
-- Generating done
-- Build files have been written to: /home/user/panda/build
> make
> ctest #optional, but highly recommended
> make install #optional
```

If you do not have `ctest` installed, PANDA will probably still work fine. If you want to execute the tests without `ctest`, simply execute every binary in the build directory (exluding PANDA itself).

If you encounter errors using PANDA, please perform a run in debug mode. To build PANDA with debug information, execute

```
> cmake -DCMAKE_BUILD_TYPE=Debug ..
> make
```
### Installation using gnumake

On a standard Linux or Mac system, `./configure; make;` should suffice to install PANDA. Use `make -j 4` for a parallel build (in this case with 4 processes). If for any reason there is an error with this, see the next section for manual installation advice.
### Troubleshooting
If there is an error during configuration, try to create the files `Makefile_configuration_compiler.mk` and `Makefile_configuration_mpi.mk` yourself.
#### Manual specification of compiler
You may specify a compiler by creating a file called `Makefile_configuration_compiler.mk`
You need to define the variables `COMPILER` and `LINKER`. Example:

```
COMPILER = clang++-3.5
LINKER = $(COMPILER)
```
#### Manual specification of MPI
Depending on whether you wish to use MPI, you need to adjust the contents of `Makefile_configuration_mpi.mk` (create file if it doesn't exist).
##### Using MPI
For using PANDA on a cluster using MPI, the headers and library files of MPI have to be known. Most MPI distributions have a command such as `mpic++ --show-me:compile` to provide this information.

Example:
```
> mpic++ --show-me:compile
g++ -I/usr/lib/openmpi/include
> mpic++ --show-me:link
g++ -L/usr/lib/openmpi/lib -pthread
```

Then, the file `Makefile_configuration_mpi.mk` has to contain

```
flags_configuration += -DMPI_SUPPORT
flags_library_headers += -I/usr/lib/openmpi/include
flags_library_objects += -L/usr/lib/openmpi/lib -pthread
```
##### Without MPI
If you build without MPI, the file `Makefile_configuration_mpi.mk` may not contain any information to the build system. Either remove all content from the file, or deactivate using the comment symbol `#`.

```
#flags_configuration += -DMPI_SUPPORT
#flags_library_headers +=
#flags_library_objects +=
```
