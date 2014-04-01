QT += core sql network

TARGET   = ARM_OD_viewCore
TEMPLATE = lib
ProjectDir=$$PWD
CONFIG += staticlib

include(../../specs/project.pri)
include(../ARM_OD_Common/reference.pri)
include($$SolutionDir/specs/CISCommonLib.pri )
include(../../specs/qxt.pri)
include(../../specs/logger.pri)
include($$SolutionDir/specs/pwgiswidget.pri )


INCLUDEPATH += $$PWD

RESOURCES += \
        translations.qrc \
        rc.qrc

TRANSLATIONS += ARM-OD_en.ts\
                ARM-OD_ru.ts
	
#LIBS += -luser32

DEFINES += UNICODE
DEFINES += _UNICODE

OTHER_FILES +=

HEADERS += \
    Abstracts/ICommonComponents.h \
    Abstracts/IModuleController.h \
    Abstracts/ISubModule.h \
    Common/CommandMessage.h \
    Common/IMessage.h \
    Common/IRouter.h \
    Common/Router.h \
    Icons/IconsGenerator.h \
    Interfaces/core/IController.h \
    Interfaces/core/IExchangeMainTreeItem.h \
    Interfaces/core/IQueue.h \
    Interfaces/core/IRouter.h \
    Interfaces/core/ITCPController.h \
    Interfaces/IExchangeMainTreeItem.h \
    Interfaces/ITCPClient.h \
    Interfaces/ITCPParser.h \
    MapInterface/IMapClient.h \
    MapInterface/IMapController.h \
    MapInterface/mapclient1.h \
    MapInterface/MapController.h \
    NIIPP/INiippController.h \
    NIIPP/NIIPPControl.h \
    NIIPP/NIIPPParser.h \
    NIIPP/TCPPareserNiiPP.h \
    Tabs/ITabManager.h \
    Tabs/ITabMap.h \
    Tabs/TabManager.h \
    Tabs/TabMap.h \
    Tabs/TabsProperty.h \
    Tabs/RPC/IRPC.h \
    Tabs/RPC/RadiolocationInterface.h \
    Tabs/RPC/RPCClient.h \
    Tabs/Solver/Solver.h \
    Tabs/Tree/BLAWidgetDelegate.h \
    Tabs/Tree/DBController.h \
    Tabs/Tree/DBManager.h \
    Tabs/Tree/IDBCache.h \
    Tabs/Tree/IDBController.h \
    Tabs/Tree/IDBManager.h \
    Tabs/Tree/ITreeModel.h \
    Tabs/Tree/TreeItem.h \
    Tabs/Tree/TreeModel.h \
    Tabs/Tree/TreeView.h \
    Tabs/Tree/TreeWidgetDelegate.h \
    UAV/BLAPerehvatDialog.h \
    UAV/ZInterception.h \
    MainWindow.h \
    MainWindowController.h
    Tabs/DbBla/DbBlaController.h \
    Tabs/DbBla/IDbBlaController.h \
    Tabs/DbBla/Defines.h

SOURCES += \
    Common/CommandMessage.cpp \
    Common/Router.cpp \
    Icons/IconsGenerator.cpp \
    MapInterface/mapclient1.cpp \
    MapInterface/MapController.cpp \
    NIIPP/NIIPPControl.cpp \
    NIIPP/NIIPPParser.cpp \
    NIIPP/TCPPareserNiiPP.cpp \
    Tabs/TabManager.cpp \
    Tabs/TabMap.cpp \
    Tabs/TabsProperty.cpp \
    Tabs/RPC/rpcclient.cpp \
    Tabs/Solver/Solver.cpp \
    Tabs/Tree/BLAWidgetDelegate.cpp \
    Tabs/Tree/DBController.cpp \
    Tabs/Tree/DBManager.cpp \
    Tabs/Tree/TreeItem.cpp \
    Tabs/Tree/treemodel.cpp \
    Tabs/Tree/TreeView.cpp \
    Tabs/Tree/TreeWidgetDelegate.cpp \
    UAV/BLAPerehvatDialog.cpp \
    UAV/ZInterception.cpp \
    MainWindow.cpp \
    MainWindowController.cpp

    Tabs/DbBla/DbBlaController.cpp

FORMS += \
    Tabs/Solver/Solver.ui \
    UAV/blaperehvatdialog.ui \
    arm_od_view.ui

