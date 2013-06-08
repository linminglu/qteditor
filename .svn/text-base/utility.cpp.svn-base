/********************************************************************
    created:    2011/07/27
    filename:   Utility.cpp
    author:     bqlf
    purpose:    公用的工具类 一个命名空间
*********************************************************************/
#include "utility.h"
#include <stdlib.h>

namespace Utility
{
    QPoint scalePoint(QPoint point,int times) {
        return QPoint(point.x()*times,point.y()*times);
    }

    QPointF scalePointF(QPointF point,int times) {
        return QPointF(point.x()*times,point.y()*times);
    }

    QRect scaleRect(QRect rect,int times) {
        return QRect(rect.x()*times,rect.y()*times,rect.width()*times,rect.height()*times);
    }

    QRectF scaleRectF(QRectF rect,int times) {
        return QRectF(rect.x()*times,rect.y()*times,rect.width()*times,rect.height()*times);
    }


    QPoint shrinkPoint(QPoint point,int times) {
        return QPoint(point.x()*100/times/100,point.y()*100/times/100);
    }

    QPointF shrinkPointF(QPointF point,int times) {
        return QPointF(point.x()*100/times/100,point.y()*100/times/100);
    }

    QRect shrinkRect(QRect rect,int times) {
        return QRect(rect.x()*100/times/100,rect.y()*100/times/100,rect.width()*100/times/100,rect.height()*100/times/100);
    }

    QRectF shrinkRectF(QRectF rect,int times) {
        return QRectF(rect.x()*100/times/100,rect.y()*100/times/100,rect.width()*100/times/100,rect.height()*100/times/100);
    }

    //设置QGraphicsPixMapItem的anchor
    void setAnchor(QGraphicsPixmapItem* item,QPointF anchor) {

        //设置画图的偏移量 默认都是以左上角为坐标,所以只能设置offset来改变锚点
        item->setOffset(-item->boundingRect().width() * anchor.x(),item->boundingRect().height() * (anchor.y() - 1));

        //设置翻转,旋转等 锚点 还要加上offset 要不然还是基于原来的左上角
        item->setTransformOriginPoint(item->boundingRect().width() * anchor.x()
                                      - item->boundingRect().width() * anchor.x(),
                                      item->boundingRect().height() * (1 - anchor.y())
                                      + item->boundingRect().height() * (anchor.y() - 1));
    }

    //rect 转换为 string
    QString convertToStringFromRect(QRect rect) {
        QString str = QString("{%1,%2,%3,%4}").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
        return str;
    }
    QString convertToStringFromRect(QRectF rect) {
        QString str = QString("{%1,%2,%3,%4}").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
        return str;
    }

    //point 转换为 string
    QString convertToStringFromPoint(QPoint point) {
        QString str = QString("{%1,%2}").arg(point.x()).arg(point.y());
        return str;
    }
    QString convertToStringFromPoint(QPointF point) {
        QString str = QString("{%1,%2}").arg(point.x()).arg(point.y());
        return str;
    }
    //int 转换为 string
    QString convertToStringFromInt(int value) {
        QString str;
        str.setNum(value);
        return str;
    }
    //float 转换为 string
    QString convertToStringFromFloat(float value) {
        QString str;
        str.setNum(value);
        return str;
    }
    //size 转换为 string
    QString converToStringFromSize(QSize size) {
        QString str = QString("{%1,%2}").arg(size.width()).arg(size.height());
        return str;

    }

    //string 转换为 rect
    QRect convertToRectFromString(QString str) {
        QRect rect;
        QStringList strList = str.split(",");
        rect.setX(strList.at(0).mid(1).toInt());
        rect.setY(strList.at(1).toInt());
        rect.setWidth(strList.at(2).toInt());
        QString strH = strList.at(3);
        strH.chop(1);
        rect.setHeight(strH.toInt());
        return rect;
    }
    //string 转换为 point
    QPoint convertToPointFromString(QString str) {
        QPoint point;
        QStringList strList = str.split(",");
        point.setX(strList.at(0).mid(1).toInt());
        QString strY = strList.at(1);
        strY.chop(1);
        point.setY(strY.toInt());
        return point;
    }
    QPointF convertToPointFFromString(QString str) {
        QPointF point;
        QStringList strList = str.split(",");
        point.setX(strList.at(0).mid(1).toFloat());
        QString strY = strList.at(1);
        strY.chop(1);
        point.setY(strY.toFloat());
        return point;
    }

    //string 转换为 size
    QSize convertToSizeFromString(QString str) {
        QSize size;
        QStringList strList = str.split(",");
        size.setWidth(strList.at(0).mid(1).toInt());
        QString strY = strList.at(1);
        strY.chop(1);
        size.setHeight(strY.toInt());
        return size;
    }

    //判断point是否在rect
    bool isPointInRect(QPoint point,QRect rect) {
        if(point.x() > rect.x() && point.x() < (rect.x() + rect.width())
                && point.y() > rect.y() && point.y() < (rect.y() + rect.height())) {
            return true;
        }
        return false;
    }

