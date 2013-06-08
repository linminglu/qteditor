#include <QtWidgets>

#include "mapwindow.h"
#include "ui_mapwindow.h"
#include "maptab.h"
#include "iodata.h"
#include "choosemapspriteview.h"
#include <QFileDialog>
#include <QTreeWidgetItem>
#include "utility.h"
#include "config.h"

using namespace Utility;

MapWindow::MapWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapWindow)
{
    ui->setupUi(this);
    uniteDockWidget();
    intEditMode = 0;
    curStMapSprite.mapSpriteName = "";
    curMapData = NULL;
    curChoosedMapSpriteItem = NULL;
    curChoosedMapLayerData = NULL;

    //设置地图splitter比列
    //    ui->splitter_clip->setStretchFactor(0, 3);
    //    ui->splitter_clip->setStretchFactor(1, 1);


    imgUp.load(":/new/prefix1/images/1.png");
    imgDown.load(":/new/prefix1/images/2.png");
    imgLeft.load(":/new/prefix1/images/3.png");
    imgRight.load(":/new/prefix1/images/4.png");
    imgShoe.load(":/new/prefix1/images/shoe.png");

    //读取事件
    eventList = new QList<EventData*>;

    //    ui->tWLayers->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    ui->tWLayers->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

MapWindow::~MapWindow()
{
    delete ui;
    delete curMapData;
    curMapData = NULL;

    for(int i = 0; i < eventList->size(); i++) {
        EventData* eventData = eventList->at(i);
        delete eventData;
        eventData = NULL;
    }
    delete eventList;
    eventList = NULL;
}

void MapWindow::uniteDockWidget()
{
    ui->dwHistory->setVisible(isShowHistory);
    ui->dwLayers->setVisible(isShowLayer);
    ui->dwSprites->setVisible(isShowSprite);
    ui->dwEvent->setVisible(isShowEvent);
    if(isShowEvent) {
        this->tabifyDockWidget(ui->dwLayers,ui->dwEvent);
    }

    //    this->tabifyDockWidget(ui->dwHistory,ui->dwSprites);
}

void MapWindow::newMap()
{
    MapTab *mapTab = new MapTab(this);
    ui->tabMap->addTab(mapTab,"newMap");
    mapTabList.append(mapTab);
    curMap = mapTab;

    //    if(curMapData != NULL) {
    //        delete curMapData;
    //    }

    MapData* mapData = new MapData;
    mapDataList.append(mapData);
    curMapData = mapData;

    //    setCurMapTabData(mapDataList.size() - 1);
    //    curMapData = new MapData();

    setMapName(curMapName);
    setMapSize(QRect(0,0,intCurGridW*intCurGridWNum,intCurGridH*intCurGridHNum));

}

//导入一个sprite文件;
void MapWindow::importSpriteFile(QString fileName)
{
    //    QString fileName = QFileDialog::getOpenFileName(this,QObject::tr("打开sprite文件"),spritePath,QObject::tr("sprite文件 (*.sprite *.plist *.xml)") );
    if(!fileName.isEmpty()) {
        //读取spritedate
        IOData* io = IOData::getInstance();

        SpriteData* spriteData = io->do_ReadSprite(fileName,true);
        //添加到mapwindown中
        addSpriteData(spriteData);

        //        if(!isRepeat) {
        //            //生成选择精灵view
        //            ChooseMapSpriteView* mapSpriteView = new ChooseMapSpriteView(spriteData);
        //            mapSpriteView->setMapWindow(this);
        //            //添加view
        //            ui->tabWMapSprite->addTab(mapSpriteView,spriteData->spriteName_);
        //        }
    }
    //    ui->dwSprites->setMinimumWidth(400);
}

//设置地图大小
void MapWindow::setMapSize(QRect rect)
{
    //    curMapData->mapSize = QSize(rect.width(),rect.height());
    curMapData->intGridW = intCurGridW;
    curMapData->intGridH = intCurGridH;
    curMapData->intGridWNum = intCurGridWNum;
    curMapData->intGridHNum = intCurGridHNum;

    curMap->mapEditView->setSceneRect(rect.x(),rect.y(),rect.width(),rect.height());
    curMap->mapEditView->scene()->update();
    //    qDebug("rect x = %d, y = %d, w = %d, h = %d",rect.x(),rect.y(),rect.width(),rect.height());
}
//设置名字
void MapWindow::setMapName(QString mapName)
{
    curMapData->mapName = mapName;
}

//设置网格
void MapWindow::setShowGrid(bool )
{
    //    isShowGrid = showGrid;
    curMap->mapEditView->scene()->update();
}
bool MapWindow::getShowGrid()
{
    return isShowGrid;
}

//设置吸附
void MapWindow::setPasteGrid(bool )
{
    curMap->mapEditView->scene()->update();
}

bool MapWindow::getPasteGrid()
{
    return isPasteGrid;
}
static int scalex=1;
static  qreal scaley=1.0;

void MapWindow::toScaleViewAdd()
{
//    if(scalex>1.0+0.00000001)
//    {
//        scalex+=0.1;
//    }
 //   scalex+=0.1;
 //   scaley+=0.1;
    scalex++;
   curMap->mapEditView->scale(1.2,1.2);
}
void MapWindow::toScaleViewSub()
{
 //   scalex-=0.1;
//    scaley-=0.1;
    scalex--;
    curMap->mapEditView->scale(1/1.2,1/1.2);
}
#include <math.h>
void MapWindow::toScaleHuanyuan()
{
    if(scalex>1)
    {
          curMap->mapEditView->scale(1/pow(1.2,scalex-1),1/pow(1.2,scalex-1));
    }
    else if(scalex<1)
    {
         curMap->mapEditView->scale(pow(1.2,fabs(scalex)+1),pow(1.2,fabs(scalex)+1));
    }

    scalex=1;
}

//设置当前mapSpriteitem
void MapWindow::setCurMapSpriteItem(QString mapSpriteName) {

    //当前选择精灵view
    ChooseMapSpriteView* curChooseMapSpriteView = (ChooseMapSpriteView*)ui->tabWMapSprite->currentWidget();
    //名字不一样就生成新的
    if(curStMapSprite.spriteName != curChooseMapSpriteView->getSpriteName() || curStMapSprite.mapSpriteName != mapSpriteName) {

        curStMapSprite.mapSpriteName = mapSpriteName;
        //当前选择精灵view
        curStMapSprite.spriteName = curChooseMapSpriteView->getSpriteName();
        //图像名字
        curStMapSprite.imageName = getMapSpriteImageName();
        //当前选择的
        curMapSprite = getMapSprite();
        //        MapSprite* mapSprite = getMapSprite();
        //        QString imageName = getImageName();
        //        curMapSpriteItem = new MapSpriteItem(mapSprite,imageName);

    }
    //    curMapSprite.mapSpriteName = mapSpriteName;
    //    //当前选择精灵view
    //    ChooseMapSpriteView* curChooseMapSpriteView = (ChooseMapSpriteView*)ui->tabWMapSprite->currentWidget();
    //    curMapSprite.spriteName = curChooseMapSpriteView->getSpriteName();
    //    //图像名字
    //    curMapSprite.imageName = getMapSpriteImageName();
}

bool MapWindow::addSpriteData(SpriteData *spriteData)
{
    //先判断是否重复
    bool isRepeat = false;
    int intRepeatIndex = -1;
    for(int i = 0; i < spriteDataList.size(); i++) {
        if(spriteData->spriteName_ == spriteDataList.at(i)->spriteName_) {
            isRepeat = true;
            intRepeatIndex = i;
            break;
        }
    }

    //添加
    //生成选择精灵view
    ChooseMapSpriteView* mapSpriteView = new ChooseMapSpriteView(spriteData);
    mapSpriteView->setMapWindow(this);
    if(isRepeat) {
        //删除tab
        QString tabName;
        int index;
        for(index = 0; index < ui->tabWMapSprite->count();index++) {
            tabName = ui->tabWMapSprite->tabText(index);
            if(tabName == spriteData->spriteName_) {
                ui->tabWMapSprite->removeTab(index);
                break;
            }
        }
        //在原地方插入
        ui->tabWMapSprite->insertTab(index,mapSpriteView,spriteData->spriteName_);


        //先删除再填充
        delete spriteDataList.at(intRepeatIndex);
        spriteDataList.removeAt(intRepeatIndex);
    }
    else {
        //添加view
        ui->tabWMapSprite->addTab(mapSpriteView,spriteData->spriteName_);
    }
    spriteDataList.append(spriteData);
    return isRepeat;
}

MapSprite * MapWindow::getMapSprite()
{
    QString spriteName = curStMapSprite.spriteName;
    QString mapSpriteName = curStMapSprite.mapSpriteName;
    //寻找精灵数据
    return getMapSpriteByName(spriteName,mapSpriteName);
    //    for(int i = 0; i < spriteDataList.size(); i++) {
    //        SpriteData* spriteData = spriteDataList.at(i);
    //        if(spriteData->spriteName_ == spriteName) {
    //            //寻找地图精灵数据
    //            for(int j = 0; j < spriteData->getMapSpriteList().size(); j++) {
    //                MapSprite* mapSprite = spriteData->getMapSpriteList().at(j);
    //                if(mapSprite->mapSpriteName == mapSpriteName) {
    //                    return mapSprite;
    //                }
    //            }
    //        }
    //    }
    //    return NULL;
}

MapSprite* MapWindow::getMapSpriteByName(QString spriteName,QString mapSpriteName)
{
    for(int i = 0; i < spriteDataList.size(); i++)
    {
        SpriteData* spriteData = spriteDataList.at(i);
        if(spriteData->spriteName_ == spriteName)
        {
            //寻找地图精灵数据
            for(int j = 0; j < spriteData->getMapSpriteList().size(); j++)
            {
                MapSprite* mapSprite = spriteData->getMapSpriteList().at(j);
                if(mapSprite->mapSpriteName == mapSpriteName)
                {
                    return mapSprite;
                }
            }
        }
    }
    return NULL;
}

QString MapWindow::getMapSpriteImageName()
{
    for(int i = 0; i < spriteDataList.size(); i++) {
        SpriteData* spriteData = spriteDataList.at(i);
        if(spriteData->spriteName_ == curStMapSprite.spriteName) {
            return spriteData->imagePath_;
        }
    }
    return NULL;
}

QImage MapWindow::getMapSpriteImage()
{
    for(int i = 0; i < spriteDataList.size(); i++) {
        SpriteData* spriteData = spriteDataList.at(i);
        for(int j = 0; j < spriteData->getMapSpriteList().size(); j++) {
            MapSprite* mapSprite = spriteData->getMapSpriteList().at(j);
            if(mapSprite->mapSpriteName == curStMapSprite.mapSpriteName) {
                return mapSprite->image;
            }
        }
    }
    return QImage();
}

QString MapWindow::getImageName()
{
    return curStMapSprite.imageName;
}

void MapWindow::setEditMode(int mode)
{
    intEditMode = mode;
    curMap->mapEditView->scene()->update();
    if(intEditMode == 3 && !isShowEvent) {
        isShowEvent = true;
        showEvent(isShowEvent);
    }
}

int MapWindow::getEditMode()
{
    //qDebug()<<"intEditMode"<<intEditMode;
    return intEditMode;
}

QString MapWindow::getMapSpriteName()
{
    return curStMapSprite.mapSpriteName;
}
//生成新的mapLayerData
void MapWindow::createMapLayerData(QPoint pos)
{
    MapLayerData* layerData = new MapLayerData();
    layerData->mapSpriteName = curStMapSprite.mapSpriteName;
    layerData->spriteName = curStMapSprite.spriteName;
    //网格吸附
    if(getPasteGrid()) {
        //        MapSprite *mapSprite = getMapSprite();
        //        //要将pos对应MapSprite anchor

        //        QPoint curPoint = QPoint(pos.x() - mapSprite->image.width()/2,pos.y()
        //                - mapSprite->image.height()/2);
        layerData->pos = getPasteGridCenterPoint(pos);
    }
    else
    {
        layerData->pos = pos;
    }

    //tag and z
    QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = currentTopLevelIndex(ui->tWLayers);
        //取得当前层的数据
        MapLayer* curLayer = curMapData->layerList.at(index);
        layerData->tag = curLayer->intMaxTag;
        curLayer->intMaxTag++;

        //        QTreeWidgetItem *curTopItem = ui->tWLayers->topLevelItem(index);
        //        int size = curTopItem->childCount();
        //        layerData->z = size * 5;
        layerData->z = layerData->tag * 5;
    }

    newMapLayerData = layerData;
    addMapSprite();
}

