INCLUDEPATH += $$SolutionDir/redist/CISCommonLib/include
LIBS        += -L$$SolutionDir/redist/CISCommonLib/lib_$$SPEC
LIBS        += $$library( CISCommonLib )

