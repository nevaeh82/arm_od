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
	NIIPP/Niipp.h \
	NIIPP/NiippController.h \
	NIIPP/NiippWidget.h \
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
	Tabs/Tree/TreeWidgetDelegate.h \
	Tabs/ITabManager.h \
	Tabs/ITabMap.h \
	Tabs/TabManager.h \
	Tabs/MapTabWidget.h \
	Tabs/DbBla/DbUavController.h \
	Tabs/DbBla/Defines.h  \
	Tabs/DbBla/IDbUavController.h \
	Tabs/DbBla/DbUavManager.h \
	Tabs/Tree/UavTreeModel.h \
	Tabs/MapTabWidgetController.h \
	UAV/BLAPerehvatDialog.h \
	UAV/ZInterception.h \
	MainWindow.h \
	MainWindowController.h \
	Interfaces/IDbChangedListener.h \
	Interfaces/IDbController.h \
	Interfaces/IDbManager.h \
	UiDefines.h \
	Interfaces/ISettingsChangedListener.h \
	Station.h \
	ControlPanel/ControlPanel.h \
	Interfaces/IUavDbChangedListener.h \
	Interfaces/IDbUavManager.h \
	Map/IMapClient.h \
	Map/IMapController.h \
	Map/MapClient1.h \
	Map/MapController.h \
	Map/Map.h \
	Map/MapWidget.h \
	Map/Features/AisFeature.h \
	Map/Features/EnemyBplaFeature.h \
	Map/Features/FriendBplaFeature.h \
	Map/Features/InterceptionFeature.h \
	Map/Features/NiippFeature.h \
	Map/Features/PelengatorFeature.h \
	Map/Features/StationFeature.h \

SOURCES += \
	Common/CommandMessage.cpp \
	Icons/IconsGenerator.cpp \
	NIIPP/NiippController.cpp \
	NIIPP/Niipp.cpp \
	NIIPP/NiippWidget.cpp \
	Tabs/Solver/Solver.cpp \
	Tabs/Solver/SolverWidget.cpp \
	Tabs/Solver/SolverController.cpp \
	Tabs/Tree/BLAWidgetDelegate.cpp \
	Tabs/Tree/DBController.cpp \
	Tabs/Tree/DBManager.cpp \
	Tabs/Tree/TreeWidgetDelegate.cpp \
	Tabs/TabManager.cpp \
	Tabs/MapTabWidget.cpp \
	Tabs/MapTabWidgetController.cpp \
	Tabs/DbBla/DbUavController.cpp \
	Tabs/DbBla/DbUavManager.cpp \
	Tabs/Tree/UavTreeModel.cpp \
	UAV/BLAPerehvatDialog.cpp \
	UAV/ZInterception.cpp \
	MainWindow.cpp \
	MainWindowController.cpp \
	Tabs/RPC/Rpcclient.cpp \
	ControlPanel/ControlPanel.cpp \
	Map/MapClient1.cpp \
	Map/MapController.cpp \
	Map/Map.cpp \
	Map/MapWidget.cpp \
	Map/Features/AisFeature.cpp \
	Map/Features/EnemyBplaFeature.cpp \
	Map/Features/FriendBplaFeature.cpp \
	Map/Features/InterceptionFeature.cpp \
	Map/Features/NiippFeature.cpp \
	Map/Features/PelengatorFeature.cpp \
	Map/Features/StationFeature.cpp \

FORMS += \
	UAV/blaperehvatdialog.ui \
	arm_od_view.ui \
	Tabs/Solver/SolverWidget.ui \
	Map/MapWidget.ui \
	NIIPP/NiippWidget.ui \
	MapTabWidget.ui \
	ControlPanel/ControlPanel.ui

