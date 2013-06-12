#-------------------------------------------------
#
# Project created by QtCreator 2011-07-16T15:02:22
#
#-------------------------------------------------

QT       += core gui widgets
QT       += xml

TARGET = GameEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mapEditor/mapwindow.cpp \
    mapEditor/maptab.cpp \
    spriteEditor/spritewindow.cpp \
    spriteEditor/cliptab.cpp \
    spriteEditor/spritetab.cpp \
    Command/ClipEditCommand/commandclipchangeimage.cpp \
    Command/ClipEditCommand/commandclipaddclip.cpp \
    Command/ClipEditCommand/commandclipdelclip.cpp \
    Command/ClipEditCommand/commandclipsetclip.cpp \
    Command/SpriteEditCommand/commandspritechangename.cpp \
    Command/ClipEditCommand/commandclipchangename.cpp \
    Command/SpriteEditCommand/commandspriteaddsubclip.cpp \
    Command/SpriteEditCommand/commandspritedelsubclip.cpp \
    Command/SpriteEditCommand/commandspritesetsubclip.cpp \
    Command/SpriteEditCommand/commandspriteupsubclip.cpp \
    Command/SpriteEditCommand/commandspritedownsubclip.cpp \
    Command/commandinvoker.cpp \
    Data/mapdata.cpp \
    Data/spritedata.cpp \
    spriteEditor/clipgraphicsview.cpp \
    Command/ClipEditCommand/commandclipaddlotclip.cpp \
    utility.cpp \
    spriteEditor/mapspritegraphicsview.cpp \
    iodata.cpp \
    Data/memento.cpp \
    Data/caretaker.cpp \
    Command/SpriteEditCommand/commandspriteaddmapsprite.cpp \
    Command/SpriteEditCommand/commandspritedelmapsprite.cpp \
    Command/SpriteEditCommand/commandspritecopymapsprite.cpp \
    Command/SpriteEditCommand/commandspritecopysubclip.cpp \
    Command/SpriteEditCommand/commandspritepastesubclip.cpp \
    mapEditor/choosemapspriteview.cpp \
    mapEditor/mapspriteitem.cpp \
    mapEditor/mapeditview.cpp \
    Data/eventdata.cpp \
    undoFloder/cundocommand.cpp

HEADERS  += mainwindow.h \
    mapEditor/mapwindow.h \
    mapEditor/maptab.h \
    spriteEditor/spritewindow.h \
    spriteEditor/cliptab.h \
    spriteEditor/spritetab.h \
    Command/command.h \
    Command/ClipEditCommand/commandclipchangename.h \
    Command/ClipEditCommand/commandclipchangeimage.h \
    Command/ClipEditCommand/commandclipaddclip.h \
    Command/ClipEditCommand/commandclipdelclip.h \
    Command/ClipEditCommand/commandclipsetclip.h \
    Command/SpriteEditCommand/commandspritechangename.h \
    Command/SpriteEditCommand/commandspriteaddsubclip.h \
    Command/SpriteEditCommand/commandspritedelsubclip.h \
    Command/SpriteEditCommand/commandspritesetsubclip.h \
    Command/SpriteEditCommand/commandspriteupsubclip.h \
    Command/SpriteEditCommand/commandspritedownsubclip.h \
    Command/commandinvoker.h \
    Data/mapdata.h \
    Data/spritedata.h \
    spriteEditor/clipgraphicsview.h \
    Command/ClipEditCommand/ClipEditCommand.h \
    Command/ClipEditCommand/commandclipaddlotclip.h \
    utility.h \
    spriteEditor/mapspritegraphicsview.h \
    Command/SpriteEditCommand/SpriteEditCommand.h \
    iodata.h \
    Data/memento.h \
    Data/caretaker.h \
    Command/SpriteEditCommand/commandspriteaddmapsprite.h \
    Command/SpriteEditCommand/commandspritedelmapsprite.h \
    Command/SpriteEditCommand/commandspritecopymapsprite.h \
    Command/SpriteEditCommand/commandspritecopysubclip.h \
    Command/SpriteEditCommand/commandspritepastesubclip.h \
    mapEditor/choosemapspriteview.h \
    mapEditor/mapspriteitem.h \
    mapEditor/mapeditview.h \
    config.h \
    Data/eventdata.h \
    undoFloder/cundocommand.h

FORMS    += mainwindow.ui \
    mapwindow.ui \
    spritewindow.ui

RESOURCES += \
    res.qrc
