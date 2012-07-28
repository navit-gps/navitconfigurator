# Used libraries:
# - quazip : http://quazip.sourceforge.net/
# - zlib   : http://zlib.net/
# Copied:
# - quazip : quazip-0.4.3.zip/quazip-0.4.3/quazip/*.h *.c *.cpp to src/quazip/.
# - zlib   : zlib125.zip/zlib-1.2.5/*.h *.c                     to src/zlib/.
# Deleted:
# - src/quazip/JlCompress.h
# - src/quazip/JlCompress.cpp
# - src/zlib/example.c
# - src/zlib/minigzip.c
win32 {
	INCLUDEPATH += D:\\Sprache\\Qt\\4.7.4\\include
}
CONFIG += debug_and_release
TEMPLATE = app
QT += core \
    gui \
    xml \
    network \
    webkit
INCLUDEPATH += src \
    src/zlib
HEADERS += src/navitconf/gui/mapview/BoxItemAttribute.h \
    src/navitconf/gui/mapview/TableComboBox.h \
    src/navitconf/gui/mapview/BoxItemDelegate.h \
    src/navitconf/gui/mapview/BoxGraphicsItemListener.h \
    src/navitconf/gui/mapview/BoxGraphicsItem.h \
    src/navitconf/gui/mapview/DialogMapView.h \
    src/navitconf/gui/mapview/MapViewScene.h \
    src/navitconf/gui/data/ComboBoxData.h \
    src/navitconf/gui/data/CheckBoxData.h \
    src/navitconf/gui/DialogGeneralSettings.h \
    src/navitconf/data/commands/TreeItemSetNodeValueCommand.h \
    src/navitconf/data/NavitMenuModel.h \
    src/navitconf/data/NavitMenuItem.h \
    src/navitconf/gui/DialogManageNavitMenu.h \
    src/navitconf/android/NavitRunner.h \
    src/navitconf/android/Android.h \
    src/navitconf/gui/DialogStartingPosition.h \
    src/navitconf/data/commands/TreeItemSetAttributeCommand.h \
    src/navitconf/data/commands/TreeItemEditCommand.h \
    src/navitconf/data/commands/TreeItemInsertCommand.h \
    src/navitconf/data/commands/TreeItemRemoveCommand.h \
    src/navitconf/data/commands/AbstractTreeNodeCommand.h \
    src/navitconf/gui/TreeView.h \
    src/navitconf/data/TreeModel.h \
    src/navitconf/data/TreeItem.h \
    src/navitconf/file/Document.h \
    src/navitconf/file/Settings.h \
    src/navitconf/file/Unzip.h \
    src/navitconf/gui/DialogDownload.h \
    src/navitconf/gui/NavitConfigurator.h \
    src/navitconf/gui/TabNode.h \
    src/navitconf/net/Download.h \
    src/quazip/crypt.h \
    src/quazip/ioapi.h \
    src/quazip/quaadler32.h \
    src/quazip/quachecksum32.h \
    src/quazip/quacrc32.h \
    src/quazip/quazip.h \
    src/quazip/quazip_global.h \
    src/quazip/quazipfile.h \
    src/quazip/quazipfileinfo.h \
    src/quazip/quazipnewinfo.h \
    src/quazip/unzip.h \
    src/quazip/zip.h \
    src/zlib/crc32.h \
    src/zlib/deflate.h \
    src/zlib/gzguts.h \
    src/zlib/inffast.h \
    src/zlib/inffixed.h \
    src/zlib/inflate.h \
    src/zlib/inftrees.h \
    src/zlib/trees.h \
    src/zlib/zconf.h \
    src/zlib/zlib.h \
    src/zlib/zutil.h
