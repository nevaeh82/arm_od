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
        Tabs/RPC/RPCClient.h \
        Tabs/Solver/SolverSettingsDialogController.h \
	Tabs/Tree/TreeWidgetDelegate.h \
	Tabs/ITabManager.h \
	Tabs/ITabMap.h \
	Tabs/TabManager.h \
	UAV/BLAPerehvatDialog.h \
	UAV/ZInterception.h \
	MainWindow.h \
	MainWindowController.h \
    Interfaces/IDbChangedListener.h \
    Interfaces/IDbController.h \
    Interfaces/IDbManager.h \
    UiDefines.h \
    Interfaces/ISettingsChangedListener.h \
    Tabs/MapTabWidget.h \
    Station.h \
    Tabs/MapTabWidgetController.h \
    ControlPanel/ControlPanel.h \
    Interfaces/IUavDbChangedListener.h \
    Tabs/DbBla/DbUavController.h \
    Tabs/DbBla/Defines.h  \
    Tabs/DbBla/IDbUavController.h \
    Tabs/DbBla/DbUavManager.h \
    Interfaces/IDbUavManager.h \
    Tabs/Tree/UavTreeModel.h \
    Tabs/RPC/RpcClientWrapper.h \
	Tabs/RPC/RpcConfigClient.h \
	Map/Map.h \
	Map/MapWidget.h \
	Map/IMapClient.h \
	Map/MapClient1.h \
	Map/IMapController.h \
	Map/MapController.h \
	Map/IMapStyleManager.h \
	Map/MapStyleManager.h \
	Map/Features/AisFeature.h \
	Map/Features/EnemyBplaFeature.h \
	Map/Features/FriendBplaFeature.h \
	Map/Features/InterceptionFeature.h \
	Map/Features/NiippFeature.h \
	Map/Features/PelengatorFeature.h \
	Map/Features/StationFeature.h \
	Map/Features/FeaturesFactory.h \
	Map/Features/IFeaturesFactory.h \
	Map/Features/FeatureAbstract.h \
	Map/Features/MarkerFeature.h \
	Map/Features/NiippPointFeature.h \
	Map/Features/PelengatorPointFeature.h \
    Tabs/Solver/SolverSettingsDialog.h

SOURCES += \
	Common/CommandMessage.cpp \
	Icons/IconsGenerator.cpp \
	NIIPP/NiippController.cpp \
	NIIPP/Niipp.cpp \
        NIIPP/NiippWidget.cpp \
        Tabs/Solver/SolverSettingsDialogController.cpp \
	Tabs/Tree/TreeWidgetDelegate.cpp \
	Tabs/TabManager.cpp \
	UAV/BLAPerehvatDialog.cpp \
	UAV/ZInterception.cpp \
	MainWindow.cpp \
	MainWindowController.cpp \
    Tabs/MapTabWidget.cpp \
    Tabs/MapTabWidgetController.cpp \
    ControlPanel/ControlPanel.cpp \
    Tabs/RPC/Rpcclient.cpp \
    Tabs/DbBla/DbUavController.cpp \
    Tabs/DbBla/DbUavManager.cpp \
    Tabs/Tree/UavTreeModel.cpp \
    Tabs/RPC/RpcClientWrapper.cpp \
	Tabs/RPC/RpcConfigClient.cpp \
	Map/Map.cpp \
	Map/MapWidget.cpp \
	Map/MapClient1.cpp \
	Map/MapController.cpp \
	Map/MapStyleManager.cpp \
	Map/Features/AisFeature.cpp \
	Map/Features/EnemyBplaFeature.cpp \
	Map/Features/FriendBplaFeature.cpp \
	Map/Features/InterceptionFeature.cpp \
	Map/Features/NiippFeature.cpp \
	Map/Features/PelengatorFeature.cpp \
	Map/Features/StationFeature.cpp \
	Map/Features/FeaturesFactory.cpp \
	Map/Features/FeatureAbstract.cpp \
	Map/Features/MarkerFeature.cpp \
	Map/Features/NiippPointFeature.cpp \
	Map/Features/PelengatorPointFeature.cpp \
    Tabs/Solver/SolverSettingsDialog.cpp

FORMS += \
	UAV/blaperehvatdialog.ui \
        arm_od_view.ui \
	Map/MapWidget.ui \
	NIIPP/NiippWidget.ui \
    MapTabWidget.ui \
    ControlPanel/ControlPanel.ui \
    Tabs/Solver/SolverSettingsDialog.ui

