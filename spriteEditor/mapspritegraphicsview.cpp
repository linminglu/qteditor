/********************************************************************
    created:    2011/07/28
    filename:   MapSpriteGraphicsView.cpp
    author:     bqlf
    purpose:    精灵的编辑区域
*********************************************************************/
#include "mapspritegraphicsview.h"
#include <QGraphicsItem>
#include "Utility.h"
#include "config.h"

MapSpriteGraphicsView::MapSpriteGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
//    this->setMouseTracking(true);
    choosedItem_ = NULL;
    intChoosedItemIndex = -1;
}

void MapSpriteGraphicsView::setSpriteData(SpriteData *spriteData)
{
    spriteData_ = spriteData;
}

void MapSpriteGraphicsView::setSpriteWindown(SpriteWindow *spriteWindow)
{
    spriteWindow_ = spriteWindow;
}

void MapSpriteGraphicsView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(this->viewport());

    QPolygon converPolygon;
    painter.setPen(Qt::black);
    QPoint hPoint_start = QPoint(0,originPoint_.y());
    QPoint hPoint_end = QPoint(this->sceneRect().width(),originPoint_.y());
    QPoint vPoint_start = QPoint(originPoint_.x(),0);
    QPoint vPoint_end = QPoint(originPoint_.x(),this->sceneRect().height());
    painter.drawLine(mapFromScene(hPoint_start),mapFromScene(hPoint_end));
    painter.drawLine(mapFromScene(vPoint_start),mapFromScene(vPoint_end));

    painter.setPen(Qt::red);
    painter.drawRect(this->sceneRect());

    if(intCurGridW != 0 && intCurGridH != 0) {
        painter.setPen(Qt::black);
        QRect rect = QRect(originPoint_.x() - intCurGridW/2,originPoint_.y() - intCurGridH/2,intCurGridW,intCurGridH);
        converPolygon = mapFromScene(rect);
        painter.drawRect(converPolygon.boundingRect());
    }



    //画选中框
    painter.setPen(Qt::blue);
//    if(intChoosedItemIndex != -1) {
        QGraphicsItem *item = getItemAt(intChoosedItemIndex);
        if(item) {
            converPolygon = mapFromScene(item->mapToScene(item->boundingRect()));
            painter.drawRect(converPolygon.boundingRect());
        }
//    }
//    QList<QGraphicsItem*> itemList = this->scene()->items();

//    for(int i = 0; i < itemList.size(); i++) {
//        QGraphicsItem* item = itemList.at(i);
//        QPointF point = mapFromScene(item->mapToScene(item->transformOriginPoint()));
////        painter.drawPoint(point.x(),point.y());
//        painter.drawRect(QRect(point.x(),point.y(),2,2));
//        converPolygon = mapFromScene(item->mapToScene(item->boundingRect()));
//        if(intChoosedItemIndex == i) {
//            painter.drawPixmap(0,0,((QGraphicsPixmapItem*)item)->pixmap());
//            painter.drawRect(converPolygon.boundingRect());
//        }
//    }
}

//void MapSpriteGraphicsView::mouseMoveEvent(QMouseEvent *event)
//{
//    this->scene()->update();
//}

void MapSpriteGraphicsView::setOriginPoint(QPoint origin)
{
    QList<QGraphicsItem*> itemList = this->scene()->items();
    for(int i = 0; i < itemList.size(); i++) {
        QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)itemList[i];
        int x = item->pos().x() - originPoint_.x() + origin.x();
        int y = item->pos().y() - originPoint_.y() + origin.y();
        item->setPos(QPoint(x,y));
    }
    originPoint_ = origin;
    scene()->update();
}

QPoint MapSpriteGraphicsView::getOriginPoint()
{
    return originPoint_;
}

void MapSpriteGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());

   // choosedItem_ = scene()->itemAt(pos.x(),pos.y());
    choosedItem_ = scene()->itemAt(pos.x(),pos.y(), QTransform());
    if(choosedItem_)
    {
        QGraphicsView::mousePressEvent(event);
        QList<QGraphicsItem*> itemList = this->scene()->items();
        for(int i = 0; i < itemList.size(); i++) {
            QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)getItemAt(i);
            if(item == choosedItem_) {
                intChoosedItemIndex = i;
                break;
            }
        }
        if(intChoosedItemIndex != -1)
            spriteWindow_->emit_choosedMapSpriteSubClipChanged(intChoosedItemIndex);
    }
    scene()->update();
}

void MapSpriteGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
//    QGraphicsItem* item = scene()->itemAt(pos.x(),pos.y());
    if(choosedItem_) {
//        QPointF pos_in_item = choosedItem_->mapFromScene(pos.x(),pos.y());
//        QPoint newPos = QPoint(pos.x() - pos_in_item.x(),pos.y() - pos_in_item.y());
//        qDebug()<<(Utility::convertToStringFromPoint(QPoint(pos_in_item.x(),pos_in_item.y())));
//        choosedItem_->setPos(pos.x() - pos_in_item.x(),pos.y() - pos_in_item.y());
//        if(pos.x() < choosedItem_->boundingRect().width()) {
//            pos.setX(choosedItem_->boundingRect().width());
//        }
//        else if(pos.x() > 0) {
//            pos.setX(0);
//        }

//        if(pos.y() < choosedItem_->boundingRect().height()) {
//            pos.setY(choosedItem_->boundingRect().height());
//        }
//        else if(pos.y() > 0) {
//            pos.setY(0);
//        }

        //choosedItem_->setPos(pos.x(),pos.y());
        QGraphicsView::mouseMoveEvent(event);
    }
}

void MapSpriteGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(choosedItem_) {
        QGraphicsView::mouseReleaseEvent(event);
        QPoint pos = QPoint(choosedItem_->pos().x() - originPoint_.x(),choosedItem_->pos().y() - originPoint_.y());

//        if(pos.x() < -choosedItem_->boundingRect().width()) {
//            pos.setX(-choosedItem_->boundingRect().width());
//        }
//        else if(pos.x() > 0) {
//            pos.setX(0);
//        }

//        if(pos.y() < -choosedItem_->boundingRect().height()) {
//            pos.setY(-choosedItem_->boundingRect().height());
//        }
//        else if(pos.y() > 0) {
//            pos.setY(0);
//        }
        spriteWindow_->setMapSpriteSubClipPos(pos);
    }
    choosedItem_ = NULL;
//    intChoosedItemIndex = -1;
    scene()->update();
}

void MapSpriteGraphicsView::setChoosedItemIndex(int index)
{
    intChoosedItemIndex = index;
    scene()->update();
}

int MapSpriteGraphicsView::getChoosedItemIndex()
{
    return intChoosedItemIndex;
}

QGraphicsItem * MapSpriteGraphicsView::getItemAt(int index)
{
    QList<QGraphicsItem*> itemList = this->scene()->items();
    for(int i = itemList.size() - 1,j = 0; i >= 0; i--,j++) {
        if(j == index) {
            QGraphicsItem* item = itemList[i];
            return item;
        }
    }
    return NULL;
}
