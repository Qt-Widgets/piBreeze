TEMPLATE = app

TARGET = pibreeze

QT += network
QT += widgets
QT += concurrent

LIBS +=  -L../common/ -lpibreeze

# Common Sources
INCLUDEPATH += ../common/ ../common/include
DEPENDPATH += ../common/ ../common/include

# Source Paths
INCLUDEPATH += . Core Core/ArgParser Core/BookmarkInfo Core/DeleteManager Core/FSWatcher Core/IconProvider Core/Logger Core/ProcessManager Gui Gui/Dialogs
INCLUDEPATH += Gui/Dialogs/AppEditor Gui/Dialogs/BugCatcher Gui/Dialogs/ConfirmDeleteDialog Gui/Dialogs/Dialog Gui/Dialogs/FileDialog Gui/Dialogs/IOErrorsDialog
INCLUDEPATH += Gui/Dialogs/MessageDialog Gui/Dialogs/NewNodeDialog Gui/Dialogs/PropertiesDialog Gui/Dialogs/RenameDialog Gui/Dialogs/RunCmdDialog
INCLUDEPATH += Gui/Dialogs/SettingsManager Gui/Dialogs/StartupWizard Gui/ModelViews Gui/ModelViews/Models Gui/ModelViews/Models/DirTreeModel
INCLUDEPATH += Gui/ModelViews/Models/ItemViewModel Gui/ModelViews/Models/MimeTypeModel Gui/ModelViews/Models/TrashModel Gui/ModelViews/Views Gui/ModelViews/Views/IconView
INCLUDEPATH += Gui/ModelViews/Views/OpenWithView Gui/ModelViews/Views/TrashView Gui/ModelViews/Views/DirectoryView Gui/PiBreezeUI Gui/Others Gui/Others/GuiFunctions
INCLUDEPATH += Gui/Others/PluginManager Gui/Others/Settings Gui/Widgets Gui/Widgets/AddressBar Gui/Widgets/BreadCrumbsBar Gui/Widgets/Buttons Gui/Widgets/ContextMenu
INCLUDEPATH += Gui/Widgets/CustomActions Gui/Widgets/CustomPeek Gui/Widgets/FolderFlash Gui/Widgets/FolderView Gui/Widgets/GuiWidgets Gui/Widgets/InfoPanel
INCLUDEPATH += Gui/Widgets/ProcessManager Gui/Widgets/SidePanel Gui/Widgets/TrashManager

DEPENDPATH += . Core Core/ArgParser Core/BookmarkInfo Core/DeleteManager Core/FSWatcher Core/IconProvider Core/Logger Core/ProcessManager Gui Gui/Dialogs
DEPENDPATH += Gui/Dialogs/AppEditor Gui/Dialogs/BugCatcher Gui/Dialogs/ConfirmDeleteDialog Gui/Dialogs/Dialog Gui/Dialogs/FileDialog Gui/Dialogs/IOErrorsDialog
DEPENDPATH += Gui/Dialogs/MessageDialog Gui/Dialogs/NewNodeDialog Gui/Dialogs/PropertiesDialog Gui/Dialogs/RenameDialog Gui/Dialogs/RunCmdDialog
DEPENDPATH += Gui/Dialogs/SettingsManager Gui/Dialogs/StartupWizard Gui/ModelViews Gui/ModelViews/Models Gui/ModelViews/Models/DirTreeModel
DEPENDPATH += Gui/ModelViews/Models/ItemViewModel Gui/ModelViews/Models/MimeTypeModel Gui/ModelViews/Models/TrashModel Gui/ModelViews/Views Gui/ModelViews/Views/IconView
DEPENDPATH += Gui/ModelViews/Views/OpenWithView Gui/ModelViews/Views/TrashView Gui/ModelViews/Views/DirectoryView Gui/PiBreezeUI Gui/Others Gui/Others/GuiFunctions
DEPENDPATH += Gui/Others/PluginManager Gui/Others/Settings Gui/Widgets Gui/Widgets/AddressBar Gui/Widgets/BreadCrumbsBar Gui/Widgets/Buttons Gui/Widgets/ContextMenu
DEPENDPATH += Gui/Widgets/CustomActions Gui/Widgets/CustomPeek Gui/Widgets/FolderFlash Gui/Widgets/FolderView Gui/Widgets/GuiWidgets Gui/Widgets/InfoPanel
DEPENDPATH += Gui/Widgets/ProcessManager Gui/Widgets/SidePanel Gui/Widgets/TrashManager

