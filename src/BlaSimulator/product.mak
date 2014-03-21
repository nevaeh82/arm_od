include ../../scripts/common.mak
include ../../scripts/project.mak
include ../../specs/project.mak
include ../../specs/logger.mak

prebuild::
	-$(cp) -ud qt.conf $(DESTDIR)
	$(call begin-build, Prebuild OK )
	
postbuild::
	
