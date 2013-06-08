#include<QtWidgets>
#include "maptab.h"
#include "choosemapspriteview.h"
#include <QtWidgets>
#include <QtWidgets/QtWidgets>

MapTab::MapTab(QWidget *parent) :
    QWidget(parent)
{
}

MapTab::MapTab(MapWindow* mapWindow)
{
    mapWindow_ = mapWindow;
    //初始化组件
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    QScrollArea *scrollArea = new QScrollArea;
    layout->addWidget(scrollArea);
    this->setLayout(layout);

    QGridLayout *scrollAreaLayout = new QGridLayout;
    scrollAreaLayout->setContentsMargins(0,0,0,0);
    //    QGraphicsScene scene;
    //    scene.setSceneRect(0, 0, 5000, 5000);
    //    QGraphicsView *view = new QGraphicsView(&scene);
    mapEditView = new MapEditView(mapWindow);
    scrollAreaLayout->addWidget(mapEditView);
    scrollArea->setLayout(scrollAreaLayout);

}

MapTab::~MapTab() {
    delete mapEditView;
    mapEditView = NULL;

//    for(int i = 0; i < spriteDataList.size(); i++) {
//        delete spriteDataList.at(i);
//        spriteDataList.at(i) = NULL;
    //    }
}

//void MapTab::addSpriteData(SpriteData *spriteData)
//{
//    spriteDataList.append(spriteData);
//}
