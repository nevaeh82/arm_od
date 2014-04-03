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
	MapInterface/Map.h \
	MapInterface/MapWidget.h \
	NIIPP/INiippController.h \
	NIIPP/TCPPareserNiiPP.h \
	NIIPP/Niipp.h \
	NIIPP/NiippController.h \
	NIIPP/NiippWidget.h \
	NIIPP/NiippParser.h \
	Tabs/RPC/IRPC.h \
	Tabs/RPC/RadiolocationInterface.h \
	Tabs/RPC/RPCClient.h \
	Tabs/Solver/Solver.h \
	Tabs/Solver/SolverWidget.h \
	Tabs/Solver/SolverController.h \
	Tabs/Tree/BLAWidgetDelegate.h \
	Tabs/Tree/DBController.h \
	Tabs/Tree/DBManager.h \
	Tabs/Tree/IDBCache.h \
	Tabs/Tree/IDBController.h \
	Tabs/Tree/IDBManager.h \
	Tabs/Tree/ITreeModel.h \
	Tabs/Tree/TreeItem.h \
	Tabs/Tree/TreeModel.h \
	Tabs/Tree/TreeWidgetDelegate.h \
	Tabs/ITabManager.h \
	Tabs/ITabMap.h \
	Tabs/TabManager.h \
	UAV/BLAPerehvatDialog.h \
	UAV/ZInterception.h \
	MainWindow.h \
	MainWindowController.h \
#    DB/DbController.h \
#    DB/DbManager.h \
#    SettingsTree/TreeItem.h \
#    SettingsTree/TreeModel.h \
#    SettingsTree/TreeWidgetDelegate.h \
    Interfaces/IDbChangedListener.h \
    Interfaces/IDbController.h \
    Interfaces/IDbManager.h \
    UiDefines.h \
    Interfaces/ISettingsChangedListener.h \
    Tabs/MapTabWidget.h \
    Station.h \
    Tabs/MapTabWidgetController.h \
    ControlPanel/ControlPanel.h \
    Interfaces/IBlaDbChangedListener.h \
    Tabs/DbBla/DbBlaController.h \
    Tabs/DbBla/Defines.h  \
    Tabs/DbBla/IDbBlaController.h

SOURCES += \
	Common/CommandMessage.cpp \
	Common/Router.cpp \
	Icons/IconsGenerator.cpp \
	MapInterface/mapclient1.cpp \
	MapInterface/MapController.cpp \
	MapInterface/Map.cpp \
	MapInterface/MapWidget.cpp \
	NIIPP/TCPPareserNiiPP.cpp \
	NIIPP/NiippController.cpp \
	NIIPP/Niipp.cpp \
	NIIPP/NiippParser.cpp \
	NIIPP/NiippWidget.cpp \
	Tabs/Solver/Solver.cpp \
	Tabs/Solver/SolverWidget.cpp \
	Tabs/Solver/SolverController.cpp \
	Tabs/Tree/BLAWidgetDelegate.cpp \
	Tabs/Tree/DBController.cpp \
	Tabs/Tree/DBManager.cpp \
	Tabs/Tree/TreeItem.cpp \
	Tabs/Tree/treemodel.cpp \
	Tabs/Tree/TreeWidgetDelegate.cpp \
	Tabs/TabManager.cpp \
	UAV/BLAPerehvatDialog.cpp \
	UAV/ZInterception.cpp \
	MainWindow.cpp \
	MainWindowController.cpp \
#    DB/DbController.cpp \
#    DB/DbManager.cpp \
#    SettingsTree/TreeItem.cpp \
#    SettingsTree/TreeModel.cpp \
#    SettingsTree/TreeWidgetDelegate.cpp \
    Tabs/MapTabWidget.cpp \
    Tabs/MapTabWidgetController.cpp \
    ControlPanel/ControlPanel.cpp \
    Tabs/RPC/Rpcclient.cpp \
    Tabs/DbBla/DbBlaController.cpp

FORMS += \
	UAV/blaperehvatdialog.ui \
	arm_od_view.ui \
	Tabs/Solver/SolverWidget.ui \
	MapInterface/MapWidget.ui \
	NIIPP/NiippWidget.ui \
    MapTabWidget.ui \
    ControlPanel/ControlPanel.ui