void MapWindow::on_btnAddLayer_clicked()
{
    addMapLayer();
}

void MapWindow::on_btnDelLayer_clicked()
{
    QTreeWidgetItem* curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = ui->tWLayers->indexOfTopLevelItem(curItem);
        if(index != -1) {
            delMapLayer();
        }
        else {
            delMapSprite();
        }
    }

}

void MapWindow::on_btnCopyLayer_clicked()
{
    QTreeWidgetItem* curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = ui->tWLayers->indexOfTopLevelItem(curItem);
        if(index != -1) {
            copyMapLayer();
        }
        else {
            copyMapSprite();
        }
    }

}

void MapWindow::on_btnUpLayer_clicked()
{
    QTreeWidgetItem* curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = ui->tWLayers->indexOfTopLevelItem(curItem);
        if(index != -1) {
            upMapLayer();
        }
        else {
            upMapSprite();
        }
    }

}

void MapWindow::on_btnDownLayer_clicked()
{
    QTreeWidgetItem* curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = ui->tWLayers->indexOfTopLevelItem(curItem);
        if(index != -1) {
            downMapLayer();
        }
        else {
            downMapSprite();
        }
    }
}
#include <QMessageBox>
void MapWindow::openMapFile(QString fileName)
{

    //新增数据
    IOData* io = IOData::getInstance();
    MapData *newMapData = io->do_ReadMap(fileName);
    newMapData->mapFilePath = fileName;
    QFileInfo finleInfo(fileName);

    //导入相关的sprite数据
    QStringList fileNameList = io->getSpriteFileNameListInMap();
    for(int i = 0; i < fileNameList.size(); i++) {
        //qDebug()<<"importSpriteFile "<<fileNameList.at(i);
        importSpriteFile(fileNameList.at(i));
    }
    //判断地图数据的精灵是否都存在
    bool isNotExistMapSprite = false;
    for(int i = 0; i < newMapData->layerList.size(); i++) {
        MapLayer* mapLayer = newMapData->layerList.at(i);
        for(int j = mapLayer->layerDataList.size() - 1; j >= 0; j--) {
            MapLayerData* mapLayerData = mapLayer->layerDataList.at(j);
            MapSprite* mapSprite = getMapSpriteByName(mapLayerData->spriteName,mapLayerData->mapSpriteName);
            if(mapSprite == NULL) {
                isNotExistMapSprite = true;
                mapLayer->delMapLayerData(mapLayerData);
            }
        }
    }
    if(isNotExistMapSprite) {
        QMessageBox message;
        message.setText(tr("该地图上的某些精灵已被删除！"));
        message.exec();
    }

    mapDataList.append(newMapData);

    //新增标签
    MapTab *mapTab = new MapTab(this);
    ui->tabMap->addTab(mapTab,finleInfo.baseName());
    mapTabList.append(mapTab);

    setCurMapTabData(mapDataList.size() - 1);
    setMapName(curMapName);
    setMapSize(QRect(0,0,intCurGridW*intCurGridWNum,intCurGridH*intCurGridHNum));


    //根据map数据构建场景
    createSceneByMapData(curMapData);

    //保存一份数据
    //    save_Memento_ClipData();
}

void MapWindow::saveMapFile()
{
    IOData* io = IOData::getInstance();
    //保存名字
    //    setMapLayerName();
    //检查图层名字
    for(int i = 0; i < curMapData->layerList.size(); i++) {
        QString layerName = curMapData->layerList.at(i)->mapLayerName;
        for(int j = i + 1; j < curMapData->layerList.size(); j++) {
            MapLayer* mapLayer = curMapData->layerList.at(j);
            if(layerName == mapLayer->mapLayerName) {
                QMessageBox mess;
                mess.setText(tr("图层有重名！修改后再保存"));
                mess.exec();
                return;
            }
        }
    }

    io->do_SaveMap(curMapData);
    //    setFileModify(false);
    QFileInfo fileInfo(curMapData->mapFilePath);
    ui->tabMap->setTabText(ui->tabMap->currentIndex(),fileInfo.baseName());
}

void MapWindow::saveMapFileAs()
{
    IOData* io = IOData::getInstance();
    io->do_SaveMap_As(curMapData);
    //    setFileModify(false);
}

//根据数据构建场景 打开地图时
void MapWindow::createSceneByMapData(MapData* mapData)
{

    for(int i = 0; i < mapData->layerList.size(); i++) {
        //添加层
        MapLayer* mapLayer = mapData->layerList.at(i);
        //        addMapLayerByName(mapLayer->mapLayerName);
        for(int j = 0; j < mapLayer->layerDataList.size(); j++) {
            MapLayerData* mapLayerData = mapLayer->layerDataList.at(j);
            //添加item到图层
            //            QTreeWidgetItem *curTopItem = ui->tWLayers->topLevelItem(i);
            //            QTreeWidgetItem *child = new QTreeWidgetItem(curTopItem);
            //            child->setCheckState(0,Qt::Checked);
            //            setTWLayerCurItem(child,mapLayerData);
            //添加item到场景
            addMapSpriteByMapLayerData(mapLayerData,i);
        }
    }
}

void MapWindow::addMapLayerByName(QString layerName)
{
    //添加图层界面
    //    QString layerName = tr("layerName");
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->tWLayers);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    item->setText(0,layerName);
    item->setCheckState(0,Qt::Checked);
}

void MapWindow::addMapSpriteByMapLayerData(MapLayerData* mapLayerData,int layer)
{
    MapSprite* mapSprite = getMapSpriteByName(mapLayerData->spriteName,mapLayerData->mapSpriteName);
    if(mapSprite == NULL) {
        qDebug() << "can't find mapSprite";
        return;
    }
    if(mapSprite->getSubClipList().size() <= 0) {
        qDebug() << "no mapSpriteSubClip";
        return;
    }
    //    QString imageName = getImageName();
    MapSpriteItem* spriteItem = new MapSpriteItem(mapSprite,"");
    //偏移量
    MapSpriteSubClip* subClip = mapSprite->getSubClipList().at(0);
    QPoint pos = QPoint(mapLayerData->pos.x() + subClip->position.x(),mapLayerData->pos.y() + subClip->position.y());
    spriteItem->setPos(pos);
    spriteItem->setLayer(layer);
    spriteItem->setTag(mapLayerData->tag);
    spriteItem->setZValue(convertZToScene(mapLayerData->z,layer));

    curMap->mapEditView->scene()->addItem(spriteItem);
}


void MapWindow::addMapLayer()
{
    //添加图层界面
    QString layerName = tr("newLayer");
    addMapLayerByName(layerName);
    //    QString layerName = tr("新图层");
    //    QTreeWidgetItem *item = new QTreeWidgetItem(ui->tWLayers);
    //    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    //    item->setText(0,layerName);
    //    item->setCheckState(0,Qt::Checked);

    //添加数据
    MapLayer* layer = new MapLayer();
    layer->mapLayerName = layerName;
    curMapData->addMapLayer(layer);

    int index = ui->tWLayers->topLevelItemCount() - 1;
    QTreeWidgetItem* item = ui->tWLayers->topLevelItem(index);
    ui->tWLayers->setCurrentItem(item);
}

void MapWindow::delMapLayer()
{
    //删除图层界面
    QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = currentTopLevelIndex(ui->tWLayers);
        //        qDebug("choose layer index = %d",index);
        ui->tWLayers->takeTopLevelItem(index);

        //删除地图上item
        MapLayer* layer = curMapData->layerList.at(index);
        for(int i = 0; i < layer->layerDataList.size(); i++) {
            MapLayerData* curLayerData = layer->layerDataList.at(i);
            MapSpriteItem* mapSpriteItem = currentMapSpriteItem(index,curLayerData);
            curMap->mapEditView->scene()->removeItem(mapSpriteItem);
        }

        //删除数据
        curMapData->delMapLayer(layer);

    }
}

