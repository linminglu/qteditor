#ifndef MAPTAB_H
#define MAPTAB_H

#include <QWidget>
#include <QList>
#include "mapwindow.h"
#include "mapeditview.h"
#include "Data/spritedata.h"

class MapWindow;
class MapEditView;
class MapTab : public QWidget
{
    Q_OBJECT
//    friend class MapWindow;
public:
    explicit MapTab(QWidget *parent = 0);
    MapTab(MapWindow* mapWindow);
    ~MapTab();
    //添加spritedata
//    void addSpriteData(SpriteData* spriteData);

    MapEditView* mapEditView;
private:
    MapWindow* mapWindow_;
//    //所需的spritedata
//    QList<SpriteData*> spriteDataList;
signals:

public slots:

};

#endif // MAPTAB_H