SOURCES += src/navitconf/gui/mapview/BoxItemAttribute.cpp \
    src/navitconf/gui/mapview/TableComboBox.cpp \
    src/navitconf/gui/mapview/BoxItemDelegate.cpp \
    src/navitconf/gui/mapview/BoxGraphicsItem.cpp \
    src/navitconf/gui/mapview/DialogMapView.cpp \
    src/navitconf/gui/mapview/MapViewScene.cpp \
    src/navitconf/gui/data/ComboBoxData.cpp \
    src/navitconf/gui/data/CheckBoxData.cpp \
    src/navitconf/gui/DialogGeneralSettings.cpp \
    src/navitconf/data/commands/TreeItemSetNodeValueCommand.cpp \
    src/navitconf/data/NavitMenuModel.cpp \
    src/navitconf/data/NavitMenuItem.cpp \
    src/navitconf/gui/DialogManageNavitMenu.cpp \
    src/navitconf/android/NavitRunner.cpp \
    src/navitconf/android/Android.cpp \
    src/navitconf/gui/DialogStartingPosition.cpp \
    src/navitconf/data/commands/TreeItemSetAttributeCommand.cpp \
    src/navitconf/data/commands/TreeItemEditCommand.cpp \
    src/navitconf/data/commands/TreeItemInsertCommand.cpp \
    src/navitconf/data/commands/TreeItemRemoveCommand.cpp \
    src/navitconf/data/commands/AbstractTreeNodeCommand.cpp \
    src/navitconf/gui/TreeView.cpp \
    src/navitconf/data/TreeModel.cpp \
    src/navitconf/data/TreeItem.cpp \
    src/main.cpp \
    src/navitconf/file/Document.cpp \
    src/navitconf/file/Settings.cpp \
    src/navitconf/file/Unzip.cpp \
    src/navitconf/gui/DialogDownload.cpp \
    src/navitconf/gui/NavitConfigurator.cpp \
    src/navitconf/gui/TabNode.cpp \
    src/navitconf/net/Download.cpp \
    src/quazip/qioapi.cpp \
    src/quazip/quaadler32.cpp \
    src/quazip/quacrc32.cpp \
    src/quazip/quazip.cpp \
    src/quazip/quazipfile.cpp \
    src/quazip/quazipnewinfo.cpp \
    src/quazip/unzip.c \
    src/quazip/zip.c \
    src/zlib/adler32.c \
    src/zlib/compress.c \
    src/zlib/crc32.c \
    src/zlib/deflate.c \
    src/zlib/gzclose.c \
    src/zlib/gzlib.c \
    src/zlib/gzread.c \
    src/zlib/gzwrite.c \
    src/zlib/infback.c \
    src/zlib/inffast.c \
    src/zlib/inflate.c \
    src/zlib/inftrees.c \
    src/zlib/trees.c \
    src/zlib/uncompr.c \
    src/zlib/zutil.c
FORMS += src/navitconf/gui/DialogMapView.ui \
    src/navitconf/gui/DialogGeneralSettings.ui \
    src/navitconf/gui/DialogManageNavitMenu.ui \
    src/navitconf/gui/DialogStartingPosition.ui \
    src/navitconf/gui/DialogDownload.ui \
    src/navitconf/gui/NavitConfigurator.ui

# store icons in the program file defined in the following qrc file
RESOURCES += NavitConfigurator.qrc

# translations for other language than english (only german yet)
TRANSLATIONS += NavitConfigurator_de.ts

# define the target for the binary file
win32 {
    # icon for exe file on windows
    RC_FILE += NavitConfigurator.rc
    TARGET = NavitConfigurator
}
unix {
    # binary file
    TARGET = navitconfigurator
    target.path = /usr/bin
    INSTALLS += target

    # icon for menu link
    icon.path = /usr/share/pixmaps
    icon.files = navitconfigurator.png
    INSTALLS += icon

    # menu link
    menu_link.path = /usr/share/applications
    menu_link.files = navitconfigurator.desktop
    INSTALLS += menu_link

    # program translation file to german
    programtranslation.path = /usr/share/navitconfigurator
    programtranslation.files = navitconfigurator_de.qm
    INSTALLS += programtranslation
    
    # /usr/share/qt4/translation/qt_de.qm installed with qt4
}
