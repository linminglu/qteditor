#include "clipgraphicsview.h"
#include <QMouseEvent>
#include <Data/spritedata.h>
#include "Command/ClipEditCommand/ClipEditCommand.h"
#include "utility.h"

using namespace Utility;

ClipGraphicsView::ClipGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    this->setMouseTracking(true);
    intChoosedClipIndex = -1;
}

void ClipGraphicsView::setSpriteData(SpriteData *spriteData)
{
    spriteData_ = spriteData;
}

void ClipGraphicsView::setSpriteWindown(SpriteWindow *spriteWindow)
{
    spriteWindow_ = spriteWindow;
}

void ClipGraphicsView::paintEvent(QPaintEvent *event)
{
    QPolygon converPolygon;
    QGraphicsView::paintEvent(event);
    QPainter painter(this->viewport());
//    painter.setBrush(Qt::blue);
    painter.setPen(Qt::red);
    //横线
    painter.drawLine(
                mapFromScene(scalePointF(QPointF(0,curMousePos_.y()),intZoomTimes)),
                mapFromScene(scalePointF(QPointF(this->sceneRect().width(),curMousePos_.y()),intZoomTimes))
                );
    //竖线
    painter.drawLine(
                mapFromScene(scalePointF(QPointF(curMousePos_.x(),0),intZoomTimes)),
                mapFromScene(scalePointF(QPointF(curMousePos_.x(),this->sceneRect().height()),intZoomTimes))
                );

    //正在切割的区域
    painter.setPen(Qt::blue);
    converPolygon = mapFromScene(scaleRect(clipRect_,intZoomTimes));
    painter.drawPolygon(converPolygon.boundingRect());

    //画切片
    painter.setPen(Qt::green);
    QList<Clip*> clipList = spriteData_->getClipList();
    QList<Clip*>::iterator clipIndex;
    for(clipIndex = clipList.begin();clipIndex != clipList.end(); ++clipIndex) {
        Clip* clip = (Clip*)*clipIndex;
        converPolygon = mapFromScene(scaleRect(clip->clipRect,intZoomTimes));
        painter.drawRect(converPolygon.boundingRect());
    }

    //选中的切片
    if(intChoosedClipIndex != -1) {
        painter.setPen(Qt::red);
        Clip* choosedClip = spriteData_->clipList_[intChoosedClipIndex];
        converPolygon = mapFromScene(scaleRect(choosedClip->clipRect,intZoomTimes));
        painter.drawRect(converPolygon.boundingRect());
    }

}


void ClipGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(spriteData_->imagePath_.isEmpty()) {
        return;
    }
//    this->mapToScene()
    QPoint converPoint = QPoint(event->pos().x()/intZoomTimes*intZoomTimes,event->pos().y()/intZoomTimes*intZoomTimes);
    QPointF pointF = mapToScene(converPoint);
    curMousePos_ = shrinkPoint(QPoint(pointF.x(),pointF.y()),intZoomTimes);

    if(scene()->items().size() > 0) {
        QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)scene()->items().at(0);
        if(curMousePos_.x() < 0) {
            curMousePos_.setX(0);
        }
        else if(curMousePos_.x() >= item->boundingRect().width() - 1) {
            curMousePos_.setX(item->boundingRect().width() - 1);
        }

        if(curMousePos_.y() < 0) {
            curMousePos_.setY(0);
        }
        else if(curMousePos_.y() >= item->boundingRect().height() - 1) {
            curMousePos_.setY(item->boundingRect().height() - 1);
        }
    }
//    qDebug("pos x = %d, y = %d",curMousePos_.x(),curMousePos_.y());
    this->setStatusTip(QString("(%1,%2)").arg(curMousePos_.x()).arg(curMousePos_.y()));
