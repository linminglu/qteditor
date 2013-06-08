/********************************************************************
    created:    2011/08/01
    filename:   IOData.cpp
    author:     bqlf
    purpose:    编辑器的读写数据
*********************************************************************/
#include "iodata.h"
#include <QFileDialog>
#include <QtGui>
#include <QDebug>
#include "Utility.h"
#include "config.h"

using namespace Utility;

IOData* IOData::ioData_ = NULL;

IOData::IOData()
{
}

IOData::~IOData()
{
    delete ioData_;
    ioData_ = NULL;
}

IOData * IOData::getInstance()
{
    if(ioData_ == NULL) {
        ioData_ = new IOData;
    }
    return ioData_;
}

void IOData::do_newSpriteFile()
{
    strSpriteFilePath = "";
}

SpriteData * IOData::do_ReadSprite(QString fileName,bool import)
{
    QDomDocument doc;
    if(!import)
    {
        //非导入是才保存
        strSpriteFilePath = fileName;
    }
    if(!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
            return NULL;

        if (!doc.setContent(&file))
        {
            file.close();
            return NULL;
        }
        file.close();
        QDomElement root = doc.documentElement();

        SpriteData* spriteData = new SpriteData;
        spriteData->spriteName_ = getValueByKey(&root,"spriteFileName");
        //保证文件名字一样
        spriteData->spriteName_ = fileInfo.baseName();
        spriteData->imagePath_ = getValueByKey(&root,"imagePath");
        //载入图片
        spriteData->spriteImage = QImage(spriteData->imagePath_);

        QDomElement clipDataElement = getValueElementByKey(&root,"clipData");
        QDomNodeList clipDataNodeList = clipDataElement.childNodes();
        for(int i = 0; i < clipDataNodeList.size(); i++) {
            QDomNode n = clipDataNodeList.at(i);
            if(n.toElement().tagName() == "key") {
                Clip* clip = new Clip;
                clip->clipName = n.toElement().text();
                clip->clipRect = convertToRectFromString(getValueByKey(&clipDataElement,clip->clipName));
                spriteData->addClip(clip);
            }
        }

        QDomElement mapSpriteDataElement = getValueElementByKey(&root,"mapSpriteData");
        QDomNodeList mapSpriteDataNodeList = mapSpriteDataElement.childNodes();
        //填从mapSpritelist
        for(int i = 0; i < mapSpriteDataNodeList.size(); i++) {
            QDomNode n = mapSpriteDataNodeList.at(i);
            if(n.toElement().tagName() == "key") {
                MapSprite* mapSprite = new MapSprite;
                //地图精灵名字
                mapSprite->mapSpriteName = n.toElement().text();
//                qDebug()<<mapSprite->mapSpriteName;
                QDomElement mapSpriteSubClipDataElement = getValueElementByKey(&mapSpriteDataElement,mapSprite->mapSpriteName);
                //填冲subCliplist
                QDomNodeList mapSpriteSubClipDataNodeList = mapSpriteSubClipDataElement.childNodes();
                for(int j = 0; j < mapSpriteSubClipDataNodeList.size();j++) {
                    QDomNode subClipNode = mapSpriteSubClipDataNodeList.at(j);
                    QDomElement subClipElement = subClipNode.toElement();
                    if(subClipElement.tagName() == "key") {
                        MapSpriteSubClip* subClip = new MapSpriteSubClip;
                        QString clipName = subClipElement.text();
                        QDomElement subClipDataElement = getValueElementByKey(&mapSpriteSubClipDataElement,clipName);
                        //临时转换
                        //QPointF anchor = convertToPointFFromString(getValueByKey(&subClipDataElement,"anchor"));;

                        //clip
                        subClip->clip = spriteData->getClipByName(clipName);
                        //pos
                        subClip->position = convertToPointFromString(getValueByKey(&subClipDataElement,"pos"));
                        //subClip->position.setX(-subClip->clip->clipRect.width() * anchor.x());
                        //subClip->position.setY(-subClip->clip->clipRect.height() * (1 - anchor.y()));
                        //flip
                        subClip->intFlipType = getValueByKey(&subClipDataElement,"filp").toInt();
                        //scale
                        subClip->scale = getValueByKey(&subClipDataElement,"scale").toFloat();
                        //rotate
                        subClip->rotate = getValueByKey(&subClipDataElement,"rotate").toFloat();
                        //anchor
                        if(getValueByKey(&subClipDataElement,"anchor") != NULL) {
                            subClip->anchor = convertToPointFFromString(getValueByKey(&subClipDataElement,"anchor"));
                            //subClip->anchor = QPointF(0,1.0);
                        }
                        mapSprite->addMapSpriteSubClip(subClip);
                    }
                    //生成图片
                    mapSprite->setImage((spriteData->spriteImage));
//                    mapSprite->image = ;
                }
                spriteData->addMapSprite(mapSprite);
            }
        }
        return spriteData;
    }
    return NULL;
}

