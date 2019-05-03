
###################################################################################
## Author: Stefan Lörwald, Universität Heidelberg                                ##
## License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode ##
###################################################################################

# target
bin_short = panda
library_name = polypanda

# directories
dir_binary  = bin
dir_library = lib
dir_object  = obj
dir_source  = src
dir_test    = test
dir_log     = log

# file extensions
ext_binary     = bin
ext_object     = o
ext_dependency = d
ext_source     = cpp
ext_header     = h

# files to compile
sources = $(wildcard $(dir_source)/*.$(ext_source))
sources_test = $(wildcard $(dir_source)/$(dir_test)/*.$(ext_source))

# object files corresponding to sources
objects = $(sources:$(dir_source)%$(ext_source)=$(dir_object)%$(ext_object))
objects_without_main = $(filter-out $(dir_object)/main.$(ext_object), $(objects))
objects_test = $(sources_test:$(dir_source)%$(ext_source)=$(dir_object)%$(ext_object))

# dependency files corresponding to sources
dependencies = $(objects:%$(ext_object)=%$(ext_dependency))
dependencies_test = $(objects_test:%$(ext_object)=%$(ext_dependency))

# binary to produce
binary = $(dir_binary)/$(bin_short)
binaries_test = $(objects_test:$(dir_object)%.$(ext_object)=$(dir_binary)%.$(ext_binary))

# logfiles the test binaries print to
logs_test = $(binaries_test:$(dir_binary)/$(dir_test)/%.$(ext_binary)=$(dir_binary)/$(dir_test)/$(dir_log)/%)

# dependencies that are deprecated
deprecated_dependencies = $(filter-out \
                               $(dependencies), \
                               $(wildcard $(dir_object)/*.$(ext_dependency)))

# objects that are deprecated
deprecated_objects = $(filter-out \
                          $(objects), \
                          $(wildcard $(dir_object)/*.$(ext_object)))

# all deprecated files
deprecated_files = $(deprecated_dependencies) $(deprecated_objects)

.PRECIOUS: $(objects) $(objects_test) $(binaries_test)