# Headers
HEADERS += Core/ArgParser/PBCLParser.hpp
HEADERS += Core/BookmarkInfo/PBBookmarkInfo.hpp
HEADERS += Core/DeleteManager/PBDeleteManager.hpp
HEADERS += Core/FSWatcher/PBFileSystemWatcher.hpp
HEADERS += Core/IconProvider/PBIconManager.hpp
HEADERS += Core/Logger/PBLogger.hpp
HEADERS += Core/ProcessManager/PBAbstractProcess.hpp
HEADERS += Core/ProcessManager/PBDeleteProcess.hpp
HEADERS += Core/ProcessManager/PBIOProcess.hpp
HEADERS += Core/ProcessManager/PBProcessManager.hpp
HEADERS += Global.hpp
HEADERS += Gui/Dialogs/AppEditor/PBAppEditorDialog.hpp
HEADERS += Gui/Dialogs/BugCatcher/PBBugReporter.hpp
HEADERS += Gui/Dialogs/ConfirmDeleteDialog/PBConfirmDeleteDialog.hpp
HEADERS += Gui/Dialogs/Dialog/AboutPB.hpp
HEADERS += Gui/Dialogs/Dialog/PBDialog.hpp
HEADERS += Gui/Dialogs/FileDialog/PBFileDialog.hpp
HEADERS += Gui/Dialogs/IOErrorsDialog/PBErrorsDialog.hpp
HEADERS += Gui/Dialogs/MessageDialog/PBMessageDialog.hpp
HEADERS += Gui/Dialogs/NewNodeDialog/PBNewNodeDialog.hpp
HEADERS += Gui/Dialogs/PropertiesDialog/PBPropertiesDialog.hpp
HEADERS += Gui/Dialogs/RenameDialog/PBRenameDialog.hpp
HEADERS += Gui/Dialogs/RunCmdDialog/PBRunCmdDialog.hpp
HEADERS += Gui/Dialogs/SettingsManager/PBSettingsManager.hpp
HEADERS += Gui/Dialogs/SettingsManager/PBSGeneralWidget.hpp
HEADERS += Gui/Dialogs/SettingsManager/PBSIconsWidget.hpp
HEADERS += Gui/Dialogs/SettingsManager/PBSKeyBindingsWidget.hpp
HEADERS += Gui/Dialogs/SettingsManager/PBSPluginsWidget.hpp
HEADERS += Gui/Dialogs/SettingsManager/PBSSideBar.hpp
HEADERS += Gui/Dialogs/SettingsManager/PBSStyleWidget.hpp
HEADERS += Gui/Dialogs/SettingsManager/qkeysequencewidget.h
HEADERS += Gui/Dialogs/SettingsManager/qkeysequencewidget_p.h
HEADERS += Gui/Dialogs/StartupWizard/PBStartupWizard.hpp
HEADERS += Gui/ModelViews/Models/DirTreeModel/PBDirTreeModel.hpp
HEADERS += Gui/ModelViews/Models/DirTreeModel/PBTreeBranch.hpp
HEADERS += Gui/ModelViews/Models/ItemViewModel/PBFileInfoGatherer.hpp
HEADERS += Gui/ModelViews/Models/ItemViewModel/PBItemViewModel.hpp
HEADERS += Gui/ModelViews/Models/ItemViewModel/PBItemViewNode.hpp
HEADERS += Gui/ModelViews/Models/MimeTypeModel/PBMimeTypeModel.hpp
HEADERS += Gui/ModelViews/Models/MimeTypeModel/PBMimeTypeNode.hpp
HEADERS += Gui/ModelViews/Models/TrashModel/PBTrashModel.hpp
HEADERS += Gui/ModelViews/Models/TrashModel/PBTrashNode.hpp
HEADERS += Gui/ModelViews/Views/IconView/PBIconDelegate.hpp
HEADERS += Gui/ModelViews/Views/IconView/PBIconView.hpp
HEADERS += Gui/ModelViews/Views/IconView/PBIconViewRestricted.hpp
HEADERS += Gui/ModelViews/Views/OpenWithView/PBOpenWithDelegate.hpp
HEADERS += Gui/ModelViews/Views/OpenWithView/PBOpenWithView.hpp
HEADERS += Gui/ModelViews/Views/TrashView/PBTrashDelegate.hpp
HEADERS += Gui/ModelViews/Views/TrashView/PBTrashView.hpp
HEADERS += Gui/ModelViews/Views/DirectoryView/PBDirectoryView.hpp
HEADERS += Gui/PiBreezeUI/PiBreeze.hpp
HEADERS += Gui/PiBreezeUI/PiBreezeTray.hpp
HEADERS += Gui/Others/GuiFunctions/PBGuiFunctions.hpp
HEADERS += Gui/Others/PluginManager/PBPluginManager.hpp
HEADERS += Gui/Others/Settings/PBSettings.hpp
HEADERS += Gui/Widgets/AddressBar/PBAddressBar.hpp
HEADERS += Gui/Widgets/BreadCrumbsBar/FlowLayout.hpp
HEADERS += Gui/Widgets/BreadCrumbsBar/PBCrumb.hpp
HEADERS += Gui/Widgets/BreadCrumbsBar/PBCrumbsBar.hpp
HEADERS += Gui/Widgets/BreadCrumbsBar/PBExtraCrumb.hpp
HEADERS += Gui/Widgets/Buttons/PBButton.hpp
HEADERS += Gui/Widgets/Buttons/PBButtons.hpp
HEADERS += Gui/Widgets/Buttons/PBPushButton.hpp
HEADERS += Gui/Widgets/ContextMenu/PBContextMenu.hpp
HEADERS += Gui/Widgets/CustomActions/PBCustomActions.hpp
HEADERS += Gui/Widgets/CustomPeek/PBCustomPeek.hpp
HEADERS += Gui/Widgets/FolderFlash/PBFolderFlash.hpp
HEADERS += Gui/Widgets/FolderView/PBFolderView.hpp
HEADERS += Gui/Widgets/GuiWidgets/PBFilterWidget.hpp
HEADERS += Gui/Widgets/GuiWidgets/PBFlashLabel.hpp
HEADERS += Gui/Widgets/GuiWidgets/PBGuiWidgets.hpp
HEADERS += Gui/Widgets/GuiWidgets/PBLabels.hpp
HEADERS += Gui/Widgets/GuiWidgets/PBProgressBar.hpp
HEADERS += Gui/Widgets/InfoPanel/PBInfoPanel.hpp
HEADERS += Gui/Widgets/ProcessManager/PBProcessManagerUI.hpp
HEADERS += Gui/Widgets/ProcessManager/PBProcessWidget.hpp
HEADERS += Gui/Widgets/SidePanel/PBBookmarkView.hpp
HEADERS += Gui/Widgets/SidePanel/PBDeviceView.hpp
HEADERS += Gui/Widgets/SidePanel/PBSidePanel.hpp
HEADERS += Gui/Widgets/SidePanel/PBVfsView.hpp
HEADERS += Gui/Widgets/TrashManager/PBTrashManager.hpp
HEADERS += optionparser.hpp

