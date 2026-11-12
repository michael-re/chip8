#===============================================================================
#
# @file         makefile
# @author       michael-re
# @date         2026-11-08
# @copyright    copyright (c) 2026 michael-re, all rights reserved.
#
#===============================================================================

#===============================================================================
# brief:
#   Configures and builds the project using the given cmake preset.
#
# usage:
#   $(call build,<preset>,<config>)
#
# arguments:
#   <preset> : Name of the cmake configure preset.
#   <config> : Build configuration (Debug or Release).
#===============================================================================
define build
	cmake -S . --preset $(1) && cmake --build build/$(1) --config $(2)
	if [ -f build/$(1)/compile_commands.json ]; then           \
		cmake -E copy build/$(1)/compile_commands.json build/; \
	fi
endef

#===============================================================================
# recipes
#===============================================================================

.PHONY: help
help:
	@echo " "
	@echo " |==================================================================="
	@echo " ||"
	@echo " || CHIP-8 Build System Usage"
	@echo " ||"
	@echo " |+=================================================================="
	@echo " ||"
	@echo " || Supported make recipes:"
	@echo " ||"
	@echo " ||     help       - Show this help message"
	@echo " ||     clean      - Remove build artifacts"
	@echo " ||     debug      - Build debug target"
	@echo " ||     release    - Build release target"
	@echo " ||"
	@echo " |==================================================================="
	@echo " "
	@exit

.PHONY: clean
clean:
	cmake -E remove_directory build/
	cmake -E remove_directory target/

.PHONY: debug
debug:
	$(call build,debug,Debug)

.PHONY: release
release:
	$(call build,release,Release)