void IOData::do_SaveSprite(SpriteData *spriteData)
{
    //打开文件保存框
    if(strSpriteFilePath.isEmpty()) {
        strSpriteFilePath = QFileDialog::getSaveFileName(NULL,QObject::tr("保存sprite文件"),spritePath,QObject::tr("sprite文件 (*.sprite *.plist *.xml)") );
        if(strSpriteFilePath.isEmpty())
            return;
    }
    saveSpriteFile(spriteData);
}

void IOData::do_SaveSprite_As(SpriteData *spriteData)
{
    QString oldPath = strSpriteFilePath;
    //打开文件保存框
    strSpriteFilePath = QFileDialog::getSaveFileName(NULL,QObject::tr("保存sprite文件"),spritePath,QObject::tr("sprite文件 (*.sprite *.plist *.xml)") );
    if(strSpriteFilePath.isEmpty())
        return;
    saveSpriteFile(spriteData);
    strSpriteFilePath = oldPath;
}

void IOData::saveSpriteFile(SpriteData *spriteData)
{
//    qDebug()<<strSpriteFilePath;
//    qDebug("filePath = %s",strSpriteFilePath);
    QFile file(strSpriteFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    //保存文件
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("dict");
    doc.appendChild(root);

//    QDomText text;
    //文件名
    createKeyElement(&doc,&root,("spriteFileName"));
    createValueElement(&doc,&root,("string"),spriteData->spriteName_);

    //图片路径
    createKeyElement(&doc,&root,("imagePath"));
    createValueElement(&doc,&root,("string"),spriteData->imagePath_);

    //切片数据
    createKeyElement(&doc,&root,("clipData"));
    //所有切片数据的dict
    QDomElement value_clipData_dict = doc.createElement("dict");
    for(int i = 0; i < spriteData->getClipList().size(); i++) {
        Clip* clip = spriteData->getClipList()[i];
        //每个切片数据的key
        createKeyElement(&doc,&value_clipData_dict,clip->clipName);
        //每个切片数据的value-rect
        createValueElement(&doc,&value_clipData_dict,("string"),convertToStringFromRect(clip->clipRect));
    }
    root.appendChild(value_clipData_dict);

    //地图精灵
    createKeyElement(&doc,&root,("mapSpriteData"));
    //所有地图精灵数据的dict
    QDomElement value_mapSpriteData_dict = doc.createElement("dict");
    for(int i = 0; i < spriteData->getMapSpriteList().size(); i++) {
        MapSprite* mapSprite = spriteData->getMapSpriteList()[i];
        //每个地图精灵数据的key
        createKeyElement(&doc,&value_mapSpriteData_dict,mapSprite->mapSpriteName);
        //每个地图精灵数据的值-dict
        QDomElement value_mapSpriteData = doc.createElement("dict");
        //生成子切片数据
        for(int j = 0; j < mapSprite->getSubClipList().size(); j++) {
            MapSpriteSubClip* subClip = mapSprite->getSubClipList()[j];
            //key
            createKeyElement(&doc,&value_mapSpriteData,subClip->clip->clipName);
            //value
            QDomElement value_mapSpriteSubClipData = doc.createElement("dict");

            createKeyElement(&doc,&value_mapSpriteSubClipData,("pos"));
            createValueElement(&doc,&value_mapSpriteSubClipData,("string"),convertToStringFromPoint(subClip->position));
            createKeyElement(&doc,&value_mapSpriteSubClipData,("filp"));
            createValueElement(&doc,&value_mapSpriteSubClipData,("integer"),convertToStringFromInt(subClip->intFlipType));
            createKeyElement(&doc,&value_mapSpriteSubClipData,("scale"));
            createValueElement(&doc,&value_mapSpriteSubClipData,("string"),convertToStringFromFloat(subClip->scale));
            createKeyElement(&doc,&value_mapSpriteSubClipData,("rotate"));
            createValueElement(&doc,&value_mapSpriteSubClipData,("string"),convertToStringFromFloat(subClip->rotate));
            createKeyElement(&doc,&value_mapSpriteSubClipData,("anchor"));
            createValueElement(&doc,&value_mapSpriteSubClipData,("string"),convertToStringFromPoint(subClip->anchor));
            value_mapSpriteData.appendChild(value_mapSpriteSubClipData);
        }
        //添加到所有地图精灵数据dict中
        value_mapSpriteData_dict.appendChild(value_mapSpriteData);
    }
    root.appendChild(value_mapSpriteData_dict);

    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4);
    file.close();
}


