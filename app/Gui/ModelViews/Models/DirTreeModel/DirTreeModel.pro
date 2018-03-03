TEMPLATE = app
TARGET = DirTreeModel
INCLUDEPATH += .

# Input
HEADERS += PBDirTreeModel.hpp PBTreeBranch.hpp
SOURCES += PBDirTreeModel.cpp PBTreeBranch.cpp

LIBS += -L/home/cosmos/Softwares/Projects/PiBreeze/PiBreezeX/common/ -lpibreeze

# Common Sources
INCLUDEPATH += . /PiBreeze/PiBreezeX/common/ /home/cosmos/Softwares/Projects/PiBreeze/PiBreezeX/common/include
DEPENDPATH += . /PiBreeze/PiBreezeX/common/ /home/cosmos/Softwares/Projects/PiBreeze/PiBreezeX/common/include

# RPATH for libnewbreeze-common
QMAKE_RPATHDIR += /usr/lib/newbreeze/

# Enable warnings and threading support
CONFIG += thread silent warn_off build_all
