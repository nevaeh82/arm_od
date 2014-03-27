include ../../scripts/common.mak
include ../../scripts/project.mak
include ../../specs/project.mak
include ../../specs/logger.mak
include ../../specs/qxt.mak

prebuild::
	-$(cp) -ud qt.conf $(DESTDIR)
	$(call begin-build, Prebuild OK )
	
postbuild::
	
