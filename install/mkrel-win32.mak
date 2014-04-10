include makeimage-win32-common.mak

.NOTPARALLEL:

DEPLOY = *.exe *.dll *.conf *.ini
DEPLOY_EXCLUDE = *Test.exe *d.* *d1.* *debug.dll

TmpRel = $(SolutionDir)/build/tmprel
BinDir = $(SolutionDir)/build/bin/msvc

Qt = Gui Core Network Sql Svg Webkit

all: makeimage

makeimage: create-dirs copy-qt-msvc copy-bin
	$(call mknsisimage, imagedef.xml)


create-dirs:
	$(mkdir) $(TmpRel)
	$(mkdir) $(TmpRel)/bin
	$(mkdir) $(TmpRel)/image
	
copy-qt-msvc: get-msvc
	$(cp) -u $(foreach name,$(Qt),$(SolutionDir)/build/qt-msvc/Qt$(name)4.dll) $(SolutionDir)/build/qt-msvc/phonon4.dll $(TmpRel)/bin
	$(cp) $(SolutionDir)/build/qt-msvc/phonon4.dll $(TmpRel)/bin
	
copy-bin:
	$(cp) -rudf $(foreach name,$(DEPLOY), $(BinDir)/$(name)) $(TmpRel)/bin
	$(rm) $(foreach name,$(DEPLOY_EXCLUDE), $(TmpRel)/bin/$(name))
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
