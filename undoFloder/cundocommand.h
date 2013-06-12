#ifndef CUNDOCOMMAND_H
#define CUNDOCOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>
class MapSpriteItem;

//! [2]
class AddCommand : public QUndoCommand
{
public:
    AddCommand(MapSpriteItem *mapGraphicsSpriteItem, QGraphicsScene *graphicsScene,
               QUndoCommand *parent = 0);
    ~AddCommand();

    void undo();
    void redo();

private:
    MapSpriteItem *myDiagramItem;
    QGraphicsScene *myGraphicsScene;
    QPointF initialPosition;
};
//! [2]

QString createCommandString(MapSpriteItem *item, const QPointF &point);







#endif // CUNDOCOMMAND_H
