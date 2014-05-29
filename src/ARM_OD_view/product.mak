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
	$(mkdir) $(DESTDIR)/plugins/imageformats
ifeq "$(OS)" "Windows_NT"
	-$(cp) $(PRODUCT_PLATFORM)/qt/$(PLATFORM)/plugins/imageformats/qjpeg*.dll $(DESTDIR)/plugins/imageformats
else
	-$(cp) $(LINUX_QT_DIR)/../plugins/imageformats/qjpeg*.so $(DESTDIR)/plugins/imageformats
endif
	$(cp) -ud *.bat $(DESTDIR)
	$(mkdir) $(SolutionDir)/build/bin/$(MAKE_PLATFORM)/icons
	$(cp) -ud images/shortcut.ico $(DESTDIR)/icons
