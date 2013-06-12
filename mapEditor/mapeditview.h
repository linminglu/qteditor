/********************************************************************
    created:    2011/08/05
    filename:   MapEditView.h
    author:     bqlf
    purpose:    地图编辑view
*********************************************************************/
#ifndef MAPEDITVIEW_H
#define MAPEDITVIEW_H

#include <QGraphicsView>
#include "mapspriteitem.h"
#include "mapwindow.h"

class MapWindow;
class MapEditView : public QGraphicsView
{

    Q_OBJECT
public:
//    explicit MapEditView(QWidget *parent = 0);
    MapEditView(MapWindow* mapWindow);
//    ~MapEditView();
    void setCurMapSpriteItem(QString spriteName,QString mapSpriteName);
    //画地图精灵
    void paintMapSprite(QPainter* painter);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void scrollContentsBy(int dx, int dy);
//public:
//    QGraphicsItemGroup *group;
private:
    MapEditView* mapEditView;
    QAction* actModifyInfo;
    MapWindow* mapWindow;

    MapSpriteItem* curMapSpriteItem;
    QString spriteName;
    QString mapSpriteName;
    QPoint curMousePos;
    bool isChoosed;
    bool isMulChoose;


private slots:
    void showModifyInfoDialog();
};

#endif // MAPEDITVIEW_H