# Sources
SOURCES += Core/ArgParser/PBCLParser.cpp
SOURCES += Core/BookmarkInfo/PBBookmarkInfo.cpp
SOURCES += Core/DeleteManager/PBDeleteManager.cpp
SOURCES += Core/FSWatcher/PBFileSystemWatcher.cpp
SOURCES += Core/IconProvider/PBIconManager.cpp
SOURCES += Core/Logger/PBLogger.cpp
SOURCES += Core/ProcessManager/PBDeleteProcess.cpp
SOURCES += Core/ProcessManager/PBIOProcess.cpp
SOURCES += Core/ProcessManager/PBProcessManager.cpp
SOURCES += Gui/Dialogs/AppEditor/PBAppEditorDialog.cpp
SOURCES += Gui/Dialogs/BugCatcher/PBBugReporter.cpp
SOURCES += Gui/Dialogs/ConfirmDeleteDialog/PBConfirmDeleteDialog.cpp
SOURCES += Gui/Dialogs/Dialog/AboutPB.cpp
SOURCES += Gui/Dialogs/Dialog/PBDialog.cpp
SOURCES += Gui/Dialogs/FileDialog/PBFileDialog.cpp
SOURCES += Gui/Dialogs/IOErrorsDialog/PBErrorsDialog.cpp
SOURCES += Gui/Dialogs/MessageDialog/PBMessageDialog.cpp
SOURCES += Gui/Dialogs/NewNodeDialog/PBNewNodeDialog.cpp
SOURCES += Gui/Dialogs/PropertiesDialog/PBPropertiesDialog.cpp
SOURCES += Gui/Dialogs/RenameDialog/PBRenameDialog.cpp
SOURCES += Gui/Dialogs/RunCmdDialog/PBRunCmdDialog.cpp
SOURCES += Gui/Dialogs/SettingsManager/PBSettingsManager.cpp
SOURCES += Gui/Dialogs/SettingsManager/PBSGeneralWidget.cpp
SOURCES += Gui/Dialogs/SettingsManager/PBSIconsWidget.cpp
SOURCES += Gui/Dialogs/SettingsManager/PBSKeyBindingsWidget.cpp
SOURCES += Gui/Dialogs/SettingsManager/PBSPluginsWidget.cpp
SOURCES += Gui/Dialogs/SettingsManager/PBSSideBar.cpp
SOURCES += Gui/Dialogs/SettingsManager/PBSStyleWidget.cpp
SOURCES += Gui/Dialogs/SettingsManager/qkeysequencewidget.cpp
SOURCES += Gui/Dialogs/StartupWizard/PBStartupWizard.cpp
SOURCES += Gui/ModelViews/Models/DirTreeModel/PBDirTreeModel.cpp
SOURCES += Gui/ModelViews/Models/DirTreeModel/PBTreeBranch.cpp
SOURCES += Gui/ModelViews/Models/ItemViewModel/PBFileInfoGatherer.cpp
SOURCES += Gui/ModelViews/Models/ItemViewModel/PBItemViewModel.cpp
SOURCES += Gui/ModelViews/Models/ItemViewModel/PBItemViewNode.cpp
SOURCES += Gui/ModelViews/Models/MimeTypeModel/PBMimeTypeModel.cpp
SOURCES += Gui/ModelViews/Models/MimeTypeModel/PBMimeTypeNode.cpp
SOURCES += Gui/ModelViews/Models/TrashModel/PBTrashModel.cpp
SOURCES += Gui/ModelViews/Models/TrashModel/PBTrashNode.cpp
SOURCES += Gui/ModelViews/Views/IconView/PBIconDelegate.cpp
SOURCES += Gui/ModelViews/Views/IconView/PBIconView.cpp
SOURCES += Gui/ModelViews/Views/IconView/PBIconViewRestricted.cpp
SOURCES += Gui/ModelViews/Views/OpenWithView/PBOpenWithDelegate.cpp
SOURCES += Gui/ModelViews/Views/OpenWithView/PBOpenWithView.cpp
SOURCES += Gui/ModelViews/Views/TrashView/PBTrashDelegate.cpp
SOURCES += Gui/ModelViews/Views/TrashView/PBTrashView.cpp
SOURCES += Gui/ModelViews/Views/DirectoryView/PBDirectoryView.cpp
SOURCES += Gui/PiBreezeUI/PiBreeze.cpp
SOURCES += Gui/PiBreezeUI/PiBreezeTray.cpp
SOURCES += Gui/Others/GuiFunctions/PBGuiFunctions.cpp
SOURCES += Gui/Others/PBStartup.cpp
SOURCES += Gui/Others/PluginManager/PBPluginManager.cpp
SOURCES += Gui/Others/Settings/PBSettings.cpp
SOURCES += Gui/Widgets/AddressBar/PBAddressBar.cpp
SOURCES += Gui/Widgets/BreadCrumbsBar/FlowLayout.cpp
SOURCES += Gui/Widgets/BreadCrumbsBar/PBCrumb.cpp
SOURCES += Gui/Widgets/BreadCrumbsBar/PBCrumbsBar.cpp
SOURCES += Gui/Widgets/BreadCrumbsBar/PBExtraCrumb.cpp
SOURCES += Gui/Widgets/Buttons/PBButton.cpp
SOURCES += Gui/Widgets/Buttons/PBButtons.cpp
SOURCES += Gui/Widgets/Buttons/PBPushButton.cpp
SOURCES += Gui/Widgets/ContextMenu/PBContextMenu.cpp
SOURCES += Gui/Widgets/CustomActions/PBCustomActions.cpp
SOURCES += Gui/Widgets/CustomPeek/PBCustomPeek.cpp
SOURCES += Gui/Widgets/FolderFlash/PBFolderFlash.cpp
SOURCES += Gui/Widgets/FolderView/PBFolderView.cpp
SOURCES += Gui/Widgets/GuiWidgets/PBFilterWidget.cpp
SOURCES += Gui/Widgets/GuiWidgets/PBFlashLabel.cpp
SOURCES += Gui/Widgets/GuiWidgets/PBGuiWidgets.cpp
SOURCES += Gui/Widgets/GuiWidgets/PBLabels.cpp
SOURCES += Gui/Widgets/GuiWidgets/PBProgressBar.cpp
SOURCES += Gui/Widgets/InfoPanel/PBInfoPanel.cpp
SOURCES += Gui/Widgets/ProcessManager/PBProcessManagerUI.cpp
SOURCES += Gui/Widgets/ProcessManager/PBProcessWidget.cpp
SOURCES += Gui/Widgets/SidePanel/PBBookmarkView.cpp
SOURCES += Gui/Widgets/SidePanel/PBDeviceView.cpp
SOURCES += Gui/Widgets/SidePanel/PBSidePanel.cpp
SOURCES += Gui/Widgets/SidePanel/PBVfsView.cpp
SOURCES += Gui/Widgets/TrashManager/PBTrashManager.cpp
SOURCES += Main.cpp

