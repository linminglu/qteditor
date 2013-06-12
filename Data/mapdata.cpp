#include "mapdata.h"

MapData::MapData()
{
    mapFilePath = "";
    mapName = "map1";
    mapSize = QSize(100,100);
    backGroundColor.setRgb(0,0,0);
}

void MapData::addMapLayer(MapLayer *layer)
{
    layerList.append(layer);
}

void MapData::delMapLayer(MapLayer *layer)
{
    layerList.removeOne(layer);
}

void MapData::insertMapLayer(MapLayer* layer,int index)
{
    layerList.insert(index,layer);
}

void MapData::addMapObjectData(MapObjectData* object)
{
    mapObjectList.append(object);
}

void MapData::delMapObjectData(MapObjectData* object)
{
    mapObjectList.removeOne(object);
}

MapLayer::MapLayer()
{
    intMaxTag = 0;
}

void MapLayer::addMapLayerData(MapLayerData *layerData)
{
    layerDataList.append(layerData);
}

void MapLayer::delMapLayerData(MapLayerData *layerData)
{
    layerDataList.removeOne(layerData);
}

void MapLayer::insertMapLayerData(MapLayerData* layerData,int index)
{
    layerDataList.insert(index,layerData);
}

MapLayerData::MapLayerData()
{
}

MapLayer * MapLayer::getMapLayerClone()
{
    MapLayer* layer = new MapLayer();
    layer->mapLayerName = this->mapLayerName;
    for(int i = 0; i < this->layerDataList.size(); i++) {
        MapLayerData* layerData = new MapLayerData();
        MapLayerData* copyLayerData = this->layerDataList.at(i);
        layerData->mapSpriteName = copyLayerData->mapSpriteName;
        layerData->spriteName = copyLayerData->spriteName;
        layerData->pos = copyLayerData->pos;
        layerData->z = copyLayerData->z;
        layerData->tag = copyLayerData->tag;
        layer->addMapLayerData(layerData);
    }
    layer->intMaxTag = this->intMaxTag;
    return layer;
}

MapLayerData * MapLayerData::getMapLayerDataClone()
{
    MapLayerData* layerData = new MapLayerData();
    layerData->mapSpriteName = this->mapSpriteName;
    layerData->spriteName = this->spriteName;
    layerData->pos = this->pos;
    layerData->tag = this->tag;
    layerData->z = this->z;
    return layerData;
}

MapObjectData::MapObjectData()
{
    curPoint = QPoint(-1,-1);
    eventId = -1;
    z = 0;
    tag = -1;
}

MapObjectData::MapObjectData(QPoint point)
{
    curPoint = point;
    eventId = -1;
    z = 0;
    tag = -1;
}

MapObjectData * MapObjectData::getMapObjectDataClone()
{
    MapObjectData* mapObjectData = new MapObjectData();
    mapObjectData->curPoint = this->curPoint;
    mapObjectData->eventId = this->eventId;
    for(int i = 0; i < this->nextPointList.size(); i++) {
        mapObjectData->nextPointList.append(this->nextPointList.at(i));
    }
    return mapObjectData;
}

