/********************************************************************
    created:    2011/07/27
    filename:   Utility.h
    author:     bqlf
    purpose:    公用的工具类 一个命名空间
*********************************************************************/
#ifndef UTILITY_H
#define UTILITY_H
#include <QtWidgets>
#include <QtGui>
#include <QGraphicsPixmapItem>
namespace Utility
{
    //放大
    QPoint scalePoint(QPoint point,int times);
    QPointF scalePointF(QPointF point,int times);
    QRect scaleRect(QRect rect,int times);
    QRectF scaleRectF(QRectF rect,int times);
    //缩小
    QPoint shrinkPoint(QPoint point,int times);
    QPointF shrinkPointF(QPointF point,int times);
    QRect shrinkRect(QRect rect,int times);
    QRectF shrinkRectF(QRectF rect,int times);

    //设置QGraphicsPixMapItem的anchor
    void setAnchor(QGraphicsPixmapItem* item,QPointF anchor);

    //rect 转换为 string
    QString convertToStringFromRect(QRect rect);
    QString convertToStringFromRect(QRectF rect);
    //point 转换为 string
    QString convertToStringFromPoint(QPoint point);
    QString convertToStringFromPoint(QPointF point);
    //int 转换为 string
    QString convertToStringFromInt(int value);
    //float 转换为 string
    QString convertToStringFromFloat(float value);
    //size 转换为 string
    QString converToStringFromSize(QSize size);
    //string 转换为 rect
    QRect convertToRectFromString(QString str);
    //string 转换为 point
    QPoint convertToPointFromString(QString str);
    QPointF convertToPointFFromString(QString str);
    //string 转换为 size
    QSize convertToSizeFromString(QString str);

    //判断point是否在rect
    bool isPointInRect(QPoint point,QRect rect);

    //rect 围绕 一个点 旋转n°后 得出一个新的rect
    QRect rotateRect(QRect rect,QPointF anchorPoint,float angle);
    //一个点围绕另一个点旋转n°后得到一个新的point
    QPointF rotatePoint(QPoint point,QPointF anchorPoint,float angle);
    //anchorPoint在转换后的rorateRect中新的Point
    QPointF pointFromRotateRect(QRect rect,QPointF anchorPoint,float angle);
}

#endif // UTILITY_H