# Vault Sources
INCLUDEPATH += Vault Vault/EncFS Vault/KeyDialog Vault/Salsa20
DEPENDPATH += Vault Vault/EncFS Vault/KeyDialog Vault/Salsa20

HEADERS += Vault/PBCircle.hpp
HEADERS += Vault/PBVault.hpp
HEADERS += Vault/PBVaultDatabase.hpp
HEADERS += Vault/EncFS/PBEncFS.hpp
HEADERS += Vault/KeyDialog/PBPasswordDialog.hpp
HEADERS += Vault/KeyDialog/PBPasswordEdit.hpp
HEADERS += Vault/KeyDialog/PBPasswordInput.hpp
HEADERS += Vault/KeyDialog/PBPatternPad.hpp
HEADERS += Vault/Salsa20/PBSalsa20.hpp
HEADERS += Vault/Salsa20/Salsa20.hpp

SOURCES += Vault/PBCircle.cpp
SOURCES += Vault/PBVault.cpp
SOURCES += Vault/PBVaultDatabase.cpp
SOURCES += Vault/KeyDialog/PBPasswordDialog.cpp
SOURCES += Vault/KeyDialog/PBPasswordEdit.cpp
SOURCES += Vault/KeyDialog/PBPasswordInput.cpp
SOURCES += Vault/KeyDialog/PBPatternPad.cpp
SOURCES += Vault/EncFS/PBEncFS.cpp
SOURCES += Vault/Salsa20/PBSalsa20.cpp

