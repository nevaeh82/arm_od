INCLUDEPATH += $$SolutionDir/redist/protobuf/src
#LIBS += $$SolutionDir/redist/protobuf/lib/libprotobuf.lib
LIBS        += -L$$SolutionDir/redist/protobuf/lib_$$SPEC
LIBS        += $$library( libprotobuf )
