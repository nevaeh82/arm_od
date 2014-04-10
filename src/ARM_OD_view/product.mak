include ../../scripts/common.mak
include $(SolutionDir)/scripts/project.mak
include ../../specs/project.mak
include ../../specs/qxt.mak
include ../../specs/sqldrivers.mak
include ../../specs/logger.mak
include ../../specs/pwgis.mak 

prebuild::
	-$(cp) -ud qt.conf $(DESTDIR)
	-$(cp) -ud ../../version.ini $(DESTDIR)
	$(call begin-build, Prebuild OK )
	
postbuild::
	