# Icon and stylesheet resources
RESOURCES += PiBreeze.qrc ../data/data.qrc

QMAKE_CFLAGS += -std=c99
QMAKE_CXXFLAGS += -std=c++11

# Enable warnings and threading support
CONFIG += thread silent warn_off build_all

# Disable Debug on Release
# CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# Build location

BUILD_PREFIX = $$(PB_BUILD_DIR)

isEmpty( BUILD_PREFIX ) {
	BUILD_PREFIX = ./build
}

MOC_DIR 	= $$BUILD_PREFIX/moc-qt5
OBJECTS_DIR = $$BUILD_PREFIX/obj-qt5
RCC_DIR		= $$BUILD_PREFIX/qrc-qt5
UI_DIR      = $$BUILD_PREFIX/uic-qt5

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}
	BINDIR = $$PREFIX/bin

	INSTALLS += target desktop icons data mime templates
	target.path = $$BINDIR

	QMAKE_RPATHDIR += $$PREFIX/lib/pibreeze/

	desktop.path = $$PREFIX/share/applications/
	desktop.files = ../data/pibreeze.desktop

	icons.path = $$PREFIX/share/icons/hicolor/256x256/apps/
	icons.files = icons/pibreeze.png

	data.path = $$PREFIX/share/pibreeze/
	data.files = README ../Changelog ../ReleaseNotes

	templates.path =  $$PREFIX/share/pibreeze/templates/
	templates.files = ../data/templates/*

	mime.path = $$PREFIX/share/mime/packages/
	mime.files = ../data/x-encrypted-file.xml
}

DEFINES += "HAVE_POSIX_OPENPT"
