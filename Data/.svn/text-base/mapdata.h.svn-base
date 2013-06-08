#ifndef MAPDATA_H
#define MAPDATA_H

#include <QList>
#include <QString>
#include <QSize>
#include <QPoint>
#include <QColor>

class MapLayer;
class MapLayerData;
class MapObjectData;
class MapData
{
public:
    MapData();
    void addMapLayer(MapLayer* layer);
    void delMapLayer(MapLayer* layer);
    void insertMapLayer(MapLayer* layer,int index);
    void addMapObjectData(MapObjectData* object);
    void delMapObjectData(MapObjectData* object);
//private:
    QList<MapLayer*> layerList;
    QList<MapObjectData*> mapObjectList;
    QString mapFilePath;
    QColor backGroundColor;
    //QString mapBackGroundColor;
    QString mapName;
    QSize mapSize;
    int intGridW;
    int intGridH;
    int intGridWNum;
    int intGridHNum;
};

class MapLayer
{
public:
    MapLayer();
    MapLayer* getMapLayerClone();
//private:
    QString mapLayerName;
    void addMapLayerData(MapLayerData* layerData);
    void delMapLayerData(MapLayerData* layerData);
    void insertMapLayerData(MapLayerData* layerData,int index);
    QList<MapLayerData*> layerDataList;
    int intMaxTag;
};

class MapLayerData
{
public:
    MapLayerData();
    MapLayerData* getMapLayerDataClone();
    QString spriteName;
    QString mapSpriteName;
    QPoint pos;
    int z;
    int tag;
};

//存储路线和事件
class MapObjectData
{
public:
    MapObjectData();
    MapObjectData(QPoint point);
    MapObjectData* getMapObjectDataClone();
    QPoint curPoint;//本身位置
    QList<QPoint> nextPointList;//下一个点的位置
    int eventId;//事件id -1无事件
    int z;//主角的z值
    int tag;//图标的tag 图标的z即是主角的z
};

#endif // MAPDATA_H
