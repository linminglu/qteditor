/********************************************************************
    created:    2011/08/01
    filename:   IOData.h
    author:     bqlf
    purpose:    编辑器的读写数据
*********************************************************************/
#ifndef IODATA_H
#define IODATA_H

#include "Data/mapdata.h"
#include "Data/spritedata.h"
#include "Data/eventdata.h"
#include <QObject>
#include <QtXml>
#include <QStringList>
#include <QList>
class IOData
{
//    Q_OBJECT

public:
//    IOData();
//    ~IOData();
    static IOData* getInstance();

    //读写精灵文件
    SpriteData* do_ReadSprite(QString fileName,bool import);
    void do_SaveSprite(SpriteData *spriteData);
    void do_SaveSprite_As(SpriteData *spriteData);
    void do_newSpriteFile();

    //读写地图文件
    void do_switchMap(QString mapFilePath);
    void do_newMapFile();
    void do_SaveMap(MapData *mapData);
    void do_SaveMap_As(MapData *mapData);
    MapData* do_ReadMap(QString fileName);

    //读写选择精灵的位置
    void readSpriteConfigFile(SpriteData* spriteData);
    void saveSpriteConfigFile(SpriteData* spriteData);

    //读写事件
    void readEvent(QList<EventData*>* eventDataList);

    //添加地图所需的sprite
    void addSpriteFileNameInMap(QString spriteName);
    QStringList getSpriteFileNameListInMap();
protected:
    IOData();
    ~IOData();
private:
    void saveSpriteFile(SpriteData* spriteData);
    void saveMapFile(MapData* mapData);
    void createKeyElement(QDomDocument *doc,QDomElement *parentElement,QString keyName);
    void createValueElement(QDomDocument *doc,QDomElement *parentElement,QString valueType,QString valueName);
    QString getValueByKey(QDomElement *element,QString keyName);
    QDomElement getValueElementByKey(QDomElement *element,QString keyName);

    //修改数据结构 修改z --- tag
    void changeZtoTag(MapData* mapData);
    int getTagByZ(MapData* mapData,int z);

    static IOData* ioData_;
//    bool isSpriteModify;//spriteData数据修改,这个标签打开,保存按钮可用
    QString strSpriteFilePath;//sprite文件路径
    QString strMapFilePath;//map文件路径
    QStringList spriteFileNameListInMap;
};

#endif // IODATA_H
