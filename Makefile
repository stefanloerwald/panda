
###################################################################################
## Author: Stefan Lörwald, Universität Heidelberg                                ##
## License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode ##
###################################################################################

# configuration of compiler invocation
include Makefile_configuration.mk

# configuration of files to process
include Makefile_variables.mk

# commands this Makefile should react to
.PHONY: all archive clean doc fast purge show-doc test library

# rules
include Makefile_rules.mk
include Makefile_implicit_rules.mk

