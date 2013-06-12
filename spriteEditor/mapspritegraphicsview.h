/********************************************************************
    created:    2011/07/28
    filename:   MapSpriteGraphicsView.h
    author:     bqlf
    purpose:    精灵的编辑区域
*********************************************************************/
#ifndef MAPSPRITEGRAPHICSVIEW_H
#define MAPSPRITEGRAPHICSVIEW_H

#include <QGraphicsView>
#include "Data/spritedata.h"
#include "spriteEditor/spritewindow.h"

class MapSpriteGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapSpriteGraphicsView(QWidget *parent = 0);
    void setSpriteData(SpriteData* spriteData);
    void setSpriteWindown(SpriteWindow* spriteWindow);
    void setOriginPoint(QPoint origin);
    QPoint getOriginPoint();
    void setChoosedItemIndex(int index);
    int getChoosedItemIndex();
    QGraphicsItem *getItemAt(int index);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:

public slots:
private:
    SpriteData* spriteData_;
    SpriteWindow* spriteWindow_;
    QPoint originPoint_;
    QGraphicsItem* choosedItem_;
    int intChoosedItemIndex;

};

#endif // MAPSPRITEGRAPHICSVIEW_H