void IOData::readSpriteConfigFile(SpriteData* spriteData)
{
    QDomDocument doc;
    QString fileName = spriteConfigPath + spriteData->spriteName_ + ".config";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();

    QDomNodeList spritePosList = root.childNodes();
    for(int i = 0; i < spritePosList.size(); i++) {
        QDomNode n = spritePosList.at(i);
        if(n.toElement().tagName() == "key") {
            QString name = n.toElement().text();
            QPoint pos = convertToPointFromString(getValueByKey(&root,name));
            for(int j = 0; j < spriteData->getMapSpriteList().size(); j++) {
                MapSprite* mapSprite = spriteData->getMapSpriteList().at(j);
                if(mapSprite->mapSpriteName == name) {
                    mapSprite->choosedSpritePos = pos;
                    break;
                }
            }
        }
    }
}

void IOData::saveSpriteConfigFile(SpriteData *spriteData)
{
    QString fileName = spriteConfigPath + spriteData->spriteName_ + ".config";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    //保存文件
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("dict");
    doc.appendChild(root);

    for(int i = 0; i < spriteData->getMapSpriteList().size(); i++) {
        MapSprite* mapSprite = spriteData->getMapSpriteList().at(i);
        createKeyElement(&doc,&root,mapSprite->mapSpriteName);
        createValueElement(&doc,&root,("string"),convertToStringFromPoint(mapSprite->choosedSpritePos));
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4);
    file.close();
}

void IOData::createKeyElement(QDomDocument *doc,QDomElement *parentElement,QString keyName)
{
    createValueElement(doc,parentElement,("key"),keyName);
}

void IOData::createValueElement(QDomDocument *doc,QDomElement *parentElement,QString valueType, QString valueName)
{
    QDomElement value = doc->createElement(valueType);
    QDomText text = doc->createTextNode(valueName);
    value.appendChild(text);

    parentElement->appendChild(value);
}

QString IOData::getValueByKey(QDomElement *element, QString keyName)
{
    QDomElement valueElement = getValueElementByKey(element,keyName);
    if(!valueElement.isNull()) {
        return valueElement.text();
    }
    return NULL;
//    QDomNodeList nodeList = element->childNodes();
//    for(int i = 0; i < nodeList.size();i++) {
//        QDomNode n = nodeList.at(i);
//        if(n.toElement().tagName() == "key" && n.toElement().text() == keyName) {
//            QDomElement nextElement = n.nextSiblingElement();
//            if(!valueElement.isNull()) {
//                return nextElement.text();
//            }
//            return NULL;
//        }
//    }
//    return NULL;
}

QDomElement IOData::getValueElementByKey(QDomElement *element, QString keyName)
{
    QDomNodeList nodeList = element->childNodes();
    for(int i = 0; i < nodeList.size();i++) {
        QDomNode n = nodeList.at(i);
        if(n.toElement().tagName() == "key" && n.toElement().text() == keyName) {
            QDomElement nextElement = n.nextSiblingElement();
            return nextElement;
        }
    }
    return QDomElement();
}