//    this->statusTip().updateProperties();
    if(event->buttons() == Qt::LeftButton) {
        //区域
        int quadrant = comparePoint(mousePressedPos_,curMousePos_);

        switch(quadrant) {
        case 1://第一象限
            clipRect_.setBottomLeft(mousePressedPos_);
            clipRect_.setTopRight(curMousePos_);
//            clipRect_.setTopRight(QPoint(curMousePos_.x()- 2,curMousePos_.y()));
            break;
        case 2://第二象限
            clipRect_.setTopLeft(curMousePos_);
            clipRect_.setBottomRight(mousePressedPos_);
//            clipRect_.setBottomRight(QPoint(mousePressedPos_.x()- 2,mousePressedPos_.y() - 2));
            break;
        case 3:
            clipRect_.setBottomLeft(curMousePos_);
//            clipRect_.setBottomLeft(QPoint(curMousePos_.x(),curMousePos_.y()- 2));
            clipRect_.setTopRight(mousePressedPos_);
//            clipRect_.setTopRight(QPoint(mousePressedPos_.x()- 2,mousePressedPos_.y()));
            break;
        case 4:
            clipRect_.setTopLeft(mousePressedPos_);
            clipRect_.setBottomRight(curMousePos_);
//            clipRect_.setBottomRight(QPoint(curMousePos_.x()- 2,curMousePos_.y() - 2));
            break;
        }

    }
//    setUpdatesEnabled(true);
//    ClipGraphicsView::repaint();
    this->scene()->update();
//    repaint();
//    qDebug("sssssssssssssss");
}

void ClipGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QPoint converPoint = QPoint(event->pos().x()/intZoomTimes*intZoomTimes,event->pos().y()/intZoomTimes*intZoomTimes);
    QPointF pointF = mapToScene(converPoint);
    mousePressedPos_ = shrinkPoint(QPoint(pointF.x(),pointF.y()),intZoomTimes);

    if(scene()->items().size() > 0) {
        QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)scene()->items().at(0);
        if(mousePressedPos_.x() < 0) {
            mousePressedPos_.setX(0);
        }
        else if(mousePressedPos_.x() >= item->boundingRect().width() - 1) {
            mousePressedPos_.setX(item->boundingRect().width() - 1);
        }

        if(mousePressedPos_.y() < 0) {
            mousePressedPos_.setY(0);
        }
        else if(mousePressedPos_.y() >= item->boundingRect().height() - 1) {
            mousePressedPos_.setY(item->boundingRect().height() - 1);
        }
    }

    //判断点是否在图片区域内

    //计算选择了哪个切片
    pointF = mapToScene(converPoint);
    QPoint pos = shrinkPoint(QPoint(pointF.x(),pointF.y()),intZoomTimes);

    int index = getChoosedClip(pos);
    if(index != -1) {
        spriteWindow_->emit_choosedClipChanged(index);
    }

}

void ClipGraphicsView::mouseReleaseEvent(QMouseEvent *)
{
    if(clipRect_.width() != 0 && clipRect_.height() != 0) {
        spriteWindow_->cmd_addClip();
        //清空数据
        mousePressedPos_.setX(0);
        mousePressedPos_.setY(0);
        clipRect_.setWidth(0);
        clipRect_.setHeight(0);
    }
}

int ClipGraphicsView::comparePoint(QPoint centerPoint, QPoint otherPoint)
{
    int quadrant = 0;
    if(otherPoint.x() > centerPoint.x()) {
        if(otherPoint.y() > centerPoint.y()) {
            quadrant = 4;
        }
        else {
            quadrant = 1;
        }
    }
    else {
        if(otherPoint.y() > centerPoint.y()) {
            quadrant = 3;
        }
        else {
            quadrant = 2;
        }
    }
    return quadrant;
}

int ClipGraphicsView::getChoosedClip(QPoint point)
{
    for(int i = 0; i < spriteData_->getClipList().size(); i++) {
        Clip* clip = spriteData_->clipList_[i];
        if(point.x() >= clip->clipRect.x() && point.x() <= clip->clipRect.x()+clip->clipRect.width()
                && point.y() >= clip->clipRect.y() && point.y() <= clip->clipRect.y() + clip->clipRect.height()) {
            return i;
        }
    }
    return -1;
}

void ClipGraphicsView::setClipRect(QRect rect)
{
    clipRect_ = rect;
}

QRect ClipGraphicsView::getClipRect()
{
    return clipRect_;
}

void ClipGraphicsView::slot_updateChoosedClip(int index)
{
    intChoosedClipIndex = index;
    scene()->update();
}

void ClipGraphicsView::setZoomTimes(int times)
{
    intZoomTimes = times;
    scene()->update();
}


