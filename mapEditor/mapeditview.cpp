﻿/********************************************************************
    created:    2011/08/05
    filename:   MapEditView.cpp
    author:     bqlf
    purpose:    地图编辑view
*********************************************************************/
#include "mapeditview.h"
#include <QMouseEvent>
#include <QAction>
#include <QDebug>
#include "config.h"
#include "utility.h"
using namespace Utility;

MapEditView::MapEditView(MapWindow *mapWindow)
{
    mapEditView = this;
    this->mapWindow = mapWindow;
    curMapSpriteItem = NULL;
    isMulChoose=false;
    QGraphicsScene *mapSpriteScene_ = new QGraphicsScene;
    mapSpriteScene_->setSceneRect(0, 0, 200, 200);
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setScene(mapSpriteScene_);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    this->setMouseTracking(true);
    this->setBackgroundRole(QPalette::Dark);
    isChoosed = false;
    setDragMode(QGraphicsView::RubberBandDrag);
    actModifyInfo = new QAction(tr("修改图层"), this);
    QObject::connect(actModifyInfo, SIGNAL(triggered()), mapEditView, SLOT(showModifyInfoDialog()));

}

#include<QMenu>

void MapEditView::mousePressEvent(QMouseEvent *event)
{
    if(this->hasFocus()) {
        QPointF pos = mapToScene(event->pos());
        if(event->button() == Qt::LeftButton) {
            if(mapWindow->getEditMode() == 0)
            {
                if(mapWindow->getMapSpriteName() == "")
                    return;
                mapWindow->createMapLayerData(pos.toPoint());
            }
            else if(mapWindow->getEditMode() == 1)
            {
                MapSpriteItem* item = mapWindow->mapSpriteItemAtPoint(scene(),pos.toPoint());
                if(item)
                {
                    mapWindow->setCurChoosedMapSpriteItemInTree(item,isMulChoose);
                    mapWindow->setCurChoosedMapSpriteItem(item,isMulChoose);
                    scene()->update();
                }
            }
            else if(mapWindow->getEditMode() == 2)
            {
                //开始添加路线
                MapObjectData &curMapObject = mapWindow->getCurMapObjectData();
                //计算所在的网格
                int wIndex = pos.x()/intCurGridW;
                int hIndex = pos.y()/intCurGridH;
                //添加下个节点
                mapWindow->addMapObjectNextPoint(QPoint(wIndex,hIndex));
                //设定当前节点
                curMapObject.curPoint = QPoint(wIndex,hIndex);
                //添加当前节点
                mapWindow->addMapObjectData(QPoint(wIndex,hIndex));
                scene()->update();
            }
            else if(mapWindow->getEditMode() == 3)
            {
                //编辑事件
                //计算所在的网格
                int wIndex = pos.x()/intCurGridW;
                int hIndex = pos.y()/intCurGridH;
                mapWindow->setMapObjectEventId(QPoint(wIndex,hIndex),mapWindow->getCurChoosedEventId());
            }
        }
        else if(event->button() == Qt::RightButton)
        {
            if(mapWindow->getEditMode() == 0) {
                //选中数据
                MapSpriteItem* item = mapWindow->mapSpriteItemAtPoint(scene(),pos.toPoint());
                if(item)
                {
                    mapWindow->setCurChoosedMapSpriteItemInTree(item);
                    mapWindow->setCurChoosedMapSpriteItem(item);
                    scene()->update();
                    //删除数据
                    mapWindow->delMapSprite();
                }
            }
            else if(mapWindow->getEditMode() == 1)
            {
                //弹出菜单
                MapSpriteItem* curChoosedMapSpriteItem = mapWindow->getCurChoosedMapSpriteItem();
                if(curChoosedMapSpriteItem) {
                    QMenu menu(this);
                    menu.addAction(actModifyInfo);

                    menu.exec(mapToGlobal(event->pos()));
                }
            }
            else if(mapWindow->getEditMode() == 2)
            {
                //开始添加路线
                //计算所在的网格
                int wIndex = pos.x()/intCurGridW;
                int hIndex = pos.y()/intCurGridH;
                //删除节点
                mapWindow->delMapObjectData(QPoint(wIndex,hIndex));
                //设定当前节点
                //curMapObject.curPoint = QPoint(wIndex,hIndex);
                scene()->update();
            }
            else if(mapWindow->getEditMode() == 3)
            {
                //清空事件
                //计算所在的网格
                int wIndex = pos.x()/intCurGridW;
                int hIndex = pos.y()/intCurGridH;

                mapWindow->setMapObjectEventId(QPoint(wIndex,hIndex),-1);
            }
        }
        else if(event->button() == Qt::MidButton)
        {
            if(mapWindow->getEditMode() == 2) {
                MapObjectData &curMapObject = mapWindow->getCurMapObjectData();
                //计算所在的网格
                int wIndex = pos.x()/intCurGridW;
                int hIndex = pos.y()/intCurGridH;
                //设定当前节点
                curMapObject.curPoint = QPoint(wIndex,hIndex);
                scene()->update();
            }
        }
        QGraphicsView::mousePressEvent(event);
        isChoosed = true;
    }
}