void IOData::do_switchMap(QString mapFilePath)
{
    strMapFilePath = mapFilePath;
    qDebug()<<strMapFilePath;
}

void IOData::do_newMapFile()
{
    strMapFilePath = "";
}

void IOData::do_SaveMap(MapData *mapData)
{
    //打开文件保存框
    if(strMapFilePath.isEmpty()) {
        strMapFilePath = QFileDialog::getSaveFileName(NULL,QObject::tr("保存map文件"),mapPath,QObject::tr("map文件 (*.map *.plist *.xml)") );
        if(strMapFilePath.isEmpty())
            return;
    }

    mapData->mapFilePath = strMapFilePath;
    saveMapFile(mapData);
}

void IOData::do_SaveMap_As(MapData *mapData)
{
    QString oldPath = strMapFilePath;
    //打开文件保存框
    strMapFilePath = QFileDialog::getSaveFileName(NULL,QObject::tr("保存map文件"),mapPath,QObject::tr("map文件 (*.map *.plist *.xml)") );
    if(strMapFilePath.isEmpty())
        return;
    saveMapFile(mapData);
    strMapFilePath = oldPath;
}

MapData * IOData::do_ReadMap(QString fileName)
{
    QDomDocument doc;
    strMapFilePath = fileName;
    if(!strMapFilePath.isEmpty()) {
        QFile file(strMapFilePath);
        if (!file.open(QIODevice::ReadOnly))
            return NULL;
        if (!doc.setContent(&file)) {
            file.close();
            return NULL;
        }
        file.close();
        QDomElement root = doc.documentElement();

        spriteFileNameListInMap.clear();
        MapData* mapData = new MapData();
        QDomElement mapInfo = getValueElementByKey(&root,"mapInfo");

        mapData->mapName = getValueByKey(&mapInfo,"mapName");
        mapData->mapSize = convertToSizeFromString(getValueByKey(&mapInfo,"mapSize"));
        QRect rect = convertToRectFromString(getValueByKey(&mapInfo,"gridInfo"));
        mapData->intGridW = rect.x();
        mapData->intGridH = rect.y();
        mapData->intGridWNum = rect.width();
        mapData->intGridHNum = rect.height();
        if(getValueByKey(&mapInfo,"backgroundColor") != NULL) {
            rect = convertToRectFromString(getValueByKey(&mapInfo,"backgroundColor"));
            mapData->backGroundColor.setRgb(rect.x(),rect.y(),rect.width(),rect.height());
        }

        //路线数据
        QDomElement walkWay = getValueElementByKey(&root,"walkWay");
        if(!walkWay.isNull()) {
            //取出所有节点
            QDomNodeList walkWayNodeList = walkWay.childNodes();
            for(int i = 0; i < walkWayNodeList.size(); i++) {
                QDomNode aWalkWayNodeNode = walkWayNodeList.at(i);
                QDomElement aWalkWayElement = aWalkWayNodeNode.toElement();
                if(aWalkWayElement.tagName() == "key") {
                    MapObjectData* mapObjectData = new MapObjectData;
                    QString key = aWalkWayElement.text();
                    //取出一个节点的数据
                    QDomElement aWalkWayObjectElement = getValueElementByKey(&walkWay,key);

                    mapObjectData->curPoint = convertToPointFromString(getValueByKey(&aWalkWayObjectElement,"curPoint"));
                    mapObjectData->eventId = (getValueByKey(&aWalkWayObjectElement,"eventId")).toInt();
                    //z
                    if(getValueByKey(&aWalkWayObjectElement,"eventId") != NULL) {
                        mapObjectData->z = (getValueByKey(&aWalkWayObjectElement,"z")).toInt();
                        mapObjectData->tag = (getValueByKey(&aWalkWayObjectElement,"tag")).toInt();
                    }
                    //取出一个节点下一步的数据
                    QDomElement aWalkWayObjectNextPointElement = getValueElementByKey(&aWalkWayObjectElement,"nextPoint");
                    for(int j = 0; j < aWalkWayObjectNextPointElement.childNodes().size(); j++) {
                        QPoint nextPoint = convertToPointFromString(aWalkWayObjectNextPointElement.childNodes().at(j).toElement().text());
                        mapObjectData->nextPointList.append(nextPoint);

                    }

                    mapData->addMapObjectData(mapObjectData);
                }
            }
        }




        QDomElement mapLayersDataElement = getValueElementByKey(&root,"mapLayers");
        QDomNodeList mapLayersDataNodeList = mapLayersDataElement.childNodes();
        //填从mapSpritelist
        for(int i = 0; i < mapLayersDataNodeList.size(); i++) {
            QDomNode n = mapLayersDataNodeList.at(i);
            if(n.toElement().tagName() == "key") {
                MapLayer* mapLayer = new MapLayer;
                //图层名字
                mapLayer->mapLayerName = n.toElement().text();
//                qDebug()<<mapLayer->mapLayerName;
                QDomElement aMapLayerDataElement = getValueElementByKey(&mapLayersDataElement,mapLayer->mapLayerName);
                //每个图层的数据
                QDomNodeList aMapLayerDataNodeList = aMapLayerDataElement.childNodes();
                for(int j = 0; j < aMapLayerDataNodeList.size();j++) {
                    QDomNode aMapLayerNode = aMapLayerDataNodeList.at(j);
                    QDomElement aMapLayerElement = aMapLayerNode.toElement();
//                    qDebug("j = %d",j);
//                    qDebug()<<aMapLayerElement.tagName();
                    if(aMapLayerElement.tagName() == "key") {
                        MapLayerData* mapLayerData = new MapLayerData;
                        QString name = QString("mapSprite%1").arg(j/2);
                        QDomElement curMapLayerDataElement = getValueElementByKey(&aMapLayerDataElement,name);
//                        qDebug()<<("key: "+name);
                        //精灵名
                        mapLayerData->spriteName = getValueByKey(&curMapLayerDataElement,"spriteName");
                        addSpriteFileNameInMap(mapLayerData->spriteName);
                        //地图精灵名
                        mapLayerData->mapSpriteName = getValueByKey(&curMapLayerDataElement,"mapSpriteName");
                        //位置
                        mapLayerData->pos = convertToPointFromString(getValueByKey(&curMapLayerDataElement,"pos"));
                        //层
                        mapLayerData->z = (getValueByKey(&curMapLayerDataElement,"z")).toInt();
                        //tag
                        mapLayerData->tag = (getValueByKey(&curMapLayerDataElement,"tag")).toInt();
                        if(mapLayer->intMaxTag <= mapLayerData->tag) {
                            mapLayer->intMaxTag = mapLayerData->tag+1;
                        }
//                        qDebug()<<("=============");
//                        qDebug()<<mapLayerData->spriteName;
//                        qDebug()<<mapLayerData->mapSpriteName;
//                        qDebug("pos x = %d,y = %d",mapLayerData->pos.x(),mapLayerData->pos.y());
//                        qDebug("z = %d,tag = %d",mapLayerData->z,mapLayerData->tag);
                        mapLayer->addMapLayerData(mapLayerData);
                    }
                }
                mapData->addMapLayer(mapLayer);
            }
        }

//        changeZtoTag(mapData);

        return mapData;
    }
    return NULL;
}

