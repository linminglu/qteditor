#include "cundocommand.h"
#include "mapEditor/mapspriteitem.h"
#include "mapEditor/mapwindow.h"
#include "mapEditor/maptab.h"
//! [7]
AddCommand::AddCommand(MapSpriteItem *item,
                        MapWindow *grapMapWindow, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    static int itemCount = 0;
    m_grapMapWindow = grapMapWindow;
    myDiagramItem = item;
    initialPosition=myDiagramItem->pos();
    m_grapMapWindow->getCurMap()->mapEditView->scene()->update();
    ++itemCount;
    setText(QObject::tr("Add %1")
        .arg(createCommandString(myDiagramItem, initialPosition)));
}

AddCommand::~AddCommand()
{
    if (!myDiagramItem->scene())
    {
        delete myDiagramItem;
    }
}

void AddCommand::undo()
{
   m_grapMapWindow->getCurMap()->mapEditView->scene()->removeItem(myDiagramItem);
    m_grapMapWindow->getCurMap()->mapEditView->scene()->update();
}

void AddCommand::redo()
{
    m_grapMapWindow->getCurMap()->mapEditView->scene()->addItem(myDiagramItem);
    myDiagramItem->setPos(initialPosition);
    m_grapMapWindow->getCurMap()->mapEditView->scene()->clearSelection();
    m_grapMapWindow->getCurMap()->mapEditView->scene()->update();
}

QString createCommandString(MapSpriteItem *item, const QPointF &pos)
{
    return QObject::tr("%1 at (%2, %3)")
            .arg(item->getMapSpriteName())
        .arg(pos.x()).arg(pos.y());
}

MoveCommand::MoveCommand(MapSpriteItem *diagramItem,MapWindow *grapMapWindow, const QPointF &oldPos,
                 QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_grapMapWindow=grapMapWindow;
    myDiagramItem = diagramItem;
    newPos = diagramItem->pos();
    myOldPos = oldPos;
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    MapSpriteItem *item = moveCommand->myDiagramItem;

    if (myDiagramItem != item)
    return false;

    newPos = item->pos();
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));

    return true;
}

void MoveCommand::undo()
{
    myDiagramItem->setPos(myOldPos);
    myDiagramItem->scene()->update();
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));
}

void MoveCommand::redo()
{
    myDiagramItem->setPos(newPos);
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));
}

DeleteCommand::DeleteCommand(MapWindow *grapMapWindow, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_grapMapWindow = grapMapWindow;
    myDiagramItems=m_grapMapWindow->getCurMap()->mapEditView->scene()->selectedItems();
    setText(QObject::tr("Delete MORE %1"));//.arg(createCommandString("myDiagramItem",QPointF(0,0))));
}

void DeleteCommand::undo()
{
    for(int i=0;i<myDiagramItems.count();i++)
    {
        m_grapMapWindow->getCurMap()->mapEditView->scene()->addItem(myDiagramItems.at(i));
    }
    m_grapMapWindow->getCurMap()->mapEditView->scene()->update();
}

void DeleteCommand::redo()
{
    for(int i=0;i<myDiagramItems.count();i++)
    {
        m_grapMapWindow->getCurMap()->mapEditView->scene()->removeItem(myDiagramItems.at(i));
    }

}