void MapWindow::copyMapLayer()
{
    //添加界面
    QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = currentTopLevelIndex(ui->tWLayers);
        qDebug("copy layer index = %d",index);
        QTreeWidgetItem *topLevelItem = ui->tWLayers->topLevelItem(index);
        QTreeWidgetItem *item = topLevelItem->clone();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
        QString layerName = item->text(0);
        item->setText(0,layerName+=tr("_copy"));
        ui->tWLayers->addTopLevelItem(item);

        //添加精灵到地图中
        int intNewLayer = ui->tWLayers->topLevelItemCount() - 1;
        MapLayer* layer = curMapData->layerList.at(index);
        for(int j = 0; j < layer->layerDataList.size(); j++) {
            MapLayerData* layData = layer->layerDataList.at(j);
            MapSprite* mapSprite = getMapSpriteByName(layData->spriteName,layData->mapSpriteName);
            QString imageName = getImageName();
            MapSpriteItem* spriteItem = new MapSpriteItem(mapSprite,imageName);
            spriteItem->setPos(layData->pos);
            spriteItem->setLayer(intNewLayer);
            spriteItem->setTag(layData->tag);
            spriteItem->setZValue(convertZToScene(layData->z,intNewLayer));

            curMap->mapEditView->scene()->addItem(spriteItem);
        }
        //添加数据
        MapLayer* newLayer = layer->getMapLayerClone();
        //        for(int i = 0; i < newLayer->layerDataList.size(); i++) {
        //            MapLayerData* layData = newLayer->layerDataList.at(i);
        //            layData->z = layData->z + (intNewLayer - index)*10000;
        //        }
        curMapData->addMapLayer(newLayer);

    }

}

void MapWindow::upMapLayer()
{
    //添加界面
    QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = currentTopLevelIndex(ui->tWLayers);
        QTreeWidgetItem *topLevelItem = ui->tWLayers->topLevelItem(index);
        if(index > 0) {
            QTreeWidgetItem *item = topLevelItem->clone();
            //删除原来的
            ui->tWLayers->takeTopLevelItem(index);
            //插入
            ui->tWLayers->insertTopLevelItem(index - 1,item);
            ui->tWLayers->setCurrentItem(item);

            //修改scene item z
            QList<MapSpriteItem*> curLayerMapSpriteItemList;
            MapLayer* curlayer = curMapData->layerList.at(index);
            for(int i = 0; i < curlayer->layerDataList.size(); i++) {
                MapLayerData* layerData = curlayer->layerDataList.at(i);
                MapSpriteItem* mapSpriteItem = currentMapSpriteItem(index,layerData);
                mapSpriteItem->setZValue(convertZToScene(layerData->z,index - 1));
                curLayerMapSpriteItemList.append(mapSpriteItem);
            }
            QList<MapSpriteItem*> preLayerMapSpriteItemList;
            MapLayer* preLayer = curMapData->layerList.at(index - 1);
            for(int i = 0; i < preLayer->layerDataList.size(); i++) {
                MapLayerData* layerData = preLayer->layerDataList.at(i);
                MapSpriteItem* mapSpriteItem = currentMapSpriteItem(index - 1,layerData);
                mapSpriteItem->setZValue(convertZToScene(layerData->z,index));
                preLayerMapSpriteItemList.append(mapSpriteItem);
            }
            //修改item layer
            for(int i = 0; i < curLayerMapSpriteItemList.size(); i++) {
                MapSpriteItem* mapSpriteItem = curLayerMapSpriteItemList.at(i);
                mapSpriteItem->setLayer(index - 1);
            }
            for(int i = 0; i < preLayerMapSpriteItemList.size(); i++) {
                MapSpriteItem* mapSpriteItem = preLayerMapSpriteItemList.at(i);
                mapSpriteItem->setLayer(index);
            }

            //添加数据
            MapLayer* layer = curMapData->layerList.at(index);
            MapLayer* newLayer = layer->getMapLayerClone();
            curMapData->delMapLayer(layer);
            curMapData->insertMapLayer(newLayer,index - 1);
        }
    }
}

void MapWindow::downMapLayer()
{
    //添加界面
    QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = currentTopLevelIndex(ui->tWLayers);
        QTreeWidgetItem *topLevelItem = ui->tWLayers->topLevelItem(index);
        int size = ui->tWLayers->topLevelItemCount();
        if(index < size - 1) {
            QTreeWidgetItem *item = topLevelItem->clone();
            //删除原来的
            ui->tWLayers->takeTopLevelItem(index);
            //插入
            ui->tWLayers->insertTopLevelItem(index + 1,item);
            ui->tWLayers->setCurrentItem(item);

            //修改scene item z
            QList<MapSpriteItem*> curLayerMapSpriteItemList;
            MapLayer* curlayer = curMapData->layerList.at(index);
            for(int i = 0; i < curlayer->layerDataList.size(); i++) {
                MapLayerData* layerData = curlayer->layerDataList.at(i);
                MapSpriteItem* mapSpriteItem = currentMapSpriteItem(index,layerData);
                mapSpriteItem->setZValue(convertZToScene(layerData->z,index + 1));
                curLayerMapSpriteItemList.append(mapSpriteItem);
            }
            QList<MapSpriteItem*> preLayerMapSpriteItemList;
            MapLayer* preLayer = curMapData->layerList.at(index + 1);
            for(int i = 0; i < preLayer->layerDataList.size(); i++) {
                MapLayerData* layerData = preLayer->layerDataList.at(i);
                MapSpriteItem* mapSpriteItem = currentMapSpriteItem(index + 1,layerData);
                mapSpriteItem->setZValue(convertZToScene(layerData->z,index));
                preLayerMapSpriteItemList.append(mapSpriteItem);
            }
            //修改item layer
            for(int i = 0; i < curLayerMapSpriteItemList.size(); i++) {
                MapSpriteItem* mapSpriteItem = curLayerMapSpriteItemList.at(i);
                mapSpriteItem->setLayer(index + 1);
            }
            for(int i = 0; i < preLayerMapSpriteItemList.size(); i++) {
                MapSpriteItem* mapSpriteItem = preLayerMapSpriteItemList.at(i);
                mapSpriteItem->setLayer(index);
            }

            //添加数据
            MapLayer* layer = curMapData->layerList.at(index);
            MapLayer* newLayer = layer->getMapLayerClone();
            curMapData->delMapLayer(layer);
            curMapData->insertMapLayer(newLayer,index + 1);
        }
    }
}

void MapWindow::addMapSprite()
{
    //添加界面
    QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
    if(curItem) {
        //取得当前topLevelItem
        int index = currentTopLevelIndex(ui->tWLayers);
        QTreeWidgetItem *curTopItem = ui->tWLayers->topLevelItem(index);
        QTreeWidgetItem *child = new QTreeWidgetItem(curTopItem);
        child->setCheckState(0,Qt::Checked);
        this->setTWLayerCurItem(child,newMapLayerData);
        //        child->setText(0,newMapLayerData->mapSpriteName+"_pos:"+convertToStringFromPoint(newMapLayerData->pos)
        //                       +"_tag:"+convertToStringFromInt(newMapLayerData->tag)+"_z:"+convertToStringFromInt(newMapLayerData->z));

        //添加数据
        //取得当前层的数据
        MapLayer* curLayer = curMapData->layerList.at(index);
        curLayer->addMapLayerData(newMapLayerData);

        //添加精灵到地图中
        addMapSpriteByMapLayerData(newMapLayerData,index);

        //功能图标 绑定事件（主要是为了绑定棋子的z值）
        setMapObjectWithMapLayerData(newMapLayerData);
    }
}
//根据当前选中的item取item所在topLevelItem的index(坑爹的API竟然连这种方法都没有，bs)
int MapWindow::currentTopLevelIndex(QTreeWidget *treeWidget)
{
    QTreeWidgetItem *curItem = treeWidget->currentItem();
    return getTopLevelIndexByTreeWidgetItem(treeWidget,curItem);
}
int MapWindow::currentTopLevelIndexBy(QTreeWidget* treeWidget,QTreeWidgetItem *item)
{
    return getTopLevelIndexByTreeWidgetItem(treeWidget,item);
}

int MapWindow::getTopLevelIndexByTreeWidgetItem(QTreeWidget* treeWidget,QTreeWidgetItem* item)
{
    if(item)
    {
        for(int i = 0; i < treeWidget->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *curTopLevelItem = treeWidget->topLevelItem(i);
            qDebug()<<curTopLevelItem<<treeWidget->topLevelItem(i);
            if(item == curTopLevelItem)
            {
                return i;
            }
            else
            {
                for(int j = 0; j < curTopLevelItem->childCount(); j++)
                {
                    if(item == curTopLevelItem->child(j))
                    {
                        return i;
                    }
                }
            }
        }
    }

    return -1;
}

//遍历地图item寻找选中的item
MapSpriteItem* MapWindow::currentMapSpriteItem(int curLayerIndex,MapLayerData* layerData)
{
    //    qDebug("curLayerIndex = %d,tag = %d",curLayerIndex,layerData->tag);
    QList<QGraphicsItem*> itemList = curMap->mapEditView->scene()->items();
    for(int i = 0; i < itemList.size(); i++) {
        MapSpriteItem* item = (MapSpriteItem*)itemList.at(i);
        //        qDebug("for layer = %d,tag = %d",item->layer(),item->tag());
        if(item->layer() == curLayerIndex && layerData->tag == item->tag()) {
            return item;
        }
    }
    return NULL;
}

//遍历scene查找item
MapSpriteItem* MapWindow::mapSpriteItemAtPoint(QGraphicsScene *scene,QPoint point)
{
    QList<QGraphicsItem*> itemList = scene->items();
    //    qDebug("size = %d",itemList.size());
    for(int i = 0; i < itemList.size(); i++) {
        MapSpriteItem* item = (MapSpriteItem*)itemList.at(i);
        //        qDebug()<<(item->parentItem());
        //排除有父节点的ITEM
        if(item->parentItem() != 0 || !item->isVisible())
            continue;
        QPolygonF convertPolygonF= item->mapToScene(item->boundingRect());
        if(isPointInRect(point,convertPolygonF.boundingRect().toRect())) {
            return item;
        }
    }
    return NULL;
}

