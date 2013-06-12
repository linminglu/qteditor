/********************************************************************
    created:    2011/08/04
    filename:   ChooseMapSpriteView.h
    author:     bqlf
    purpose:    显示地图精灵view
*********************************************************************/
#ifndef CHOOSEMAPSPRITEVIEW_H
#define CHOOSEMAPSPRITEVIEW_H

#include <QGraphicsView>
#include <Data/spritedata.h>
#include "mapspriteitem.h"
#include "mapwindow.h"

class ChooseMapSpriteView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChooseMapSpriteView(QWidget *parent = 0);
    ChooseMapSpriteView(SpriteData* spriteData);

    void setMapWindow(MapWindow* mapWindow) {
        mapWindow_ = mapWindow;
    }

    QString getSpriteName() {
        return spriteData_->spriteName_;
    }

    int getCurMapSpriteIndex();

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    MapWindow* mapWindow_;
    SpriteData* spriteData_;
    MapSpriteItem* choosedMapSpriteItem;
    int intChoosedMapSpriteIndex;//namelist index 不是itemList的index(相反的)
    QStringList nameList;
signals:

public slots:

};

#endif // CHOOSEMAPSPRITEVIEW_H
