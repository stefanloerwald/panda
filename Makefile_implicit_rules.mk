
###################################################################################
## Author: Stefan Lörwald, Universität Heidelberg                                ##
## License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode ##
###################################################################################

# create directory $(dir_binary)
$(dir_binary):
	@echo "[Status] created $@";
	@mkdir -p $(dir_binary)

# create directory $(dir_binary)/$(dir_test)
$(dir_binary)/$(dir_test):
	@echo "[Status] created $@";
	@mkdir -p $(dir_binary)/$(dir_test)

# create directory $(dir_binary)/$(dir_test)/$(dir_log)
$(dir_binary)/$(dir_test)/$(dir_log):
	@echo "[Status] created $@";
	@mkdir -p $(dir_binary)/$(dir_test)/$(dir_log)

# create directory $(dir_library)
$(dir_library):
	@echo "[Status] created $@";
	@mkdir -p $(dir_library)

# create directory $(dir_object)
$(dir_object):
	@echo "[Status] created $@";
	@mkdir -p $(dir_object)

# create directory $(dir_object)/$(dir_test)
$(dir_object)/$(dir_test):
	@echo "[Status] created $@";
	@mkdir -p $(dir_object)/$(dir_test)

# link the main binary
$(binary): $(objects)
	@echo "[Status] linking $@";
	@$(COMPILER) -o $@ $^ $(flags_linkage)

# link a test binary
$(dir_binary)/$(dir_test)/%.$(ext_binary): $(dir_object)/$(dir_test)/%.o $(dir_object)/%.o $(binary)
	@echo "[Status] linking $@";
	@$(COMPILER) -o $@ $< $(objects_without_main) $(flags_linkage)

# execute a test binary. this is done via updating the corresponding log
$(dir_binary)/$(dir_test)/$(dir_log)/% : $(dir_binary)/$(dir_test)/%.$(ext_binary)
	@echo "[Status] testing $<";
	@./$< > $@
	@if [ $$(wc -c $@ | sed 's/$(dir_binary).*//') -gt 0 ]; \
	 then \
	   echo "[Status] Test $@ produced output to log"; \
	 fi

# compile an object
$(dir_object)/%.$(ext_object): $(dir_source)/%.$(ext_source) Makefile_configuration.mk Makefile_configuration_compiler.mk Makefile_configuration_mpi.mk
	@echo "[Status] compiling $@";
	@$(COMPILER) $(flags_compilation) -o $@ -c $<

# compiler generated dependencies to automatically rebuild objects if necessary
-include $(dependencies)
-include $(dependencies_test)

