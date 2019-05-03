
###################################################################################
## Author: Stefan Lörwald, Universität Heidelberg                                ##
## License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode ##
###################################################################################

# if no command passed, perform a complete build (and test run)
all: $(dir_object) $(dir_binary) purge $(binary) test

# creates an archive of source code and Makefiles
archive:
	@timestamp=$$(date +%Y-%m-%d) && \
	git archive --format=tar -o $(bin_short)-$${timestamp}.tar --prefix=$(bin_short)/ HEAD && \
	tar --transform 's,^,$(bin_short)/,' -u src/git_revision.h -f $(bin_short)-$${timestamp}.tar && \
	tar --delete -f $(bin_short)-$${timestamp}.tar $(bin_short)/.gitignore $(bin_short)/revision && \
	echo "[Status] created archive $(bin_short)-$${timestamp}.tar"

# removes all files and directories this Makefile creates
clean:
	@rm -rf $(dir_object)
	@rm -rf $(dir_binary)
	@echo "[Status] removed binary, dependencies and objects"

# creates the developer documentation
doc: $(sources)
	@doxygen doxygen.conf

# perform a build only, no tests
fast: $(dir_object) $(dir_binary) purge $(binary)

# creates a library of all panda methods and classes
library: $(dir_library) $(objects)
	@ar cr $(dir_library)/lib$(library_name).a $(objects);
	@ranlib $(dir_library)/lib$(library_name).a
	@echo "[Status] created library \"$(dir_library)/lib$(library_name).a\""

# removes deprecated files (such as objects for which the source file was deleted)
purge:
	@rm -f $(deprecated_files);

# opens the most recent version of the developer documentation in the default browser
show-doc: doc
	@sensible-browser doc/html/index.html &

# executes the test suite
test: $(dir_object)/$(dir_test) $(dir_binary)/$(dir_test) $(dir_binary)/$(dir_test)/$(dir_log) $(logs_test)