void MapWindow::on_tWLayers_itemClicked(QTreeWidgetItem *item, int column)
{
    //    qDebug("column = %d",column);

    QTreeWidgetItem* curItem = ui->tWLayers->currentItem();
    if(curItem) {
        setMapSpriteItemVisable(item);
        int index = ui->tWLayers->indexOfTopLevelItem(curItem);
        if(index != -1) {
            //图层 topLevel被选中
            ui->btnCopyLayer->setEnabled(true);

            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);

        }
        else {
            ui->btnCopyLayer->setEnabled(false);

            //取得当前层的数据
            int topItemIndex = currentTopLevelIndex(ui->tWLayers);
            QTreeWidgetItem *curTopItem = ui->tWLayers->topLevelItem(topItemIndex);
            int intChildIndex = curTopItem->indexOfChild(curItem);

            MapLayer* curLayer = curMapData->layerList.at(topItemIndex);
            MapLayerData* curLayerData = curLayer->layerDataList.at(intChildIndex);
            //选中精灵
            setCurChoosedMapSpriteItem(currentMapSpriteItem(topItemIndex,curLayerData));
            //            this->setCurChoosedMapLayerData(curLayerData);
            //            curChoosedMapSpriteItem = currentMapSpriteItem(topItemIndex,curLayerData);
            curMap->mapEditView->scene()->update();
            //释放选择数据
            //curChoosedMapLayerData = NULL;
        }
    }
}

void MapWindow::delMapSprite()
{
    int index = currentTopLevelIndex(ui->tWLayers);
    if(index != -1) {
        //删除界面
        QTreeWidgetItem *curTopLevelItem = ui->tWLayers->topLevelItem(index);
        QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
        int intChildIndex = curTopLevelItem->indexOfChild(curItem);
        curTopLevelItem->takeChild(intChildIndex);
        //        ui->tWLayers->removeItemWidget(curItem,0);

        //取得当前层的数据
        MapLayer* curLayer = curMapData->layerList.at(index);
        MapLayerData* curLayerData = curLayer->layerDataList.at(intChildIndex);
        //删除地图上item
        MapSpriteItem* mapSpriteItem = currentMapSpriteItem(index,curLayerData);
        curMap->mapEditView->scene()->removeItem(mapSpriteItem);
        //删除数据
        curLayer->delMapLayerData(curLayerData);

    }
}

void MapWindow::copyMapSprite()
{
    //添加界面
    QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
    if(curItem) {
        //取得当前topLevelItem
        int index = currentTopLevelIndex(ui->tWLayers);
        QTreeWidgetItem *curTopItem = ui->tWLayers->topLevelItem(index);
        int intChildIndex = curTopItem->indexOfChild(curItem);
        QTreeWidgetItem *child = curItem->clone();
        QString childText = child->text(0);
        child->setText(0,childText + tr("_copy"));
        curTopItem->addChild(child);

        //添加数据
        //取得当前层的数据
        MapLayer* curLayer = curMapData->layerList.at(index);
        MapLayerData* curLayerData = curLayer->layerDataList.at(intChildIndex);
        MapLayerData* newMapLayerData = curLayerData->getMapLayerDataClone();
        curLayer->addMapLayerData(newMapLayerData);

        //功能图标 绑定事件（主要是为了绑定棋子的z值）
        setMapObjectWithMapLayerData(newMapLayerData);
    }
}

void MapWindow::upMapSprite()
{
    int index = currentTopLevelIndex(ui->tWLayers);
    if(index != -1) {

        QTreeWidgetItem *curTopLevelItem = ui->tWLayers->topLevelItem(index);
        QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
        int intChildIndex = curTopLevelItem->indexOfChild(curItem);
        if(intChildIndex > 0) {
            //取数据
            MapLayer* curLayer = curMapData->layerList.at(index);
            MapLayerData* curLayerData = curLayer->layerDataList.at(intChildIndex);
            MapLayerData* preLayerData = curLayer->layerDataList.at(intChildIndex - 1);
            //修改地图上item zValue
            MapSpriteItem* curMapSpriteItem = currentMapSpriteItem(index,curLayerData);
            curMapSpriteItem->setZValue(convertZToScene(preLayerData->z,index));//同一层不用转换
            MapSpriteItem* preMapSpriteItem = currentMapSpriteItem(index,preLayerData);
            preMapSpriteItem->setZValue(convertZToScene(curLayerData->z,index));

            MapLayerData* copyCurLayerData = curLayerData->getMapLayerDataClone();
            int z = copyCurLayerData->z;
            copyCurLayerData->z = preLayerData->z;
            preLayerData->z = z;
            curLayer->delMapLayerData(curLayerData);
            curLayer->insertMapLayerData(copyCurLayerData,intChildIndex - 1);
            //界面
            QTreeWidgetItem *preItem = curTopLevelItem->child(intChildIndex - 1);
            this->setTWLayerCurItem(curItem,preLayerData);
            this->setTWLayerCurItem(preItem,copyCurLayerData);
            //            curItem->setText(0,preLayerData->mapSpriteName+"_pos:"+convertToStringFromPoint(preLayerData->pos)
            //                           +"_tag:"+convertToStringFromInt(preLayerData->tag)+"_z:"+convertToStringFromInt(preLayerData->z));
            //            preItem->setText(0,copyCurLayerData->mapSpriteName+"_pos:"+convertToStringFromPoint(copyCurLayerData->pos)
            //                           +"_tag:"+convertToStringFromInt(copyCurLayerData->tag)+"_z:"+convertToStringFromInt(copyCurLayerData->z));
            ui->tWLayers->setCurrentItem(preItem);

            //修改主角z
            //功能图标 绑定事件（主要是为了绑定棋子的z值）
            setMapObjectWithMapLayerData(preLayerData);
            setMapObjectWithMapLayerData(copyCurLayerData);

        }
    }
}

void MapWindow::downMapSprite()
{
    int index = currentTopLevelIndex(ui->tWLayers);
    if(index != -1) {

        QTreeWidgetItem *curTopLevelItem = ui->tWLayers->topLevelItem(index);
        QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
        int intChildIndex = curTopLevelItem->indexOfChild(curItem);
        int size = curTopLevelItem->childCount();
        if(intChildIndex < size - 1) {
            //取数据
            MapLayer* curLayer = curMapData->layerList.at(index);
            MapLayerData* curLayerData = curLayer->layerDataList.at(intChildIndex);
            MapLayerData* preLayerData = curLayer->layerDataList.at(intChildIndex + 1);
            //修改地图上item zValue
            MapSpriteItem* curMapSpriteItem = currentMapSpriteItem(index,curLayerData);
            curMapSpriteItem->setZValue(convertZToScene(preLayerData->z,index));
            MapSpriteItem* preMapSpriteItem = currentMapSpriteItem(index,preLayerData);
            preMapSpriteItem->setZValue(convertZToScene(curLayerData->z,index));

            MapLayerData* copyCurLayerData = curLayerData->getMapLayerDataClone();
            int z = copyCurLayerData->z;
            copyCurLayerData->z = preLayerData->z;
            preLayerData->z = z;
            curLayer->delMapLayerData(curLayerData);
            curLayer->insertMapLayerData(copyCurLayerData,intChildIndex + 1);
            //界面
            QTreeWidgetItem *preItem = curTopLevelItem->child(intChildIndex + 1);
            this->setTWLayerCurItem(curItem,preLayerData);
            this->setTWLayerCurItem(preItem,copyCurLayerData);
            //            curItem->setText(0,preLayerData->mapSpriteName+"_pos:"+convertToStringFromPoint(preLayerData->pos)
            //                           +"_tag:"+convertToStringFromInt(preLayerData->tag)+"_z:"+convertToStringFromInt(preLayerData->z));
            //            preItem->setText(0,copyCurLayerData->mapSpriteName+"_pos:"+convertToStringFromPoint(copyCurLayerData->pos)
            //                           +"_tag:"+convertToStringFromInt(copyCurLayerData->tag)+"_z:"+convertToStringFromInt(copyCurLayerData->z));
            ui->tWLayers->setCurrentItem(preItem);

            //修改主角z
            //功能图标 绑定事件（主要是为了绑定棋子的z值）
            setMapObjectWithMapLayerData(preLayerData);
            setMapObjectWithMapLayerData(copyCurLayerData);
        }
    }
}


void MapWindow::setMapSpriteAndLayerData(MapSpriteItem *ChoosedMapSpriteItem, MapLayerData *ChooseMapLayerData)
{
    if(ChooseMapLayerData)
    {

        MapSprite* mapSprite = this->getMapSpriteByName(ChooseMapLayerData->spriteName,ChooseMapLayerData->mapSpriteName);

        MapSpriteSubClip* subClip = mapSprite->subClipList_.at(0);
        QPoint pos = ChoosedMapSpriteItem->pos().toPoint();
        ChooseMapLayerData->pos = QPoint(pos.x() - subClip->position.x(),pos.y() - subClip->position.y());
        ChooseMapLayerData->mapSpriteName = ChoosedMapSpriteItem->getMapSpriteName();
        ChooseMapLayerData->z = convertZToData(ChoosedMapSpriteItem->zValue(),ChoosedMapSpriteItem->layer());
    }
}


void MapWindow::setMapSprite()
{
    //非空 修改数据
    if(curChoosedMapLayerData) {

        //这里curChoosedMapSpriteItem->pos() 是item的左上角  底层数据是sprite的偏移量的位置 所以要转换
        MapSprite* mapSprite = this->getMapSpriteByName(curChoosedMapLayerData->spriteName,curChoosedMapLayerData->mapSpriteName);
        MapSpriteSubClip* subClip = mapSprite->subClipList_.at(0);
        QPoint pos = curChoosedMapSpriteItem->pos().toPoint();
        curChoosedMapLayerData->pos = QPoint(pos.x() - subClip->position.x(),pos.y() - subClip->position.y());
        curChoosedMapLayerData->mapSpriteName = curChoosedMapSpriteItem->getMapSpriteName();
        curChoosedMapLayerData->z = convertZToData(curChoosedMapSpriteItem->zValue(),curChoosedMapSpriteItem->layer());
    }
}

//保存图层名字
void MapWindow::setMapLayerName()
{
    if(curMapData) {
        for(int i = 0; i < ui->tWLayers->topLevelItemCount(); i++) {
            QTreeWidgetItem* item = ui->tWLayers->topLevelItem(i);
            MapLayer* mapLayer = curMapData->layerList.at(i);
            mapLayer->mapLayerName = item->text(0);
        }
    }
}

