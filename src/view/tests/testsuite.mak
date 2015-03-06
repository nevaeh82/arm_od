include ../../scripts/common.mak
# include $(SolutionDir)/specs/project.mak


Suites=$(HEADERS:.h=)


.PHONY:: prebuild postbuild mkdefines copyout testsuite.cpp  pretest


all::
	@$(echo) Avaiable targets: mksuite copyout postbuild clean
	@echo Suites: $(Suites)

prebuild:: testsuite.generated.cpp

testsuite.generated.cpp: $(wildcard *.h)
#	$(python)  $(SolutionDir)/tools/cxxtest/cxxtestgen.py --runner=VerbosePrinter --have-eh  --have-std --abort-on-fail --template ../TestShared/runner.tpl -o $@ $(HEADERS)
	$(python)  $(SolutionDir)/tools/cxxtest/cxxtestgen.py --have-eh  --have-std --abort-on-fail --runner=ErrorPrinter  -o $@ $(wildcard *.h)


postbuild:: pretest copyout
	@echo $(DESTDIR_TARGET)
	@echo Starting unit-tests $(patsubst /,\,$(DESTDIR_TARGET))...
ifeq "$(OS)" "Windows_NT"
	cd $(DESTDIR) && $(if $(NORUNSUITE) neq "y", $(DESTDIR_TARGET:/=\\))
else
	cd $(DESTDIR) && $(if $(NORUNSUITE) neq "y", LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(DESTDIR) $(DESTDIR_TARGET))
endif



clean::
	$(rm) testsuite.generated.cpp