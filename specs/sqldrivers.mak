prebuild:: create-dirs
ifeq "$(OS)" "Windows_NT"
	-$(cp) -ud $(SolutionDir)/redist/plugins/sqldrivers/*.dll $(DESTDIR)/plugins/sqldrivers
	-$(cp) -ud $(SolutionDir)/redist/plugins/sqldrivers/libmysql.dll $(DESTDIR)
	-$(cp) -ud $(SolutionDir)/redist/plugins/sqldrivers/libmysqld.dll $(DESTDIR)
else
	-$(cp) -ud $(SolutionDir)/redist/plugins/sqldrivers/*.so $(DESTDIR)
endif

create-dirs:
	$(mkdir) $(DESTDIR)/plugins
	$(mkdir) $(DESTDIR)/plugins/sqldrivers