//修改lay
void MapWindow::modifyMapSpriteLayer(int lay)
{
    MapLayerData* copyMapLayerData = curChoosedMapLayerData->getMapLayerDataClone();
    //删除原来的
    int curLayer = this->getCurLayer();
    curMapData->layerList.at(curLayer)->delMapLayerData(curChoosedMapLayerData);
    //添加新的到指定层
    MapLayer* mapLayer = curMapData->layerList.at(lay);
    copyMapLayerData->tag = mapLayer->intMaxTag;
    copyMapLayerData->z = copyMapLayerData->tag * 5;
    mapLayer->intMaxTag++;
    mapLayer->addMapLayerData(copyMapLayerData);

    //修改tree
    QTreeWidgetItem* curTopItem = ui->tWLayers->topLevelItem(curLayer);
    QTreeWidgetItem* curItem = ui->tWLayers->currentItem();
    curTopItem->removeChild(curItem);

    QTreeWidgetItem* topItem = ui->tWLayers->topLevelItem(lay);
    QTreeWidgetItem* newItem = new QTreeWidgetItem(topItem);
    newItem->setCheckState(0,Qt::Checked);
    this->setTWLayerCurItem(newItem,copyMapLayerData);

    //修改item z和其他数据
    curChoosedMapSpriteItem->setZValue(convertZToScene(copyMapLayerData->z,lay));
    curChoosedMapSpriteItem->setLayer(lay);
    curChoosedMapSpriteItem->setTag(copyMapLayerData->tag);

    ui->tWLayers->setCurrentItem(newItem);
}

//修改z
void MapWindow::modifyMapSpriteZ(int z)
{
    int layIndex = currentTopLevelIndex(ui->tWLayers);
    if(layIndex != -1) {
        MapLayer* curLayer = curMapData->layerList.at(layIndex);
        QTreeWidgetItem *curTopLevelItem = ui->tWLayers->topLevelItem(layIndex);
        int size = curTopLevelItem->childCount();
        //遍历寻找z值的位置
        int index = size-1;
        for(int i = 0; i < size - 1; i++) {
            MapLayerData* curLayerData = curLayer->layerDataList.at(i);
            MapLayerData* nextLayerData = curLayer->layerDataList.at(i+1);
            if(curLayerData->z == z) {
                index = i;
                break;
            }
            else if(curLayerData->z < z && nextLayerData->z > z) {
                index = i + 1;
                break;
            }
        }

        //复制一份
        MapLayerData* curMapLayerData = getCurChoosedMapLayerData();
        MapLayerData* copyCurLayerData = curMapLayerData->getMapLayerDataClone();
        copyCurLayerData->z = z;

        curLayer->delMapLayerData(curMapLayerData);
        curLayer->insertMapLayerData(copyCurLayerData,index);

        //重新构建z值
        for(int i = 0; i < curLayer->layerDataList.size(); i++) {
            MapLayerData* tmpMapLayerData = curLayer->layerDataList.at(i);
            tmpMapLayerData->z = i * 5;
            MapSpriteItem* tmpItem = currentMapSpriteItem(layIndex,tmpMapLayerData);

            tmpItem->setZValue(convertZToScene(tmpMapLayerData->z,layIndex));

            QTreeWidgetItem* childItem = curTopLevelItem->child(i);
            setTWLayerCurItem(childItem,tmpMapLayerData);

            setMapObjectWithMapLayerData(tmpMapLayerData);
        }

        //        qDebug("size = %d",size);
        //        qDebug("index = %d",index);
        QTreeWidgetItem* curItem = curTopLevelItem->child(index);
        ui->tWLayers->setCurrentItem(curItem);
    }
}
//修改图层位置
void MapWindow::modifyMapLayerPos(int offsetX,int offsetY,bool isSave)
{
    QTreeWidgetItem* curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = ui->tWLayers->indexOfTopLevelItem(curItem);
        MapLayer* curLayer = curMapData->layerList.at(index);

        for(int i = 0; i < curItem->childCount(); i ++) {
            QTreeWidgetItem* child = curItem->child(i);
            MapSpriteItem* item = getMapSpriteItemByTreeWidgetItem(child);
            //            item->setVisible(isCheck);
            item->setPos(item->pos().x() + offsetX,item->pos().y() + offsetY);

            if(isSave) {
                //取得当前层的数据
                curChoosedMapSpriteItem = item;
                MapLayerData* curLayerData = curLayer->layerDataList.at(i);
                setCurChoosedMapLayerData(curLayerData);
                setMapSprite();
            }
        }
    }
}

//修改地图扩展
void MapWindow::modifyMapSize(int extend_left,int extend_right)
{
    //修改位置
    int offsetX = extend_left * intCurGridW;
    int offsetY = extend_right * intCurGridH;

    intCurGridWNum += extend_left;
    intCurGridHNum += extend_right;

    int size = curMapData->layerList.size();
    for(int i = 0; i < size; i++) {
        MapLayer* curLayer = curMapData->layerList.at(i);
        for(int j = 0; j < curLayer->layerDataList.size(); j ++) {
            //            QTreeWidgetItem* child = curItem->child(i);
            //            MapSpriteItem* item = getMapSpriteItemByTreeWidgetItem(child);
            MapLayerData* curLayerData = curLayer->layerDataList.at(j);
            MapSpriteItem* item = currentMapSpriteItem(i,curLayerData);
            //            item->setVisible(isCheck);
            item->setPos(item->pos().x() + offsetX,item->pos().y() + offsetY);

            //            if(isSave) {
            //取得当前层的数据
            curChoosedMapSpriteItem = item;

            setCurChoosedMapLayerData(curLayerData);
            setMapSprite();
            //            }
        }
    }

    //修改数据
    size = curMapData->mapObjectList.size();
    for(int i = 0; i < size; i++) {

        MapObjectData*  mapObjData = curMapData->mapObjectList.at(i);

        QPoint curPoint = mapObjData->curPoint;//本身位置

        mapObjData->curPoint = QPoint(curPoint.x() + extend_left,curPoint.y() + extend_right);
        for(int j = 0; j < mapObjData->nextPointList.size(); j++) {
            QPoint nextPoint = mapObjData->nextPointList.at(j);
            nextPoint = QPoint(nextPoint.x() + extend_left,nextPoint.y() + extend_right);
            mapObjData->nextPointList.replace(j,nextPoint);
        }
    }

}

//取得当前选中的地图精灵

MapSpriteItem * MapWindow::getCurChoosedMapSpriteItem()
{
    return curChoosedMapSpriteItem;
}

QList<MapSpriteItem *>  *MapWindow::getCurMulChooseMapSpriteItems()
{
    return &curChoosedMapSpriteItemList;
}
QList <MapLayerData *> *MapWindow::getCurMulChooseMapLayerDatas()
{
    return &curChoosedMapLayerDataList;
}


MapLayerData * MapWindow::getCurChoosedMapLayerData()
{
    return curChoosedMapLayerData;
}

MapLayerData *MapWindow::getByMapSpriteItem(MapSpriteItem *item)
{

    QTreeWidgetItem* topItem = ui->tWLayers->topLevelItem(item->layer());
    MapLayer* mapLayer = curMapData->layerList.at(item->layer());
    for(int i = 0; i < mapLayer->layerDataList.size(); i++)
    {
        MapLayerData* mapLayerData = mapLayer->layerDataList.at(i);
        if(mapLayerData->tag == item->tag())
        {
            QTreeWidgetItem *item = topItem->child(i);
            int index = currentTopLevelIndexBy(ui->tWLayers,item);
            if(index != -1)
            {
                QTreeWidgetItem *curTopLevelItem = ui->tWLayers->topLevelItem(index);
                //QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
                int intChildIndex = curTopLevelItem->indexOfChild(item);
                MapLayer* curLayer = curMapData->layerList.at(index);
                if(intChildIndex<=curLayer->layerDataList.count()-1)
                {
                    MapLayerData* curLayerData = curLayer->layerDataList.at(intChildIndex);
                     return curLayerData;
                }
                else
                {
                    return NULL;
                }

            }

        }
    }

    return NULL;
}


void MapWindow::setCurChoosedMapSpriteItem(MapSpriteItem *item,bool isAltKeyDowning)
{
    curChoosedMapSpriteItem = item;
    int index = currentTopLevelIndex(ui->tWLayers);
    if(index != -1)
    {
        QTreeWidgetItem *curTopLevelItem = ui->tWLayers->topLevelItem(index);
        QTreeWidgetItem *curItem = ui->tWLayers->currentItem();
        int intChildIndex = curTopLevelItem->indexOfChild(curItem);
        curItem->setSelected(true);
        //取得当前层的数据
        MapLayer* curLayer = curMapData->layerList.at(index);
        MapLayerData* curLayerData = curLayer->layerDataList.at(intChildIndex);
        setCurChoosedMapLayerData(curLayerData);
    }

}

void MapWindow::setCurChoosedMapSpriteItemInTree(MapSpriteItem *item,bool isAltKeyDowning)
{
        QTreeWidgetItem* topItem = ui->tWLayers->topLevelItem(item->layer());
        MapLayer* mapLayer = curMapData->layerList.at(item->layer());
        for(int i = 0; i < mapLayer->layerDataList.size(); i++)
        {
            MapLayerData* mapLayerData = mapLayer->layerDataList.at(i);
            if(mapLayerData->tag == item->tag())
            {
                QTreeWidgetItem *item = topItem->child(i);
                ui->tWLayers->setCurrentItem(item);
              //  item->setSelected(true);
                break;
            }
        }
}

//设置当前选中的mapLayerData
void MapWindow::setCurChoosedMapLayerData(MapLayerData* layerData)
{
    curChoosedMapLayerData = layerData;
}

//因为修改了数据，也要修改图层的item信息
void MapWindow::setTWLayerCurItem(QTreeWidgetItem *treeWidgetItem, MapLayerData *layerData)
{
    if(ui->tWLayers->indexOfTopLevelItem(treeWidgetItem) != -1)
        return;
    treeWidgetItem->setText(0,layerData->mapSpriteName+"_pos:"+convertToStringFromPoint(layerData->pos)
                            +"_tag:"+convertToStringFromInt(layerData->tag)+"_z:"+convertToStringFromInt(layerData->z));
}

int MapWindow::convertZToScene(int data_z, int curLayer)
{
    return data_z + curLayer * 10000;
}