//修改数据结构 修改z --- tag
void IOData::changeZtoTag(MapData* mapData)
{
    for(int i = 0; i < mapData->mapObjectList.size();i++) {
        MapObjectData* mapObjectData = mapData->mapObjectList.at(i);
        mapObjectData->tag = getTagByZ(mapData,mapObjectData->z);
    }
}
int IOData::getTagByZ(MapData* mapData,int z)
{
    //将事件数据的z转换为图标的tag
    MapLayer* mapLayer = mapData->layerList.at(2);
    for(int i = 0; i < mapLayer->layerDataList.size(); i++) {
        MapLayerData* mapLayerData = mapLayer->layerDataList.at(i);
        if(mapLayerData->z == z) {
            return mapLayerData->tag;
        }
    }
    return -1;
}

void IOData::saveMapFile(MapData *mapData)
{
    QFile file(strMapFilePath);
    //qDebug() << "mapPath " << strMapFilePath;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    //保存文件
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("dict");
    doc.appendChild(root);

    //key - 地图信息
    createKeyElement(&doc,&root,("mapInfo"));
    //value - 地图信息
    QDomElement value_mapInfo_dict = doc.createElement("dict");
    //地图名
    createKeyElement(&doc,&value_mapInfo_dict,("mapName"));
    createValueElement(&doc,&value_mapInfo_dict,("string"),mapData->mapName);
    //地图大小
    createKeyElement(&doc,&value_mapInfo_dict,("mapSize"));
    createValueElement(&doc,&value_mapInfo_dict,("string"),converToStringFromSize(mapData->mapSize));
    //地图底色
    createKeyElement(&doc,&value_mapInfo_dict,("backgroundColor"));
    QColor color = mapData->backGroundColor;
    QString background = QString("{%1,%2,%3,%4}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    createValueElement(&doc,&value_mapInfo_dict,("string"),background);
    //格子相关信息
    createKeyElement(&doc,&value_mapInfo_dict,("gridInfo"));
    QRect rect = QRect(mapData->intGridW,mapData->intGridH,mapData->intGridWNum,mapData->intGridHNum);
    createValueElement(&doc,&value_mapInfo_dict,("string"),convertToStringFromRect(rect));
    //图层名字(保存顺序)
    createKeyElement(&doc,&value_mapInfo_dict,("layerNameArray"));
    QString layerNameArray;
    for(int i = 0; i < mapData->layerList.size(); i++) {
        MapLayer* mapLayer = mapData->layerList.at(i);
        layerNameArray += mapLayer->mapLayerName;
        if(i != mapData->layerList.size() - 1) {
            layerNameArray += ",";
        }
    }
    createValueElement(&doc,&value_mapInfo_dict,("string"),layerNameArray);
    root.appendChild(value_mapInfo_dict);

    //kye - 路线信息
    createKeyElement(&doc,&root,("walkWay"));
    //value - 路线信息
    QDomElement value_mapWalkWay_dict = doc.createElement("dict");
    for(int i = 0; i < mapData->mapObjectList.size(); i++) {
        MapObjectData* mapObjectData = mapData->mapObjectList.at(i);
        //添加一个路线节点
        int key = mapObjectData->curPoint.y() * mapData->intGridWNum + mapObjectData->curPoint.x();
        createKeyElement(&doc,&value_mapWalkWay_dict,QString("%1").arg(key));
        QDomElement value_mapWalkWay_Object_dict = doc.createElement("dict");
        //添加一个路线节点的具体数据
        //curPoint
        createKeyElement(&doc,&value_mapWalkWay_Object_dict,"curPoint");
        createValueElement(&doc,&value_mapWalkWay_Object_dict,("string"),convertToStringFromPoint(mapObjectData->curPoint));
        //eventid
        createKeyElement(&doc,&value_mapWalkWay_Object_dict,"eventId");
        createValueElement(&doc,&value_mapWalkWay_Object_dict,("string"),convertToStringFromInt(mapObjectData->eventId));
        //z
        createKeyElement(&doc,&value_mapWalkWay_Object_dict,"z");
        createValueElement(&doc,&value_mapWalkWay_Object_dict,("string"),convertToStringFromInt(mapObjectData->z));
        //tag
        createKeyElement(&doc,&value_mapWalkWay_Object_dict,"tag");
        createValueElement(&doc,&value_mapWalkWay_Object_dict,("string"),convertToStringFromInt(mapObjectData->tag));
        //添加多个下一步
        createKeyElement(&doc,&value_mapWalkWay_Object_dict,"nextPoint");
        QDomElement value_mapWalkWay_Object_NextPoint_dict = doc.createElement("array");
        for(int j = 0; j < mapObjectData->nextPointList.size(); j++) {
            createValueElement(&doc,&value_mapWalkWay_Object_NextPoint_dict,("string"),convertToStringFromPoint(mapObjectData->nextPointList.at(j)));
        }
        value_mapWalkWay_Object_dict.appendChild(value_mapWalkWay_Object_NextPoint_dict);

        value_mapWalkWay_dict.appendChild(value_mapWalkWay_Object_dict);
    }
    root.appendChild(value_mapWalkWay_dict);



    //key - 地图图层
    createKeyElement(&doc,&root,("mapLayers"));
    //value - 地图图层
    QDomElement value_mapLayers_dict = doc.createElement("dict");
//    qDebug("图层sizie = %d",mapData->layerList.size());
    for(int i = 0; i < mapData->layerList.size(); i++) {
        MapLayer* mapLayer = mapData->layerList.at(i);
        //key - 图层名字
//        QString mapLayerName = QString(mapLayer->mapLayerName+"_%1").arg(i);
        createKeyElement(&doc,&value_mapLayers_dict,mapLayer->mapLayerName);
        //value - 具体地图图层数据
        QDomElement value_curLayer_dict = doc.createElement("dict");

        //具体图层下的数据
        for(int j = 0 ; j < mapLayer->layerDataList.size(); j++) {
            MapLayerData* mapLayerData = mapLayer->layerDataList.at(j);

            //key
            QString name = QString("mapSprite%1").arg(j);
            createKeyElement(&doc,&value_curLayer_dict,name);
            //value
            QDomElement value_mapLayerData_dict = doc.createElement("dict");

            //精灵名
            createKeyElement(&doc,&value_mapLayerData_dict,("spriteName"));
            createValueElement(&doc,&value_mapLayerData_dict,("string"),mapLayerData->spriteName);
            //地图精灵名
            createKeyElement(&doc,&value_mapLayerData_dict,("mapSpriteName"));
            createValueElement(&doc,&value_mapLayerData_dict,("string"),mapLayerData->mapSpriteName);
            //位置
            createKeyElement(&doc,&value_mapLayerData_dict,("pos"));
            createValueElement(&doc,&value_mapLayerData_dict,("string"),convertToStringFromPoint(mapLayerData->pos) );
            //层
            createKeyElement(&doc,&value_mapLayerData_dict,("z"));
            createValueElement(&doc,&value_mapLayerData_dict,("integer"),convertToStringFromInt(mapLayerData->z) );
            //tag
            createKeyElement(&doc,&value_mapLayerData_dict,("tag"));
            createValueElement(&doc,&value_mapLayerData_dict,("integer"),convertToStringFromInt(mapLayerData->tag) );

            value_curLayer_dict.appendChild(value_mapLayerData_dict);
        }

        value_mapLayers_dict.appendChild(value_curLayer_dict);
    }
    root.appendChild(value_mapLayers_dict);

    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4);
    file.close();
}

