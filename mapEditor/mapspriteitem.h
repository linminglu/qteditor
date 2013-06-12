/********************************************************************
    created:    2011/08/04
    filename:   MapSpriteItem.h
    author:     bqlf
    purpose:    画地图精灵item
*********************************************************************/
#ifndef MAPSPRITEITEM_H
#define MAPSPRITEITEM_H

//#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QList>
#include <QImage>
#include "Data/spritedata.h"
#include <QMimeData>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QKeyEvent>

class MapSpriteItem : public QGraphicsItem
{
public:
    MapSpriteItem();
    MapSpriteItem(MapSprite* sprite,QString imageName);
   //初始化自定义的数据
    void initData();
    void setPixmapItemProperty(QGraphicsPixmapItem* item,MapSpriteSubClip *subClip,QString imagePath);
    void setPixmapItemFlipType(QGraphicsPixmapItem* item,MapSpriteSubClip* subClip,QString imagePath);
    QRectF boundingRect() const;
    QRectF outlineRect() const;
    int roundness(double size) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString getMapSpriteName();
    void setLayer(int layer);
    void setTag(int tag);
    void setisChoosed(bool isChoosed);
    int layer();
    int tag();
    bool isChoosed();
};

#endif // MAPSPRITEITEM_H
