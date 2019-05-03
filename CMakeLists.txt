
###################################################################################
## Author: Stefan Lörwald, Universität Heidelberg                                ##
## License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode ##
###################################################################################

cmake_minimum_required(VERSION 2.6.4)

project(panda)

# dependencies
find_package(MPI)
find_package(Threads REQUIRED)

# build type (default=Release)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# list of all source files
file(GLOB source_files src/*.cpp)
# list of all source files except the main file
list(REMOVE_ITEM source_files src/main.cpp)
# grouping the translation units
add_library(polypanda ${source_files})
# associating the main file with the executable target
add_executable(panda src/main.cpp)

# optimization properties
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

# warning flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using AppleClang
  set(compile_flags "-Weverything -pedantic -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-missing-noreturn -Wno-exit-time-destructors -Wno-unknown-pragmas -Wno-global-constructors -Wfatal-errors")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set(compile_flags "-pedantic -Wall -Wextra -Wcast-align -Weffc++ -Wfatal-errors -Wfloat-equal -Wformat=2 -Winit-self -Wmissing-declarations -Wno-unknown-pragmas -Wold-style-cast -Wpointer-arith -Wredundant-decls -Wsequence-point -Wshadow -Wundef")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  set(compile_flags "/W4")
endif()

# setting C++ standard
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(--std=c++11 SUPPORTS_STD_CXX11)
    check_cxx_compiler_flag(--std=c++0x SUPPORTS_STD_CXX01)
    if(SUPPORTS_STD_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c++11")
    elseif(SUPPORTS_STD_CXX01)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c++0x")
    else()
        message(ERROR "Compiler does not support --std=c++11 or --std=c++0x.")
    endif()
endif()

# collecting flags for compilation and linkage
if(MPI_FOUND)
   include_directories(${MPI_CXX_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/src)
   target_link_libraries(polypanda ${MPI_CXX_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
   set(compile_flags "${compile_flags} ${MPI_CXX_COMPILE_FLAGS} -DMPI_SUPPORT")
else()
   include_directories(${CMAKE_SOURCE_DIR}/src)
   target_link_libraries(polypanda ${CMAKE_THREAD_LIBS_INIT})
endif()

# setting flags for compilation and linkage
target_link_libraries(panda ${CMAKE_THREAD_LIBS_INIT} polypanda)
set_target_properties(panda polypanda PROPERTIES COMPILE_FLAGS "${compile_flags}")
if(MPI_LINK_FLAGS)
  set_target_properties(panda polypanda PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
endif()

install(TARGETS panda RUNTIME DESTINATION bin)

# test specific rules

include(CTest)
enable_testing()

file(GLOB test_files src/test/*.cpp)
foreach(test ${test_files})
   string(REGEX REPLACE "(.*/)?(.*)\\.cpp" "\\2" test_name ${test})
   add_executable(${test_name} ${test})
   add_test(${test_name} ${test_name})
   target_link_libraries(${test_name} ${CMAKE_THREAD_LIBS_INIT} polypanda)
   set_target_properties(${test_name} PROPERTIES INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src" COMPILE_FLAGS "${compile_flags}")
endforeach()

