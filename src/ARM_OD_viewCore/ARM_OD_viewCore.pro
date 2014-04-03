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
	Tabs/Tree/TreeView.h \
	Tabs/Tree/TreeWidgetDelegate.h \
	Tabs/ITabManager.h \
	Tabs/ITabMap.h \
	Tabs/TabManager.h \
	Tabs/TabMap.h \
	Tabs/TabsProperty.h \
	UAV/BLAPerehvatDialog.h \
	UAV/ZInterception.h \
	MainWindow.h \
	MainWindowController.h \
	Map/IMapClient.h \
	Map/IMapController.h \
	Map/MapClient1.h \
	Map/MapController.h \
	Map/Map.h \
	Map/MapWidget.h \
	Map/Features/Ais.h \
	Map/Features/Niipp.h \
	Map/Features/Pelengator.h \
	Map/Features/Interception.h \
	Map/Features/FriendBpla.h \
	Map/Features/EnemyBpla.h \
	Map/Features/Station.h \

SOURCES += \
	Common/CommandMessage.cpp \
	Common/Router.cpp \
	Icons/IconsGenerator.cpp \
	NIIPP/TCPPareserNiiPP.cpp \
	NIIPP/NiippController.cpp \
	NIIPP/Niipp.cpp \
	NIIPP/NiippParser.cpp \
	NIIPP/NiippWidget.cpp \
	Tabs/RPC/rpcclient.cpp \
	Tabs/Solver/Solver.cpp \
	Tabs/Solver/SolverWidget.cpp \
	Tabs/Solver/SolverController.cpp \
	Tabs/Tree/BLAWidgetDelegate.cpp \
	Tabs/Tree/DBController.cpp \
	Tabs/Tree/DBManager.cpp \
	Tabs/Tree/TreeItem.cpp \
	Tabs/Tree/treemodel.cpp \
	Tabs/Tree/TreeView.cpp \
	Tabs/Tree/TreeWidgetDelegate.cpp \
	Tabs/TabManager.cpp \
	Tabs/TabMap.cpp \
	Tabs/TabsProperty.cpp \
	UAV/BLAPerehvatDialog.cpp \
	UAV/ZInterception.cpp \
	MainWindow.cpp \
	MainWindowController.cpp \
	Map/MapClient1.cpp \
	Map/MapController.cpp \
	Map/Map.cpp \
	Map/MapWidget.cpp \
	Map/Features/Ais.cpp \
	Map/Features/Niipp.cpp \
	Map/Features/Pelengator.cpp \
	Map/Features/Interception.cpp \
	Map/Features/FriendBpla.cpp \
	Map/Features/EnemyBpla.cpp \
	Map/Features/Station.cpp \

FORMS += \
	UAV/blaperehvatdialog.ui \
	arm_od_view.ui \
	TabMap.ui \
	Tabs/Solver/SolverWidget.ui \
	Map/MapWidget.ui \
	NIIPP/NiippWidget.ui

