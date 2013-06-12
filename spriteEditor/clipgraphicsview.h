#ifndef CLIPGRAPHICSVIEW_H
#define CLIPGRAPHICSVIEW_H

#include <QGraphicsView>
#include "Data/spritedata.h"
#include "spriteEditor/spritewindow.h"

class ClipGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ClipGraphicsView(QWidget *parent = 0);
    void setSpriteData(SpriteData* spriteData);
    void setSpriteWindown(SpriteWindow* spriteWindow);

    void setClipRect(QRect rect);
    QRect getClipRect();

    void setZoomTimes(int times);
        SpriteData* spriteData_;
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:

public slots:
    void slot_updateChoosedClip(int index);

private:
    // 返回象限值 相对centerPoint [1,4];
    int comparePoint(QPoint centerPoint,QPoint otherPoint);
    int getChoosedClip(QPoint point);

    QPoint curMousePos_;
    QPoint mousePressedPos_;

//    SpriteData* spriteData_;
    SpriteWindow* spriteWindow_;

    QRect clipRect_;
    int intChoosedClipIndex;

    int intZoomTimes;
//    bool isFocus;


};

#endif // CLIPGRAPHICSVIEW_H
