prebuild::
	$(mkdir) $(DESTDIR)/DB
	$(mkdir) $(DESTDIR)/images
	$(mkdir) $(DESTDIR)/Map
	$(mkdir) $(DESTDIR)/RPC
	$(mkdir) $(DESTDIR)/Styles
	$(mkdir) $(DESTDIR)/Tabs
	$(mkdir) $(DESTDIR)/TCP
	$(mkdir) $(DESTDIR)/Enviroment
	$(mkdir) $(DESTDIR)/RDP
	
	$(cp) -ud $(SolutionDir)/settings/*.ini $(DESTDIR)
	$(cp) -ud $(SolutionDir)/settings/DB/*.* $(DESTDIR)/DB
	$(cp) -ud $(SolutionDir)/settings/images/*.* $(DESTDIR)/images
	$(cp) -ud $(SolutionDir)/settings/Map/*.* $(DESTDIR)/Map
	$(cp) -ud $(SolutionDir)/settings/RPC/*.* $(DESTDIR)/RPC
	$(cp) -ud $(SolutionDir)/settings/Styles/*.* $(DESTDIR)/Styles
	$(cp) -ud $(SolutionDir)/settings/Tabs/*.* $(DESTDIR)/Tabs
	$(cp) -ud $(SolutionDir)/settings/TCP/*.* $(DESTDIR)/TCP
	$(cp) -ud $(SolutionDir)/settings/Enviroment/*.* $(DESTDIR)/Enviroment
	$(cp) -ud $(SolutionDir)/settings/RDP/*.* $(DESTDIR)/RDP
		
	$(call begin-build, Prebuild OK )