    //rect 围绕 一个点 旋转n°后 得出一个新的rect
    QRect rotateRect(QRect rect,QPointF anchorPoint,float angle) {
        //求出旋转后得四点
        QPoint topLeft = rect.topLeft();
        QPoint topRight = rect.topRight();
        QPoint bottomLeft = rect.bottomLeft();
        QPoint bottomRight = rect.bottomRight();
        QPointF leftupPoint = rotatePoint(topLeft,anchorPoint,angle);
        QPointF rightupPoint = rotatePoint(QPoint(topRight.x()+1,topRight.y()),anchorPoint,angle);
        QPointF leftdownPoint = rotatePoint(QPoint(bottomLeft.x(),bottomLeft.y()+1),anchorPoint,angle);
        QPointF rightdownPoint = rotatePoint(QPoint(bottomRight.x()+1,bottomRight.y()+1),anchorPoint,angle);

        qreal minX = leftupPoint.x();
        minX = qMin(minX,rightupPoint.x());
        minX = qMin(minX,leftdownPoint.x());
        minX = qMin(minX,rightdownPoint.x());

        qreal maxX = leftupPoint.x();
        maxX = qMax(maxX,rightupPoint.x());
        maxX = qMax(maxX,leftdownPoint.x());
        maxX = qMax(maxX,rightdownPoint.x());

        qreal minY = leftupPoint.y();
        minY = qMin(minY,rightupPoint.y());
        minY = qMin(minY,leftdownPoint.y());
        minY = qMin(minY,rightdownPoint.y());

        qreal maxY = leftupPoint.y();
        maxY = qMax(maxY,rightupPoint.y());
        maxY = qMax(maxY,leftdownPoint.y());
        maxY = qMax(maxY,rightdownPoint.y());

        QRect newRect;
        qreal width = maxX - minX;
        qreal height = maxY - minY;
        newRect.setRect(0,0,width,height);
//        newRect.setTopLeft(QPoint(minX,maxY));
//        newRect.setTopRight(QPoint(maxX,maxY));
//        newRect.setBottomLeft(QPoint(minX,minY));
//        newRect.setBottomRight(QPoint(maxX,minY));

        return newRect;
    }

    //anchorPoint在转换后的rorateRect中新的Point
    QPointF pointFromRotateRect(QRect rect,QPointF anchorPoint,float angle)
    {
        //求出旋转后得四点
//        qDebug()<<rect.topLeft();
//        qDebug()<<rect.topRight();
//        qDebug()<<rect.bottomLeft();
//        qDebug()<<rect.bottomRight();
        QPoint topLeft = rect.topLeft();
        QPoint topRight = rect.topRight();
        QPoint bottomLeft = rect.bottomLeft();
        QPoint bottomRight = rect.bottomRight();
        QPointF leftupPoint = rotatePoint(topLeft,anchorPoint,angle);
        QPointF rightupPoint = rotatePoint(QPoint(topRight.x()+1,topRight.y()),anchorPoint,angle);
        QPointF leftdownPoint = rotatePoint(QPoint(bottomLeft.x(),bottomLeft.y()+1),anchorPoint,angle);
        QPointF rightdownPoint = rotatePoint(QPoint(bottomRight.x()+1,bottomRight.y()+1),anchorPoint,angle);

        qreal minX = leftupPoint.x();
        minX = qMin(minX,rightupPoint.x());
        minX = qMin(minX,leftdownPoint.x());
        minX = qMin(minX,rightdownPoint.x());

        qreal maxX = leftupPoint.x();
        maxX = qMax(maxX,rightupPoint.x());
        maxX = qMax(maxX,leftdownPoint.x());
        maxX = qMax(maxX,rightdownPoint.x());

        qreal minY = leftupPoint.y();
        minY = qMin(minY,rightupPoint.y());
        minY = qMin(minY,leftdownPoint.y());
        minY = qMin(minY,rightdownPoint.y());

        qreal maxY = leftupPoint.y();
        maxY = qMax(maxY,rightupPoint.y());
        maxY = qMax(maxY,leftdownPoint.y());
        maxY = qMax(maxY,rightdownPoint.y());

        //坐标系统x y↓左上角应该是(minX,minY)
        qreal x = anchorPoint.x() - minX;
        qreal y = anchorPoint.y() - minY;

//        qDebug()<<minX;
//        qDebug()<<maxY;
//        qDebug()<<anchorPoint;
//        qDebug()<<QPoint(x,y);
        return QPointF(x,y);
    }

    //一个点围绕另一个点旋转n°后得到一个新的point
    QPointF rotatePoint(QPoint point,QPointF anchorPoint,float angle) {
        QPointF newPoint;
        float increment = angle /180 * M_PI;

        float x = anchorPoint.x() + (point.x() - anchorPoint.x()) * cos(increment) - (point.y() - anchorPoint.y()) * sin(increment);
        float y = anchorPoint.y() + (point.y() - anchorPoint.y()) * cos(increment) + (point.x() - anchorPoint.x()) * sin(increment);
        newPoint.setX(x);
        newPoint.setY(y);
//        qDebug() << "before" << point;
//        qDebug() << "after" << newPoint;
        return newPoint;

    }

}