int MapWindow::convertZToData(int scene_z, int curLayer)
{
    return scene_z - curLayer * 10000;
}

//mapLayerData 与 MapSpriteItem 映射
MapSpriteItem* MapWindow::getMapSpriteItemByTreeWidgetItem(QTreeWidgetItem* item)
{
    if(item) {
        int layer = getTopLevelIndexByTreeWidgetItem(ui->tWLayers,item);
        //        int layer = currentTopLevelIndex(ui->tWLayers);
        if(layer != -1) {
            QTreeWidgetItem* curTopLevelItem = ui->tWLayers->topLevelItem(layer);
            int intChildIndex = curTopLevelItem->indexOfChild(item);
            if(intChildIndex == -1)
                return NULL;
            //取得item的数据
            MapLayer* curLayer = curMapData->layerList.at(layer);
            MapLayerData* curLayerData = curLayer->layerDataList.at(intChildIndex);
            //删除地图上item
            MapSpriteItem* mapSpriteItem = currentMapSpriteItem(layer,curLayerData);
            return mapSpriteItem;
        }
    }
    return NULL;
}

void MapWindow::setMapSpriteItemVisable(QTreeWidgetItem *item)
{
    int intCheckState = item->checkState(0);
    bool isCheck = false;
    if(intCheckState == Qt::Unchecked) {
        isCheck = false;
    }
    else {
        isCheck = true;
    }
    //    isCheck = !isCheck;
    //    qDebug("isCheck %d",intCheckState);
    for(int i = 0; i < item->childCount(); i ++) {
        QTreeWidgetItem* child = item->child(i);
        if(isCheck) {
            child->setCheckState(0,Qt::Checked);
        }
        else {
            child->setCheckState(0,Qt::Unchecked);
        }

        MapSpriteItem* item = getMapSpriteItemByTreeWidgetItem(child);
        item->setVisible(isCheck);
    }

    if(isCheck) {
        item->setCheckState(0,Qt::Checked);
    }
    else {
        item->setCheckState(0,Qt::Unchecked);
    }


    MapSpriteItem* childitem = getMapSpriteItemByTreeWidgetItem(item);
    if(childitem) {
        childitem->setVisible(isCheck);
    }
}


//设置当前地图数据,通过当前地图，刷新公有的属性，比如地图名字，格子信息
void MapWindow::setCurMapTabData(int index)
{
    if(index < 0 || index > mapTabList.size() - 1)
        return;
    curMap = mapTabList.at(index);
    curMapData = mapDataList.at(index);
    curChoosedMapSpriteItem = NULL;
    curChoosedMapLayerData = NULL;
    curMapName = curMapData->mapName;
    intCurGridW = curMapData->intGridW;
    intCurGridH = curMapData->intGridH;
    intCurGridWNum = curMapData->intGridWNum;
    intCurGridHNum = curMapData->intGridHNum;
    //清除图层
    ui->tWLayers->clear();
    //添加新图层
    for(int i = 0; i < curMapData->layerList.size(); i++) {
        //添加层
        MapLayer* mapLayer = curMapData->layerList.at(i);
        addMapLayerByName(mapLayer->mapLayerName);
        for(int j = 0; j < mapLayer->layerDataList.size(); j++) {
            MapLayerData* mapLayerData = mapLayer->layerDataList.at(j);
            //添加item到图层
            QTreeWidgetItem *curTopItem = ui->tWLayers->topLevelItem(i);
            QTreeWidgetItem *child = new QTreeWidgetItem(curTopItem);
            child->setCheckState(0,Qt::Checked);
            setTWLayerCurItem(child,mapLayerData);
        }
    }
    //刷新路径
    IOData* io = IOData::getInstance();
    io->do_switchMap(curMapData->mapFilePath);
    //刷新底色
    curMap->mapEditView->setBackgroundBrush(QBrush(curMapData->backGroundColor));
    curMap->mapEditView->setAutoFillBackground(true);
}

MapData* MapWindow::getCurMapData()
{
    return curMapData;
}

int MapWindow::getCurLayer()
{
    return currentTopLevelIndex(ui->tWLayers);
}

void MapWindow::on_tabMap_tabCloseRequested(int index)
{
    ui->tabMap->removeTab(index);

    mapTabList.removeAt(index);
    mapDataList.removeAt(index);

    //清除图层
    ui->tWLayers->clear();

    int curTabIndex = ui->tabMap->currentIndex();
    setCurMapTabData(curTabIndex);
}
#include <QLabel>
#include <QLineEdit>
#include <QtWidgets/QtWidgets>
///////地图设置
MapSettingDialog::MapSettingDialog()
{
    extend_left_count = 0;
    extend_up_count = 0;
    intExtendLeftGridNum = 0;
    intExtendUpGridNum = 0;
    this->setWindowTitle(tr("地图设置"));
    QGridLayout* layout = new QGridLayout(this);
    QLabel *labMapName = new QLabel(tr("地图名:"),this);
    layout->addWidget(labMapName,0,0);
    leMapName = new QLineEdit(curMapName,this);
    layout->addWidget(leMapName,0,1,1,2);

    // new add by phileas in 2013/5/9
    QLabel *labMapW = new QLabel(tr("地图宽:"),this);
    layout->addWidget(labMapW,1,0);
    QString str = convertToStringFromInt(intCurMapW);
    leMapWidth = new QLineEdit(str,this);
    layout->addWidget(leMapWidth,1,1);

    QLabel *labMapH = new QLabel(tr("地图高:"),this);
    layout->addWidget(labMapH,1,2);
    str = convertToStringFromInt(intCurMapH);
    leMapHeight = new QLineEdit(str,this);
    layout->addWidget(leMapHeight,1,3);


    // new add end

    QLabel *labW = new QLabel(tr("网格宽:"),this);
    layout->addWidget(labW,2,0);
    str = convertToStringFromInt(intCurGridW);
    leGridW = new QLineEdit(str,this);
    layout->addWidget(leGridW,2,1);

    QLabel *labH = new QLabel(tr("网格高:"),this);
    layout->addWidget(labH,2,2);
    str = convertToStringFromInt(intCurGridH);
    leGridH = new QLineEdit(str,this);
    layout->addWidget(leGridH,2,3);

    QLabel *labGridWNum = new QLabel(tr("网格数w:"),this);
    layout->addWidget(labGridWNum,3,0);
    str = convertToStringFromInt(intCurGridWNum);
    leGridWNum = new QLineEdit(str,this);
    layout->addWidget(leGridWNum,3,1);

    QLabel *labGridHNum = new QLabel(tr("网格数h:"),this);
    layout->addWidget(labGridHNum,3,2);
    str = convertToStringFromInt(intCurGridHNum);
    leGridHNum = new QLineEdit(str,this);
    layout->addWidget(leGridHNum,3,3);

    QLabel *labGridWNum_extend = new QLabel(tr("向左扩展:"),this);
    layout->addWidget(labGridWNum_extend,4,0);
    str = convertToStringFromInt(intExtendLeftGridNum);
    leGridExtendLeft = new QLineEdit(str,this);
    layout->addWidget(leGridExtendLeft,4,1);

    QLabel *labGridHNum_extend = new QLabel(tr("向上扩展:"),this);
    layout->addWidget(labGridHNum_extend,4,2);
    str = convertToStringFromInt(intExtendUpGridNum);
    leGridExtendUp = new QLineEdit(str,this);
    layout->addWidget(leGridExtendUp,4,3);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0,0,0,0);
    QPushButton *btnConfirm = new QPushButton(tr("确定"),this);
    btnLayout->addWidget(btnConfirm);
    QPushButton *btnCancel = new QPushButton(tr("取消"),this);
    btnLayout->addWidget(btnCancel);

    layout->addLayout(btnLayout,5,1,1,2);

    connect(leMapHeight, SIGNAL(editingFinished()), this, SLOT(onChangMapHeight()));
    connect(leMapWidth, SIGNAL(editingFinished()), this, SLOT(onChangMapWidth()));
    connect(leGridH, SIGNAL(editingFinished()), this, SLOT(onChangeGridHeight()));
    connect(leGridW, SIGNAL(editingFinished()), this, SLOT(onChangeGridWidth()));
    connect(leGridHNum, SIGNAL(editingFinished()), this, SLOT(onChangeGridHNum()));
    connect(leGridWNum, SIGNAL(editingFinished()), this, SLOT(onChangeGridWNum()));

    connect(btnConfirm,SIGNAL(clicked()),this,SLOT(on_btnConfirm_clicked()));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(close()));

}

void MapSettingDialog::on_btnConfirm_clicked()
{
    int tmpGridW;
    int tmpGridH;
    int tmpGridWNum;
    int tmpGridHNum;
    int tmpExtendLeftGridNum;
    int tmpExtendUpGridNum;


    curMapName = leMapName->text();
    tmpGridW = (leGridW->text()).toInt();
    tmpGridH = (leGridH->text()).toInt();
    tmpGridWNum = (leGridWNum->text()).toInt();
    tmpGridHNum = (leGridHNum->text()).toInt();
    tmpExtendLeftGridNum = (leGridExtendLeft->text()).toInt();
    tmpExtendUpGridNum = (leGridExtendUp->text()).toInt();
    if(curMapName == "" || tmpGridH == 0 || tmpGridW == 0 || tmpGridWNum == 0 || tmpGridHNum == 0) {
        QMessageBox mess;
        mess.setText(tr("地图名不能为空或其他参数不能为0"));
        mess.exec();
        return;
    }

    intCurGridW = tmpGridW ;
    intCurGridH = tmpGridH;
    intCurGridWNum = tmpGridWNum;
    intCurGridHNum = tmpGridHNum;
    intExtendLeftGridNum = tmpExtendLeftGridNum;
    intExtendUpGridNum = tmpExtendUpGridNum;
    accept();
}

void MapSettingDialog::onChangMapHeight()
{
    int tmpMapHeight = (leMapHeight->text()).toInt();

    if(tmpMapHeight != 0)
    {
        int tmpGridHNum = tmpMapHeight / intCurGridH;

        intCurGridHNum = (tmpMapHeight - intCurGridH*tmpGridHNum) < ((intCurGridH*(tmpGridHNum+1)-tmpMapHeight)) ? tmpGridHNum : (tmpGridHNum+1);
        intCurMapH = intCurGridH * intCurGridHNum;
        leMapHeight->setText(QString::number(intCurMapH));
        leGridHNum->setText(QString::number(intCurGridHNum));
    }
    else
    {
        leMapHeight->setText(QString::number(intCurMapH));
    }


}

