
TEMPLATE = subdirs
BUILD_ALL = $$(BUILD_ALL)

include( ARM_OD_UI.pri )
include( ARM_OD_Server.pri )

ARM_OD_viewCore.depends = ARM_OD_Common
ARM_OD_view.depends         = ARM_OD_viewCore ARM_OD_viewCoreTest ARM_OD_Common
ARM_OD_viewCoreTest.depends = ARM_OD_viewCore ARM_OD_Common
ARM_OD_ServerCore.depends = ARM_OD_Common
ARM_OD_Server.depends         = ARM_OD_ServerCore ARM_OD_ServerCoreTest
ARM_OD_ServerCoreTest.depends = ARM_OD_ServerCore
