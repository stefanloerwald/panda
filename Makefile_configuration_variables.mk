
###################################################################################
## Author: Stefan Lörwald, Universität Heidelberg                                ##
## License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode ##
###################################################################################

# definition of variables

-include Makefile_configuration_mpi.mk

flags_cpp_standard    += -std=c++11
flags_compatibility   += -D_GLIBCXX_USE_NANOSLEEP
flags_configuration   += -pthread
flags_dependencies    += -MD -MP
flags_library_headers += -I./src/
flags_library_objects +=
flags_warnings        += -pedantic -Wall -Wextra -Wcast-align -Weffc++       \
                         -Wfatal-errors -Wfloat-equal -Wformat=2 -Winit-self \
                         -Wmissing-declarations -Wno-unknown-pragmas         \
                         -Wold-style-cast -Wpointer-arith -Wredundant-decls  \
                         -Wsequence-point -Wshadow -Wundef

flags_debugging       += -DDEBUG -g
flags_profiling       += -g -p -pg
flags_optimization    += -O2 -march=native -DNDEBUG

# aggregation

flags_compilation = $(flags_cpp_standard) \
                    $(flags_compatibility) \
                    $(flags_configuration) \
                    $(flags_dependencies) \
                    $(flags_library_headers) \
                    $(flags_warnings) \
                    $(additional_flags)

ifeq ($(enable_debugging),true)
   flags_compilation += $(flags_debugging)
else
   flags_compilation += -DNDEBUG
   flags_compilation += $(flags_optimization)
endif
ifeq ($(enable_profiling),true)
   flags_compilation += $(flags_profiling)
endif

flags_linkage = $(flags_library_objects) \
                $(flags_compilation)