//添加地图所需的sprite
void IOData::addSpriteFileNameInMap(QString spriteName)
{
//    bool isRepeat = false;
    //转绝对路径
    QString QPath = spritePath + spriteName + ".sprite";
    // new add by phileas.chen
    if(QFile(QPath).exists() == false)
    {
        return;
    }
    // new end
    QDir temDir(QPath);
    QString absDir = temDir.absolutePath();

    for(int i = 0; i < spriteFileNameListInMap.size(); i++) {
        if(absDir == spriteFileNameListInMap.at(i)) {
            return;
        }
    }


    spriteFileNameListInMap.append(absDir);
}

QStringList IOData::getSpriteFileNameListInMap()
{
    return spriteFileNameListInMap;
}

//读取事件
void IOData::readEvent(QList<EventData*>* eventDataList)
{
    eventDataList->clear();

    QString eventFilePath = eventPath + "event";
    QDomDocument doc;
    if(!eventFilePath.isEmpty()) {
        QFile file(eventFilePath);
        if (!file.open(QIODevice::ReadOnly))
            return ;
        if (!doc.setContent(&file)) {
            file.close();
            return ;
        }
        file.close();
        QDomElement root = doc.documentElement();

        //事件数据
        QDomNodeList eventList = root.childNodes();
        for(int i = 0; i < eventList.size(); i++) {
            QDomNode eventNode = eventList.at(i);
            QDomElement eventElement = eventNode.toElement();
            if(eventElement.tagName() == "key") {
                EventData* eventData = new EventData;
                QString key = eventElement.text();
                eventData->id = key.toInt();
                QDomElement eventDataElement = getValueElementByKey(&root,key);

                eventData->event_type = getValueByKey(&eventDataElement,"event_type").toInt();
                eventData->event_content = getValueByKey(&eventDataElement,"content");

                eventDataList->append(eventData);
            }
        }
    }
}