void MapEditView::mouseMoveEvent(QMouseEvent *event)
{
    curMousePos = mapToScene( event->pos() ).toPoint();

    if(mapWindow->getEditMode() == 1 && isChoosed)
    {
        QGraphicsView::mouseMoveEvent(event);
        if(this->scene()->selectedItems().size()>1)
        {
            MapSpriteItem* curChoosedMapSpriteItem = mapWindow->getCurChoosedMapSpriteItem();
            QList<QGraphicsItem *> selectitemTotal=this->scene()->selectedItems();
            for( QList<QGraphicsItem *> ::Iterator it=selectitemTotal.begin();it!=selectitemTotal.end();it++)
            {
                MapLayerData* mapLayerData =mapWindow->getByMapSpriteItem((MapSpriteItem *)(*it));
                if(mapLayerData)
                {
                    if(mapWindow->getPasteGrid())
                    {
                        if((*it)!=curChoosedMapSpriteItem)
                        {
                            //非当前点击的  不吸附.  是否要添加判断多选的话是否同时吸附呢?
                           // QPoint pos = mapWindow->getPasteGridCenterPoint(curMousePos);
                            //MapSprite* mapSprite = mapWindow->getMapSpriteByName(mapLayerData->spriteName,mapLayerData->mapSpriteName);
                           // QPoint offsetPoint = QPoint(pos.x()+mapSprite->getSubClipList().at(0)->position.x(),
                           //                             pos.y()+mapSprite->getSubClipList().at(0)->position.y());
                            (*it)->setPos(mapLayerData->pos);
                        }
                        else
                        {
                         QPoint pos = mapWindow->getPasteGridCenterPoint(curMousePos);
                         MapSprite* mapSprite = mapWindow->getMapSpriteByName(mapLayerData->spriteName,mapLayerData->mapSpriteName);
                         QPoint offsetPoint = QPoint(pos.x() + mapSprite->getSubClipList().at(0)->position.x(),
                                                    pos.y() + mapSprite->getSubClipList().at(0)->position.y());
                         (*it)->setPos(offsetPoint);
                        }
                    }
                }
                mapWindow->setMapSpriteAndLayerData((MapSpriteItem *)(*it),mapLayerData);
            }
        }
        else
        {
            //移动选中精灵

            MapSpriteItem* curChoosedMapSpriteItem = mapWindow->getCurChoosedMapSpriteItem();
            MapLayerData* mapLayerData = mapWindow->getCurChoosedMapLayerData();
            if(mapLayerData)
            {
                if(mapWindow->getPasteGrid())
                {
                    QPoint pos = mapWindow->getPasteGridCenterPoint(curMousePos);
                    MapSprite* mapSprite = mapWindow->getMapSpriteByName(mapLayerData->spriteName,mapLayerData->mapSpriteName);
                    QPoint offsetPoint = QPoint(pos.x() + mapSprite->getSubClipList().at(0)->position.x(),
                                                pos.y() + mapSprite->getSubClipList().at(0)->position.y());
                    curChoosedMapSpriteItem->setPos(offsetPoint);
                }
            }
            mapWindow->setMapSprite();
        }
    }
    else if (mapWindow->getEditMode() == 2)
    {
        //移动选中图层

    }
    this->scene()->update();
}

void MapEditView::mouseReleaseEvent(QMouseEvent *event)
{
    isChoosed = false;
    //释放选中数据
    //mapWindow->setCurChoosedMapLayerData(NULL);

    QGraphicsView::mouseReleaseEvent(event);
}

void MapEditView::setCurMapSpriteItem(QString spriteName, QString mapSpriteName)
{
    this->spriteName = spriteName;
    this->mapSpriteName = mapSpriteName;
}