void MapSettingDialog::onChangMapWidth()
{
    int tmpMapWidth = (leMapWidth->text()).toInt();
    if(tmpMapWidth != 0)
    {
        int tmpGridWNum = tmpMapWidth / intCurGridW;

        intCurGridWNum = (tmpMapWidth - intCurGridW*tmpGridWNum) < ((intCurGridW*(tmpGridWNum+1)-tmpMapWidth)) ? tmpGridWNum : (tmpGridWNum+1);
        intCurMapW = intCurGridW * intCurGridWNum;
        leMapWidth->setText(QString::number(intCurMapW));
        leGridWNum->setText(QString::number(intCurGridWNum));
    }
    else
    {
        leMapWidth->setText(QString::number(intCurMapW));
    }


}

void MapSettingDialog::onChangeGridHeight()
{
    int tmpGridheight = (leGridH->text()).toInt();


    if(tmpGridheight != 0)
    {
        intCurGridH = tmpGridheight;
        int tmpGridHNum = intCurMapH / tmpGridheight;
        intCurGridHNum = (intCurMapH - intCurGridH*tmpGridHNum) < (intCurGridH*(tmpGridHNum+1)- intCurMapH) ? tmpGridHNum : (tmpGridHNum+1);
        intCurMapH = intCurGridHNum * intCurGridH;
        leMapHeight->setText(QString::number(intCurMapH));
        leGridHNum->setText(QString::number(intCurGridHNum));
        leGridH->setText(QString::number(intCurGridH));
    }
    else
    {
        leGridH->setText(QString::number(intCurGridH));
    }
}

void MapSettingDialog::onChangeGridWidth()
{
    int tmpGridWidth = (leGridW->text()).toInt();

    if(tmpGridWidth != 0)
    {
        intCurGridW = tmpGridWidth;
        int tmpGridWNum = intCurMapW / tmpGridWidth;
        intCurGridWNum = (intCurMapW - intCurGridW*tmpGridWNum) < (intCurGridW*(tmpGridWNum+1)- intCurMapW) ? tmpGridWNum : (tmpGridWNum+1);
        intCurMapW = intCurGridWNum * intCurGridW;
        leMapWidth->setText(QString::number(intCurMapW));
        leGridWNum->setText(QString::number(intCurGridWNum));
        leGridW->setText(QString::number(intCurGridW));
    }
    else
    {
        leGridW->setText(QString::number(intCurGridW));
    }

}

void MapSettingDialog::onChangeGridHNum()
{
    int tmpGridHNum = (leGridHNum->text()).toInt();

    if ( tmpGridHNum != 0)
    {
        intCurGridHNum = tmpGridHNum;
        intCurMapH = intCurGridH * intCurGridHNum;
        leMapHeight->setText(QString::number(intCurMapH));
    }
    else
    {
        leGridHNum->setText(QString::number(intCurGridHNum));
    }

}

void MapSettingDialog::onChangeGridWNum()
{
    int tmpGridWNum = (leGridWNum->text()).toInt();

    if (tmpGridWNum != 0)
    {
        intCurGridWNum = tmpGridWNum;
        intCurMapW = intCurGridW * intCurGridWNum;
        leMapWidth->setText(QString::number(intCurMapW));
    }
    else
    {
        leGridWNum->setText(QString::number(intCurGridWNum));
    }
}

//当前的地图精灵MapSprite
MapSprite * MapWindow::getCurMapSprite()
{
    return curMapSprite;
}


QPoint MapWindow::getPasteGridCenterPoint(QPoint curPoint)
{
    //计算所处的网格
    int wIndex = curPoint.x()/intCurGridW;
    int hIndex = curPoint.y()/intCurGridH;

    QPoint gridPointCenter ;
    if(isMidPasteGid)
    {
        gridPointCenter = QPoint(wIndex*intCurGridW+intCurGridW/2,hIndex*intCurGridH+intCurGridH/2);

    }
    else
    {
        gridPointCenter= QPoint(wIndex*intCurGridW,hIndex*intCurGridH);//不要把图片放到中心点 直接放到格子的左上点
    }

    return gridPointCenter;
}




void MapWindow::on_tWLayers_doubleClicked(const QModelIndex &index)
{
    QTreeWidgetItem* curItem = ui->tWLayers->currentItem();
    if(curItem) {
        int index = ui->tWLayers->indexOfTopLevelItem(curItem);
        if(index != -1) {
            curItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
            //弹出图层位置窗口
            ModifyMapLayerPosDialog modifyMapLayerPos(this);
            int rec = modifyMapLayerPos.exec();
            if(rec == QDialog::Accepted) {

            }
        }
        else {
            curItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
            //        setMapSpriteItemVisable(curItem);
            //弹出修改z窗口
            ModifyMapSpriteInfoDialog modifyMapSpriteInfo(this);
            int rec = modifyMapSpriteInfo.exec();
            if(rec == QDialog::Accepted) {

            }
        }

    }
}

void MapWindow::on_tWLayers_itemChanged(QTreeWidgetItem *item, int column)
{
    int index = ui->tWLayers->indexOfTopLevelItem(item);
    if(index != -1 && index < curMapData->layerList.size()) {
        MapLayer* mapLayer = curMapData->layerList.at(index);
        if(mapLayer->mapLayerName != item->text(0)) {
            //            qDebug()<<tr("修改图层名字");
            mapLayer->mapLayerName = item->text(0);
            //            setMapLayerName();
        }
    }

}

void MapWindow::on_tabWMapSprite_tabCloseRequested(int index)
{
    QString tapName = ui->tabWMapSprite->tabText(index);
    //删除数据
    for(int i = 0; i < spriteDataList.size(); i++) {
        SpriteData* spriteData = spriteDataList.at(i);
        if(spriteData->spriteName_ == tapName) {
            spriteDataList.removeOne(spriteData);
        }
    }
    if(curStMapSprite.spriteName == tapName) {
        curStMapSprite.spriteName = "";
        curStMapSprite.mapSpriteName = "";
    }

    ui->tabWMapSprite->removeTab(index);


}

