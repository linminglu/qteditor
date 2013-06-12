#ifndef CUNDOCOMMAND_H
#define CUNDOCOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>
class MapWindow;
class MapSpriteItem;

//! [2]
class AddCommand : public QUndoCommand
{
public:
    AddCommand(MapSpriteItem *mapGraphicsSpriteItem, MapWindow *grapMapWindow,
               QUndoCommand *parent = 0);
    ~AddCommand();

    void undo();
    void redo();

private:
    MapSpriteItem *myDiagramItem;
    MapWindow *m_grapMapWindow;
    QPointF initialPosition;
};
//! [2]

QString createCommandString(MapSpriteItem *item, const QPointF &point);


//! [0]
class MoveCommand : public QUndoCommand
{
public:
    enum { Id = 1234 };

    MoveCommand(MapSpriteItem *diagramItem,MapWindow *grapMapWindow, const QPointF &oldPos,
                QUndoCommand *parent = 0);

    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *command);
    int id() const { return Id; }

private:
    MapWindow *m_grapMapWindow;
    MapSpriteItem *myDiagramItem;
    QPointF myOldPos;
    QPointF newPos;
};
//! [0]

//! [1]
class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(MapWindow *grapMapWindow, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    QList<QGraphicsItem *> myDiagramItems;
    MapWindow *m_grapMapWindow;
};





#endif // CUNDOCOMMAND_H
