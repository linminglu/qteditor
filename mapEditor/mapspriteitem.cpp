/********************************************************************
    created:    2011/08/04
    filename:   MapSpriteItem.cpp
    author:     bqlf
    purpose:    画地图精灵item
*********************************************************************/
#include "mapspriteitem.h"
#include "utility.h"

using namespace Utility;

static const int key_MapSpriteName = 0;
static const int key_Layer = 1;
static const int key_Tag = 2;
static const int key_IsChoosed = 3;

MapSpriteItem::MapSpriteItem()
{
        setFlags(ItemIsMovable|ItemIsFocusable | ItemIsSelectable);
}

//初始化自定义的数据
void MapSpriteItem::initData()
{
    this->setData(key_MapSpriteName,"");
    this->setData(key_Layer,-1);
    this->setData(key_Tag,-1);
    this->setData(key_IsChoosed,false);
}

MapSpriteItem::MapSpriteItem(MapSprite* sprite,const QString imageName)
{
    initData();
    this->setData(key_MapSpriteName,sprite->mapSpriteName);

    for(int i = 0; i < sprite->getSubClipList().size();i++) {

        MapSpriteSubClip *subClip = sprite->getSubClipList().at(i);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(this);
        QPixmap clipPixmap;
        clipPixmap.convertFromImage(sprite->image);
        item->setPixmap(clipPixmap);
        //TODO
        QPointF anchor = QPointF(sprite->anchorOffsetPoint.x()*1.0/clipPixmap.width(),1-sprite->anchorOffsetPoint.y()*1.0/clipPixmap.height());
//        qDebug() << "anchor " << anchor;
        setAnchor(item,anchor);
        item->setFlag(QGraphicsItem::ItemIsSelectable,false);
        item->setData(key_MapSpriteName,"");
        item->setData(key_Layer,-1);
        item->setData(key_Tag,-1);
        item->setData(key_IsChoosed,false);
        item->setData(key_MapSpriteName,sprite->mapSpriteName);
    }
    setFlags(ItemIsMovable |ItemIsFocusable| ItemIsSelectable);
    setisChoosed(false);
}

void MapSpriteItem::setPixmapItemProperty(QGraphicsPixmapItem* item,MapSpriteSubClip *subClip,QString imagePath)
{
    //设置属性
    setAnchor(item,QPointF(0.5,0.5));
    item->setPos(subClip->position.x(),subClip->position.y());
    item->setRotation(subClip->rotate);
    item->setScale(subClip->scale);
    setPixmapItemFlipType(item,subClip,imagePath);

}

void MapSpriteItem::setPixmapItemFlipType(QGraphicsPixmapItem* item,MapSpriteSubClip* subClip,QString imagePath)
{
    QPixmap clipPixmap;
    QImage image(imagePath);
    QImage clipImage = image.copy(subClip->clip->clipRect);
//    QImage clipImage = item->pixmap().toImage();
    int intFlipType = subClip->intFlipType;
    //无翻转
    if(intFlipType == 0)
    {
        clipPixmap.convertFromImage(clipImage.mirrored(false,false));
    }
    //水平翻转
    else if(intFlipType == 1) {
        clipPixmap.convertFromImage(clipImage.mirrored(true,false));
    }
    //垂直翻转
    else if(intFlipType == 2) {
        clipPixmap.convertFromImage(clipImage.mirrored(false,true));
    }
    //水平+垂直
    else if(intFlipType == 3) {
        clipPixmap.convertFromImage(clipImage.mirrored(true,true));
    }
    item->setPixmap(clipPixmap);
}

void MapSpriteItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option->state & QStyle::State_Selected)
    {

        QPen pen(Qt::red);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        painter->setPen(pen);
        QRectF rect = outlineRect();
        painter->drawRoundRect(rect, roundness(rect.width()),
                               roundness(rect.height()));
    }
    if(isChoosed())
    {
        painter->setPen(Qt::blue);
        painter->drawRect(this->boundingRect().toRect());
    }
}

int MapSpriteItem::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
}
QRectF MapSpriteItem::outlineRect() const
{
   return this->childrenBoundingRect();
}
QRectF MapSpriteItem::boundingRect() const
{
    return this->childrenBoundingRect();
}

QString MapSpriteItem::getMapSpriteName()
{
//    qDebug()<<mapSpriteName;
//    return QString(mapSpriteName);
//    return "clip5";
//    qDebug()<<(this->data(key_MapSpriteName).toString());
    return (this->data(key_MapSpriteName).toString());
}

int MapSpriteItem::layer()
{
    return (this->data(key_Layer).toInt());
}

int MapSpriteItem::tag()
{
    return (this->data(key_Tag).toInt());
}

bool MapSpriteItem::isChoosed()
{
    return (this->data(key_IsChoosed).toBool());
}

void MapSpriteItem::setLayer(int layer)
{
    this->setData(key_Layer,layer);

}

void MapSpriteItem::setTag(int tag)
{
    this->setData(key_Tag,tag);

}

void MapSpriteItem::setisChoosed(bool isChoosed)
{
    this->setData(key_IsChoosed,isChoosed);

}