void MapWindow::changeMapEditViewBackGround()
{
    QColorDialog chooseColorDialog(Qt::darkGray, this);
    int rec = chooseColorDialog.exec();
    if(rec == QDialog::Accepted) {
        QColor color = chooseColorDialog.selectedColor();

        if(curMap) {
            curMap->mapEditView->setBackgroundBrush(QBrush(color));
            curMap->mapEditView->setAutoFillBackground(true);

            curMapData->backGroundColor = color;
            //curMapData->mapBackGroundColor = QString("{%1,%2,%3,%4}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        }

    }
}

void MapWindow::on_tabMap_currentChanged(int index)
{
    setCurMapTabData(index);
}

//视图显示
void MapWindow::showLayer(bool visible)
{
    ui->dwLayers->setVisible(visible);
}

void MapWindow::showHistory(bool visible)
{
    ui->dwHistory->setVisible(visible);
}

void MapWindow::showSprite(bool visible)
{
    ui->dwSprites->setVisible(visible);
}

//显示事件
void MapWindow::showEvent(bool visible)
{
    ui->dwEvent->setVisible(visible);
    this->tabifyDockWidget(ui->dwLayers,ui->dwEvent);
    if(visible) {
        //刷新事件列表
        IOData* io = IOData::getInstance();
        io->readEvent(eventList);

        ui->listWEvent->clear();
        QString event_id;
        for(int i = 0; i < eventList->size(); i++) {
            event_id = convertToStringFromInt( eventList->at(i)->id );
            QListWidgetItem* item = new QListWidgetItem(event_id);
            ui->listWEvent->addItem(item);
        }
        ui->listWEvent->setCurrentRow(0);
    }
}

void MapWindow::refreshMap()
{
    //刷新当前地图
    if(curMapData) {
        if(curMapData->mapFilePath == "") {
            QMessageBox message;
            message.setText(tr("请先保存地图，再刷新"));
            message.exec();
            return;
        }
        //保存底色
        QBrush brush = curMap->mapEditView->backgroundBrush();
        QString fileName = curMapData->mapFilePath;
        //先关闭
        int index = ui->tabMap->currentIndex();
        on_tabMap_tabCloseRequested(index);
        //重新打开
        openMapFile(fileName);
        if(curMap) {
            curMap->mapEditView->setBackgroundBrush(brush);
            curMap->mapEditView->setAutoFillBackground(true);
        }
    }
}

MapObjectData& MapWindow::getCurMapObjectData()
{
    return mapObjectData;
}

void MapWindow::addMapObjectData(QPoint point)
{
    int index = getMapObjectDataByPoint(point);
    if(index == -1) {
        //添加新节点
        MapObjectData* newObject = new MapObjectData(point);
        curMapData->addMapObjectData(newObject);
        //        qDebug()<<tr("添加新节点");
    }
}

void MapWindow::addMapObjectNextPoint(QPoint nextPoint)
{
    if(mapObjectData.curPoint.x() != -1 && mapObjectData.curPoint.y() != -1) {

        //        if(mapObjectData.curPoint == nextPoint) {
        //            return;
        //        }
        //临近的格子才会添加
        QPoint subPoint = nextPoint - mapObjectData.curPoint;
        if(subPoint == QPoint(-1,0) || subPoint == QPoint(1,0) || subPoint == QPoint(0,-1) || subPoint == QPoint(0,1)) {
            int index = getMapObjectDataByPoint(mapObjectData.curPoint);
            if(index != -1) {
                MapObjectData* curObject = curMapData->mapObjectList.at(index);
                //判断是否已存在这个节点
                for(int i = 0; i < curObject->nextPointList.size(); i++) {
                    if(curObject->nextPointList.at(i) == nextPoint) {
                        return;
                    }
                }
                curObject->nextPointList.append(nextPoint);
                //                qDebug()<<tr("添加下个位置节点");
            }
        }
    }

}

void MapWindow::delMapObjectData(QPoint point)
{
    int index = getMapObjectDataByPoint(point);
    if(index != -1) {
        //删除节点
        MapObjectData* curObject = curMapData->mapObjectList.at(index);
        curMapData->delMapObjectData(curObject);
        //        qDebug()<<tr("删除节点");
    }
}

//设置路线节点事件
void MapWindow::setMapObjectEventId(QPoint point,int eventId)
{
    int index = getMapObjectDataByPoint(point);
    if(index == -1) {
        //不存在 添加
        MapObjectData* newObject = new MapObjectData(point);
        newObject->eventId = eventId;
        curMapData->addMapObjectData(newObject);
    }
    else {
        //存在 修改
        MapObjectData* curObject = curMapData->mapObjectList.at(index);
        curObject->eventId = eventId;
    }
    curMap->mapEditView->scene()->update();
}

//设置路线节点z
void MapWindow::setMapObjectZ(QPoint point,int z,int tag)
{
    int index = getMapObjectDataByPoint(point);
    if(index == -1) {
        //不存在 添加
        MapObjectData* newObject = new MapObjectData(point);
        newObject->z = z;
        newObject->tag = tag;
        curMapData->addMapObjectData(newObject);
    }
    else {
        //存在 修改
        MapObjectData* curObject = curMapData->mapObjectList.at(index);
        curObject->z = z;
        curObject->tag = tag;
    }
    curMap->mapEditView->scene()->update();
}
//功能图标绑定事件,添加事件和z值
void MapWindow::setMapObjectWithMapLayerData(MapLayerData* mapLayerData)
{
    //功能图标 绑定事件（主要是为了绑定棋子的z值）
    if(mapLayerData->spriteName == QString("icon")) {
        //添加路径节点
        //计算所在的网格
        int wIndex = mapLayerData->pos.x()/intCurGridW;
        int hIndex = mapLayerData->pos.y()/intCurGridH;
        //设置事件id
        int eventId = -1;//默认空格子
        if(mapLayerData->mapSpriteName == "random") {
            eventId = 13;
        }
        else if(mapLayerData->mapSpriteName == "reward") {
            eventId = 10;
        }
        else if(mapLayerData->mapSpriteName == "fight") {
            eventId = 12;
        }
        else if(mapLayerData->mapSpriteName == "hurt_1") {
            eventId = 4;
        }
        else if(mapLayerData->mapSpriteName == "hurt_2") {
            eventId = 14;
        }
        else if(mapLayerData->mapSpriteName == "b_box") {
            eventId = 8;
        }
        else if(mapLayerData->mapSpriteName == "s_box") {
            eventId = 15;
        }
        else if(mapLayerData->mapSpriteName == "return") {
            eventId = 3;
        }
        else if(mapLayerData->mapSpriteName == "mercenary") {
            eventId = 6;
        }
        else if(mapLayerData->mapSpriteName == "money") {
            eventId = 7;
        }
        else if(mapLayerData->mapSpriteName == "recover") {
            eventId = 5;
        }
        else if(mapLayerData->mapSpriteName == "start") {
            eventId = 1;
        }
        else if(mapLayerData->mapSpriteName == "tool") {
            eventId = 9;
        }
        else if(mapLayerData->mapSpriteName == "boss") {
            eventId = 11;
        }
        else if(mapLayerData->mapSpriteName == "clip14") {
            eventId = 16;
        }
        else if(mapLayerData->mapSpriteName == "s_boss") {
            eventId = 2;
        }

        setMapObjectEventId(QPoint(wIndex,hIndex),eventId);
        //设置z
        setMapObjectZ(QPoint(wIndex,hIndex),mapLayerData->z,mapLayerData->tag);
    }
}

//返回当前事件的id
int MapWindow::getCurChoosedEventId()
{
    if(!isShowEvent) {
        return -1;
    }
    int index = ui->listWEvent->currentRow();
    if(index < 0)
        index = 0;
    EventData* eventData = eventList->at(index);
    return eventData->id;
}

int MapWindow::getMapObjectDataByPoint(QPoint point)
{
    if(curMapData) {
        for(int i = 0; i < curMapData->mapObjectList.size(); i++) {
            MapObjectData* mapObject = curMapData->mapObjectList.at(i);
            if(mapObject->curPoint == point) {
                return i;
            }
        }
    }
    return -1;
}

ModifyMapSpriteInfoDialog::ModifyMapSpriteInfoDialog(MapWindow *mapWindow)
{
    this->mapWindow = mapWindow;
    this->setWindowTitle(tr("地图设精灵置"));
    QGridLayout* layout = new QGridLayout(this);

    MapData* curMapData = mapWindow->getCurMapData();
    MapLayerData* curMapLayerData = mapWindow->getCurChoosedMapLayerData();
    if(curMapData == NULL || curMapLayerData == NULL)
        return;
    QLabel *labLay = new QLabel(tr("图层:"),this);
    layout->addWidget(labLay,0,0);
    spinBoxLay = new QSpinBox(this);
    spinBoxLay->setRange(0,curMapData->layerList.size() - 1);
    spinBoxLay->setValue(mapWindow->getCurLayer());
    spinBoxLay->setSingleStep(1);
    layout->addWidget(spinBoxLay,0,1,1,2);

    QLabel *labZ = new QLabel(tr("z:"),this);
    layout->addWidget(labZ,1,0);
    spinBoxZ = new QSpinBox(this);
    spinBoxZ->setRange(0,10000);
    spinBoxZ->setValue(curMapLayerData->z);
    spinBoxZ->setSingleStep(5);
    layout->addWidget(spinBoxZ,1,1,1,2);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0,0,0,0);
    QPushButton *btnConfirm = new QPushButton(tr("确定"),this);
    btnLayout->addWidget(btnConfirm);
    QPushButton *btnCancel = new QPushButton(tr("取消"),this);
    btnLayout->addWidget(btnCancel);

    layout->addLayout(btnLayout,2,1,1,2);

    connect(btnCancel,SIGNAL(clicked()),this,SLOT(on_btnCancel_clicked()));
    connect(btnConfirm,SIGNAL(clicked()),this,SLOT(on_btnConfirm_clicked()));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(spinBoxZ,SIGNAL(valueChanged(int)),this,SLOT(changeZ(int)));
    connect(spinBoxLay,SIGNAL(valueChanged(int)),this,SLOT(changeLayer(int)));
}

void ModifyMapSpriteInfoDialog::on_btnConfirm_clicked()
{
    int lay = spinBoxLay->value();
    if(lay != mapWindow->getCurLayer()) {
        mapWindow->modifyMapSpriteLayer(lay);
    }
    //确认修改z值
    int z = spinBoxZ->value();
    //相同z 要往后移动
    MapLayerData* curMapLayerData = mapWindow->getCurChoosedMapLayerData();
    if(z != curMapLayerData->z) {
        mapWindow->modifyMapSpriteZ(z);
    }
    accepted();
    close();
}

void ModifyMapSpriteInfoDialog::changeLayer(int lay)
{

}

void ModifyMapSpriteInfoDialog::changeZ(int z)
{
    MapSpriteItem* curMapSpriteItem = mapWindow->getCurChoosedMapSpriteItem();
    curMapSpriteItem->setZValue(mapWindow->convertZToScene(z,curMapSpriteItem->layer()));
    //qDebug()<<z;
}

void ModifyMapSpriteInfoDialog::on_btnCancel_clicked()
{
    //还原z
    MapLayerData* curMapLayerData = mapWindow->getCurChoosedMapLayerData();
    if(curMapLayerData == NULL)
        return;
    MapSpriteItem* curMapSpriteItem = mapWindow->getCurChoosedMapSpriteItem();
    curMapSpriteItem->setZValue(mapWindow->convertZToScene(curMapLayerData->z,curMapSpriteItem->layer()));
}

ModifyMapLayerPosDialog::ModifyMapLayerPosDialog(MapWindow *mapWindow)
{
    this->mapWindow = mapWindow;
    this->setWindowTitle(tr("图层位置修改"));
    QGridLayout* layout = new QGridLayout(this);

    offsetX = 0;
    offsetY = 0;

    QLabel *labLay = new QLabel(tr("偏移量x:"),this);
    layout->addWidget(labLay,0,0);
    spinBoxX = new QSpinBox(this);
    spinBoxX->setRange(-10000,10000);
    spinBoxX->setValue(offsetX);
    spinBoxX->setSingleStep(1);
    layout->addWidget(spinBoxX,0,1,1,2);

    QLabel *labZ = new QLabel(tr("偏移量y:"),this);
    layout->addWidget(labZ,1,0);
    spinBoxY = new QSpinBox(this);
    spinBoxY->setRange(-10000,10000);
    spinBoxY->setValue(offsetY);
    spinBoxY->setSingleStep(1);
    layout->addWidget(spinBoxY,1,1,1,2);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0,0,0,0);
    QPushButton *btnConfirm = new QPushButton(tr("确定"),this);
    btnLayout->addWidget(btnConfirm);
    QPushButton *btnCancel = new QPushButton(tr("取消"),this);
    btnLayout->addWidget(btnCancel);

    layout->addLayout(btnLayout,2,1,1,2);

    connect(btnCancel,SIGNAL(clicked()),this,SLOT(on_btnCancel_clicked()));
    connect(btnConfirm,SIGNAL(clicked()),this,SLOT(on_btnConfirm_clicked()));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(spinBoxX,SIGNAL(valueChanged(int)),this,SLOT(changeOffsetX(int)));
    connect(spinBoxY,SIGNAL(valueChanged(int)),this,SLOT(changeOffsetY(int)));
}

void ModifyMapLayerPosDialog::on_btnCancel_clicked()
{
    mapWindow->modifyMapLayerPos(-offsetX,-offsetY);
    close();
}

void ModifyMapLayerPosDialog::on_btnConfirm_clicked()
{
    //    changePos();
    //    int offsetX = spinBoxX->value();
    //    int offsetY = spinBoxY->value();
    mapWindow->modifyMapLayerPos(-offsetX,-offsetY);
    mapWindow->modifyMapLayerPos(offsetX,offsetY,true);
    accepted();
    close();
}
void ModifyMapLayerPosDialog::changeOffsetX(int x)
{
    mapWindow->modifyMapLayerPos(-offsetX,-offsetY);
    offsetX = x;
    mapWindow->modifyMapLayerPos(offsetX,offsetY);
}
void ModifyMapLayerPosDialog::changeOffsetY(int y)
{
    mapWindow->modifyMapLayerPos(-offsetX,-offsetY);
    offsetY = y;
    mapWindow->modifyMapLayerPos(offsetX,offsetY);
}

void ModifyMapLayerPosDialog::changePos()
{
}
