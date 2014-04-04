SharedMakFile=../TestShared/testsuite.mak
include $(SharedMakFile)

include $(SolutionDir)/specs/project.mak
include $(SolutionDir)/specs/qxt.mak
include $(SolutionDir)/specs/sqldrivers.mak
include $(SolutionDir)/specs/logger.mak
include $(SolutionDir)/specs/pwgis.mak   
include $(SolutionDir)/specs/settings.mak

prebuild::
	-$(cp) -ud ../ARM_OD_view/qt.conf $(DESTDIR)
	$(call begin-build, Prebuild OK )
