/********************************************************************
    created:    2011/08/04
    filename:   ChooseMapSpriteView.cpp
    author:     bqlf
    purpose:    显示地图精灵view
*********************************************************************/
#include "choosemapspriteview.h"
#include "mapspriteitem.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "config.h"
#include "utility.h"
#include "iodata.h"
//#include "math.h"
using namespace Utility;

ChooseMapSpriteView::ChooseMapSpriteView(QWidget *parent) :
    QGraphicsView(parent)
{
}

ChooseMapSpriteView::ChooseMapSpriteView(SpriteData* spriteData)
{
    spriteData_ = spriteData;
    choosedMapSpriteItem = NULL;

    QGraphicsScene *mapSpriteScene_ = new QGraphicsScene;
    mapSpriteScene_->setSceneRect(0, 0, 600, 3200);

    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setScene(mapSpriteScene_);
    setTransformationAnchor(QGraphicsView::NoAnchor);

    //添加item
    //读取位置
    IOData* io = IOData::getInstance();
    io->readSpriteConfigFile(spriteData_);
    int x = 10;
    int y = 10;
    int intMaxH = 0;
    QPoint convertPoint;
    for(int i = 0; i <spriteData_->getMapSpriteList().size(); i++) {
        MapSprite* mapSprite = spriteData_->getMapSpriteList().at(i);
        MapSpriteItem *item = new MapSpriteItem(mapSprite,spriteData_->imagePath_);
        item->setScale(0.5);
        //设置位置
        if(item->childItems().size() <= 0)
            continue;
        QGraphicsPixmapItem* pixItem = (QGraphicsPixmapItem*)item->childItems().at(0);
        QPointF offset = pixItem->offset();

        if(mapSprite->choosedSpritePos == QPoint(0,0)) {
            convertPoint = QPoint(x - offset.x(),y - offset.y());
            mapSprite->choosedSpritePos = convertPoint;
        }
        else {
            convertPoint = mapSprite->choosedSpritePos;
        }
        item->setPos(convertPoint);

        x += item->boundingRect().width()+10;
        if(item->boundingRect().height() > intMaxH)
            intMaxH = item->boundingRect().height();
        if(x > 400) {
            x = 10;
            y += intMaxH + 10;
            intMaxH = 0;
        }
        mapSpriteScene_->addItem(item);
        nameList.append(mapSprite->mapSpriteName);
    }

//    for(int i = 0; i < mapSpriteScene_->items().size(); i++) {
//        qDebug()<<("name : "+ convertToStringFromInt(i) + mapSpriteScene_->items().at(i)->data(0).toString());
//    }
    this->setBackgroundRole(QPalette::Dark);
}

void ChooseMapSpriteView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    QPointF convertPoint = mapToScene(event->pos());

//    QList<QGraphicsItem*> itemList = scene()->items();
//    QRectF itemRect;
//    qDebug("size + %d",itemList.size());
//    for(int i = 0; i < itemList.size(); i++) {
//        QGraphicsItem* item = itemList.at(i);
////        itemRect = item->mapToScene(itemList.at(i)->boundingRect());
//        QPolygonF converPolygon = item->mapToScene(itemList.at(i)->boundingRect());
//        if(isPointInRect(convertPoint.toPoint(),converPolygon.boundingRect().toRect())) {
//            choosedMapSpriteItem = (MapSpriteItem *)item;
//            intChoosedMapSpriteIndex = getCurMapSpriteIndex();
//            qDebug("index = %d",intChoosedMapSpriteIndex);
//            break;
//        }
//    }

//    MapSpriteItem *item = (MapSpriteItem *)this->scene()->itemAt(convertPoint);
    MapSpriteItem *item = mapWindow_->mapSpriteItemAtPoint(this->scene(),convertPoint.toPoint());
    if(item) {
        choosedMapSpriteItem = item;
        intChoosedMapSpriteIndex = getCurMapSpriteIndex();
        mapWindow_->setCurMapSpriteItem(item->getMapSpriteName());
    }
    scene()->update();
//    item->setSelected(true);
//    item->update();
}

void ChooseMapSpriteView::mouseMoveEvent(QMouseEvent *event)
{
    if(isMoveChooseSprite) {
        QGraphicsView::mouseMoveEvent(event);
        QPointF scenePos = mapToScene(event->pos());
        MapSpriteItem *item = mapWindow_->mapSpriteItemAtPoint(this->scene(),scenePos.toPoint());
        if(item) {
            if(item->x() < 0) {
                item->setPos(0,item->y());
            }
            if(item->y() < 0) {
                item->setPos(item->x(),0);
            }
            //保存位置
            for(int i = 0; i < spriteData_->getMapSpriteList().size(); i++) {
                MapSprite* mapSprite = spriteData_->getMapSpriteList().at(i);
                if(mapSprite->mapSpriteName == item->getMapSpriteName()) {
                    mapSprite->choosedSpritePos = item->pos().toPoint();
                    IOData* io = IOData::getInstance();
                    io->saveSpriteConfigFile(spriteData_);
                }
            }
        }



    }
}

void ChooseMapSpriteView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(this->viewport());
    if(choosedMapSpriteItem) {
        QRectF rectF = choosedMapSpriteItem->mapRectToScene(choosedMapSpriteItem->boundingRect());
        QPolygon converPolygon = mapFromScene(rectF);
        painter.setPen(Qt::red);
        painter.drawRect(converPolygon.boundingRect());
        if(intChoosedMapSpriteIndex != -1) {
//            painter.drawText(choosedMapSpriteItem->x(),choosedMapSpriteItem->y(),nameList.at(intChoosedMapSpriteIndex));
        }
    }
}

int ChooseMapSpriteView::getCurMapSpriteIndex()
{
    QList<QGraphicsItem*> itemList = scene()->items();

    for(int i = 0; i < itemList.size(); i++) {

        if(choosedMapSpriteItem == itemList.at(i)) {
            return itemList.size() - 1 - i;
        }
    }
    return -1;
}