void MapEditView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(this->viewport());
    painter.setPen(Qt::red);
    if(mapWindow->getEditMode() == 0) {
        painter.drawText(0,12,tr("编辑模式:"));
    }
    else if(mapWindow->getEditMode() == 1) {
        painter.drawText(0,12,tr("拖拽模式:"));
    }
    else if(mapWindow->getEditMode() == 2) {
        painter.drawText(0,12,tr("路线模式:"));
    }
    else if(mapWindow->getEditMode() == 3) {
        painter.drawText(0,12,tr("事件模式:"));
    }

    QPolygonF converPolygonF;
    QPolygon converPolygon;
    converPolygon = mapFromScene(this->sceneRect());
    painter.drawRect(converPolygon.boundingRect());

    //画网格
    if(mapWindow->getShowGrid())
    {
        painter.setPen(Qt::black);
        for(int i = 0; i <= intCurGridWNum; i++)
        {
            for(int j = 0; j <= intCurGridHNum; j++)
            {
                //横线
                QPoint start = QPoint(0,intCurGridH*j);
                QPoint end = QPoint(intCurGridW*intCurGridWNum,intCurGridH*j);
                painter.drawLine(mapFromScene(start),mapFromScene(end));
                //竖线
                start = QPoint(intCurGridW*i,0);
                end = QPoint(intCurGridW*i,intCurGridH*intCurGridHNum);
                painter.drawLine(mapFromScene(start),mapFromScene(end));
            }
        }
    }
//    if(mapWindow->getEditMode()==1)
//    {
////        for(QList<QGraphicsItem *> ::Iterator it=this->scene()->selectedItems().begin();
////            it!=this->scene()->selectedItems().end();it++)
////        {
//            MapSpriteItem* mapSpriteItem = (MapSpriteItem*)(*it);
//            if(mapSpriteItem)
//            {
//                painter.setPen(Qt::red);
//                converPolygonF = mapSpriteItem->mapToScene(mapSpriteItem->boundingRect().toRect());
//                converPolygon = mapFromScene(converPolygonF);
//                painter.drawRect(converPolygon.boundingRect());
//            }
//        //}

//    }
//    else
//    {
        MapSpriteItem* mapSpriteItem = mapWindow->getCurChoosedMapSpriteItem();
        if(mapSpriteItem)
        {
            painter.setPen(Qt::red);
            converPolygonF = mapSpriteItem->mapToScene(mapSpriteItem->boundingRect().toRect());
            converPolygon = mapFromScene(converPolygonF);
            painter.drawRect(converPolygon.boundingRect());
        }
   // }


    //鼠标跟随画选中精灵
    if(mapWindow->getEditMode() == 0)
    {
        paintMapSprite(&painter);
    }
    else if(mapWindow->getEditMode() == 2 || mapWindow->getEditMode() == 3)
    {
        //画当前的节点
        MapObjectData &curMapObjectData = mapWindow->getCurMapObjectData();
        if(curMapObjectData.curPoint != QPoint(-1,-1)) {
            QPoint curPoint = curMapObjectData.curPoint;
            int wIndex = curPoint.x();
            int hIndex = curPoint.y();
            QPoint drawPoint = mapFromScene(QPoint(wIndex*intCurGridW+intCurGridW/2,hIndex*intCurGridH+intCurGridH/2));
            painter.drawImage(drawPoint.x() - 35,drawPoint.y() - 35,mapWindow->imgShoe);
        }
        //画路线图
        MapData* mapData = mapWindow->getCurMapData();
        if(mapData) {
            for(int i = 0; i < mapData->mapObjectList.size(); i++) {
                MapObjectData* mapObjectData = mapData->mapObjectList.at(i);
                painter.setPen(Qt::red);
                QPoint curPoint = mapObjectData->curPoint;
                int wIndex = mapObjectData->curPoint.x();
                int hIndex = mapObjectData->curPoint.y();
                //画eventId
                QString sEventId = QString("eventId = %1").arg(mapObjectData->eventId);
                QPoint drawPoint = mapFromScene(QPoint(wIndex*intCurGridW+intCurGridW/4,hIndex*intCurGridH+intCurGridH/4));
                painter.drawText(drawPoint,sEventId);
                QString sTag = QString("tag = %1").arg(mapObjectData->tag);
                drawPoint = mapFromScene(QPoint(wIndex*intCurGridW+intCurGridW/4,hIndex*intCurGridH+intCurGridH/4*3));
                painter.drawText(drawPoint,sTag);

                drawPoint = mapFromScene(QPoint(wIndex*intCurGridW+intCurGridW/2,hIndex*intCurGridH+intCurGridH/2));
                for(int j = 0; j < mapObjectData->nextPointList.size(); j++) {
                    QPoint nextPoint = mapObjectData->nextPointList.at(j);
                    if(curPoint.x() == nextPoint.x()&& curPoint.y() == nextPoint.y() + 1) {
                        //上
                        painter.drawImage(drawPoint.x() - 16 ,drawPoint.y() - 5 - 32,mapWindow->imgUp);
                    }
                    else if(curPoint.x() == nextPoint.x() && curPoint.y() == nextPoint.y() - 1) {
                        //下
                        painter.drawImage(drawPoint.x() - 16,drawPoint.y() + 5,mapWindow->imgDown);
                    }
                    else if(curPoint.x() == nextPoint.x() + 1 && curPoint.y() == nextPoint.y()) {
                        //左
                        painter.drawImage(drawPoint.x() - 5 - 32,drawPoint.y()-16,mapWindow->imgLeft);
                    }
                    else if(curPoint.x() == nextPoint.x() - 1 && curPoint.y() == nextPoint.y()) {
                        //右
                        painter.drawImage(drawPoint.x() + 5,drawPoint.y()-16,mapWindow->imgRight);
                    }
                }
            }
        }
    }
}

