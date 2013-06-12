#include "cundocommand.h"
#include "mapEditor/mapspriteitem.h"
//! [7]
AddCommand::AddCommand(MapSpriteItem *item,
                       QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    static int itemCount = 0;

    myGraphicsScene = scene;
    myDiagramItem = item;//new MapSpriteItem(item);
   // initialPosition = QPointF((itemCount * 15) % int(scene->width()),
                             // (itemCount * 15) % int(scene->height()));
    initialPosition=myDiagramItem->pos();
    scene->update();
    ++itemCount;
    setText(QObject::tr("Add %1")
        .arg(createCommandString(myDiagramItem, initialPosition)));
}
//! [7]

AddCommand::~AddCommand()
{
    if (!myDiagramItem->scene())
        delete myDiagramItem;
}

//! [8]
void AddCommand::undo()
{
    myGraphicsScene->removeItem(myDiagramItem);
    myGraphicsScene->update();
}
//! [8]

//! [9]
void AddCommand::redo()
{
    myGraphicsScene->addItem(myDiagramItem);
    myDiagramItem->setPos(initialPosition);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}
//! [9]

QString createCommandString(MapSpriteItem *item, const QPointF &pos)
{
    return QObject::tr("%1 at (%2, %3)")
            .arg(item->getMapSpriteName())
        .arg(pos.x()).arg(pos.y());
}
