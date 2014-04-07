include makeimage-win32-common.mak

.NOTPARALLEL:

DEPLOY = *.exe *.dll *.conf

TmpRel = $(SolutionDir)/build/tmprel
BinDir = $(SolutionDir)/build/bin/msvc

Qt = Gui Core Network Sql

all: makeimage

makeimage: create-dirs copy-qt-msvc copy-bin
	$(call mknsisimage, imagedef.xml)


create-dirs:
	$(mkdir) $(TmpRel)
	$(mkdir) $(TmpRel)/bin
	$(mkdir) $(TmpRel)/image
	
copy-qt-msvc: get-msvc
	$(cp) -u $(foreach name,$(Qt),$(SolutionDir)/build/qt-msvc/Qt$(name)4.dll) $(TmpRel)/bin
	
copy-bin:
	$(cp) -rudf $(foreach name,$(DEPLOY), $(BinDir)/$(name)) $(TmpRel)/bin
	$(rm) $(TmpRel)/bin/*d.*
	$(rm) $(TmpRel)/bin/*debug.dll
	$(cp) -rudf $(BinDir)/plugins $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/DB $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/images $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/KTR $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/Map $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/Rpc $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/Styles $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/Tabs $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/TCP $(TmpRel)/bin
	$(echo) VERSION = $(if $(PRODUCT_VERSION_REL),$(PRODUCT_VERSION_REL),$(PRODUCT_VERSION).0) > $(TmpRel)/bin/version.ini


move-components:

include ../specs/get-runtime.mak