void MapEditView::paintMapSprite(QPainter *painter)
{
    if(mapWindow->getMapSpriteName() == "")
        return;
    MapSprite* mapSprite = mapWindow->getCurMapSprite();
    //    QImage image = mapWindow->getStChooseMapSpriteImage();
    if(mapSprite && mapSprite->subClipList_.size() > 0) {
        QPointF convertPoint = mapFromScene(curMousePos);

        MapSpriteSubClip* subClip = mapSprite->subClipList_.at(0);
        QPointF rotatePoint = mapSprite->anchorOffsetPoint;
        //网格吸附
        if(mapWindow->getPasteGrid())
        {
            QPoint gridPointCenter = mapWindow->getPasteGridCenterPoint(curMousePos);
            QPoint curPoint = QPoint(gridPointCenter.x() - rotatePoint.x(),gridPointCenter.y()
                                     - rotatePoint.y());
            //最后转换坐标
            QPointF pointF = mapFromScene(curPoint);
            //偏移量
            pointF.setX(pointF.x() + subClip->position.x());
            pointF.setY(pointF.y() + subClip->position.y());
            painter->drawImage(pointF,mapSprite->image);
            if(isShowChooseSpriteRect) {
                painter->setPen(Qt::blue);
                painter->drawRect(QRect(pointF.x(),pointF.y(),mapSprite->image.width(),mapSprite->image.height()));
            }

        }
        else {
            QPoint curPoint = QPoint(convertPoint.x() - rotatePoint.x(),convertPoint.y()
                                     - rotatePoint.y());
            //偏移量
            curPoint.setX(curPoint.x() + subClip->position.x());
            curPoint.setY(curPoint.y() + subClip->position.y());
            painter->drawImage(curPoint,mapSprite->image);
            if(isShowChooseSpriteRect) {
                painter->setPen(Qt::blue);
                painter->drawRect(QRect(curPoint.x(),curPoint.y(),mapSprite->image.width(),mapSprite->image.height()));
            }

        }
    }
}

void MapEditView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx,dy);
    this->scene()->update();
}

void MapEditView::keyPressEvent(QKeyEvent *event)
{
    MapSpriteItem* curChoosedMapSpriteItem = mapWindow->getCurChoosedMapSpriteItem();
    MapLayerData* mapLayerData = mapWindow->getCurChoosedMapLayerData();
    if(curChoosedMapSpriteItem && mapLayerData != NULL)
    {
        QPoint offsetPoint = curChoosedMapSpriteItem->pos().toPoint();
        if(event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
        {
            offsetPoint = QPoint(curChoosedMapSpriteItem->pos().x(),curChoosedMapSpriteItem->y() - 1);
        }
        else if(event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
            offsetPoint = QPoint(curChoosedMapSpriteItem->pos().x(),curChoosedMapSpriteItem->y() + 1);
        }
        else if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
            offsetPoint = QPoint(curChoosedMapSpriteItem->pos().x() - 1,curChoosedMapSpriteItem->y());
        }
        else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
            offsetPoint = QPoint(curChoosedMapSpriteItem->pos().x() + 1,curChoosedMapSpriteItem->y());
        }
        curChoosedMapSpriteItem->setPos(offsetPoint);
        mapWindow->setMapSprite();
    }



}



void MapEditView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        if(isMulChoose)
        {
            isMulChoose=false;
            mapWindow->getCurMulChooseMapLayerDatas()->clear();
            mapWindow->getCurMulChooseMapSpriteItems()->clear();
        }
    }
}
//弹出修改图层窗口
void MapEditView::showModifyInfoDialog()
{
    qDebug()<<"showModifyInfoDialog";
    ModifyMapSpriteInfoDialog modifyInfoDialog(mapWindow);
    modifyInfoDialog.exec();
}
