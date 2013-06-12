#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QSpinBox>
#include <QDialog>
#include "Data/spritedata.h"
#include "Data/mapdata.h"
#include "Data/eventdata.h"
#include "maptab.h"
#include "mapspriteitem.h"

class MapTab;
namespace Ui {
    class MapWindow;
}

struct StChooseMapSprite{
    //精灵名--文件的最顶层名字 即文件的id
    QString spriteName;
    //地图精灵名字 --- 精灵的id
    QString mapSpriteName;
    //图像名字
    QString imageName;
};

enum POINTNEAR
{
    LEFTTOP,
    RIGHTTOP,
    RIGHTBOTTOM,
    LEFTBOTTOM
};


class MapWindow : public QMainWindow
{
    Q_OBJECT
    friend class MapTab;
public:
    explicit MapWindow(QWidget *parent = 0);
    ~MapWindow();

    //打开文件
//    void openNewSpriteFile();
    void openMapFile(QString fileName);
    //保存文件
    void saveMapFile();
    void saveMapFileAs();
    //新建一张地图
    void newMap();
    //导入一个sprite文件;
    void importSpriteFile(QString fileName);
    //设置地图大小
    void setMapSize(QRect rect);
    //设置名字
    void setMapName(QString mapName);
    //设置网格
    void setShowGrid(bool showGrid);
    bool getShowGrid();
    //设置吸附
    void setPasteGrid(bool pasteGrid);
    bool getPasteGrid();
    void toScaleViewAdd();
    void toScaleViewSub();
    void toScaleHuanyuan();


    //设置显示或隐藏
    void setMapSpriteItemVisable(QTreeWidgetItem* item);
    //设置编辑模式
    void setEditMode(int mode);
    int getEditMode();
    //设置当前地图数据,通过当前地图，刷新公有的属性，比如地图名字，格子信息
    void setCurMapTabData(int index);
    MapData* getCurMapData();
    int getCurLayer();
    //设置当前mapSpriteitem
    void setCurMapSpriteItem(QString mapSpriteName);
    //添加spritedata
    bool addSpriteData(SpriteData* spriteData);
    //判断是否是靠近 上下左右的哪个定点
    QPoint getPasteGridCenterPointPointNear(QPoint curPoint,QRectF  rectf);
    //网格吸附坐标转换
    QPoint getPasteGridCenterPoint(QPoint curPoint);
    //根据数据构建场景 打开地图时
    void createSceneByMapData(MapData* mapData);
    void addMapLayerByName(QString layerName);
    void addMapSpriteByMapLayerData(MapLayerData* mapLayerData,int layer);
    //视图显示
    //显示图层
    void showLayer(bool visible);
    //显示历史
    void showHistory(bool visible);
    //显示精灵
    void showSprite(bool visible);
    //显示事件
    void showEvent(bool visible);
    //刷新地图
    void refreshMap();
    //当前路线对象节点
    MapObjectData& getCurMapObjectData();
    //通过精灵名字找精灵
    MapSprite* getMapSprite();
    MapSprite* getMapSpriteByName(QString spriteName,QString mapSpriteName);
    QString getMapSpriteImageName();
    QImage getMapSpriteImage();
    QString getImageName();
    QString getMapSpriteName();
    //当前的地图精灵MapSprite
    MapSprite* getCurMapSprite();
    //生成新的mapLayerData
    void createMapLayerData(QPoint pos);
    //根据当前选中的item取item所在topLevelItem的index(坑爹的API竟然连这种方法都没有，bs)
    int currentTopLevelIndex(QTreeWidget* treeWidget);
    int currentTopLevelIndexBy(QTreeWidget* treeWidget,QTreeWidgetItem *item);
    int getTopLevelIndexByTreeWidgetItem(QTreeWidget* treeWidget,QTreeWidgetItem* item);
    //遍历地图item寻找选中的item
    MapSpriteItem* currentMapSpriteItem(int curLayerIndex,MapLayerData* layerData);
    //遍历scene查找item 排除有父节点的item
    MapSpriteItem* mapSpriteItemAtPoint(QGraphicsScene *scene,QPoint point);
    //因为修改了数据，也要修改图层的item信息
    void setTWLayerCurItem(QTreeWidgetItem* treeWidgetItem,MapLayerData* layerData);
    //转换z值--数据中不同的层可以有相同的z，画面上没有层的区分，只有z值
    int convertZToScene(int data_z,int curLayer);
    int convertZToData(int scene_z,int curLayer);
    //取得当前选中的地图精灵
    MapSpriteItem* getCurChoosedMapSpriteItem();
    MapLayerData* getCurChoosedMapLayerData();
    MapLayerData *getByMapSpriteItemAndDel(MapSpriteItem *item,bool flag);
    MapLayerData *getByMapSpriteItem(MapSpriteItem *item);
    void  deleteTreeItemByMapSpriteItem(MapSpriteItem *item);

    QList<MapSpriteItem *>  *getCurMulChooseMapSpriteItems();
    QList<MapLayerData *>  *getCurMulChooseMapLayerDatas();

    //add  多选的环境下;
    void setCurChoosedMapSpriteItem(MapSpriteItem* item ,bool isAltKeyDowning=false) ;
    void setCurChoosedMapSpriteItemInTree(MapSpriteItem* item,bool isAltKeyDowning=false);
    //设置当前选中的mapLayerData
    void setCurChoosedMapLayerData(MapLayerData* layerData);
    //TreeWidgetItem 与 MapSpriteItem 映射
    MapSpriteItem* getMapSpriteItemByTreeWidgetItem(QTreeWidgetItem* item);
//    MapLayerData* getMapLayerDataByMapSpriteItem(MapSpriteItem* mapSpriteItem);
    //改变编辑区的底色
    void changeMapEditViewBackGround();
    //添加/修改数据
    void addMapLayer();
    void delMapLayer();
    void copyMapLayer();
    void upMapLayer();
    void downMapLayer();
    void addMapSprite();
    void delMapSprite();
    void delMapSprites();
    void copyMapSprite();
    void upMapSprite();
    void downMapSprite();
    void setMapSprite();
    void setMapSpriteAndLayerData( MapSpriteItem* curChoosedMapSpriteItem, MapLayerData* newMapLayerData);
    //当前选中的item
    void setMapLayerName();
    //修改lay
    void modifyMapSpriteLayer(int lay);
    //修改z
    void modifyMapSpriteZ(int z);
    //修改图层位置
    void modifyMapLayerPos(int offsetX,int offsetY,bool isSave = false);
    //修改地图扩展
    void modifyMapSize(int extend_left,int extend_right);

    //添加路线节点
    void addMapObjectData(QPoint point);
    //添加已存在路线节点的下个路线
    void addMapObjectNextPoint(QPoint nextPoint);
    //删除路线节点
    void delMapObjectData(QPoint point);
    //设置路线节点事件
    void setMapObjectEventId(QPoint point,int eventId);
    //设置路线节点z
    void setMapObjectZ(QPoint point,int z,int tag);
    //返回当前事件的id
    int getCurChoosedEventId();
    //判断是否存在路线节点
    int getMapObjectDataByPoint(QPoint point);
    //功能图标绑定事件,添加事件和z值
    void setMapObjectWithMapLayerData(MapLayerData* mapLayerData);


    QImage imgUp;
    QImage imgDown;
    QImage imgLeft;
    QImage imgRight;
    QImage imgShoe;

private slots:
    //图层按钮
    void on_btnAddLayer_clicked();
    void on_btnDelLayer_clicked();
    void on_btnCopyLayer_clicked();
    void on_btnUpLayer_clicked();
    void on_btnDownLayer_clicked();

    void on_tWLayers_itemClicked(QTreeWidgetItem *item, int column);
    //关闭地图
    void on_tabMap_tabCloseRequested(int index);

    void on_tWLayers_doubleClicked(const QModelIndex &index);

    void on_tWLayers_itemChanged(QTreeWidgetItem *item, int column);
    //关闭精灵
    void on_tabWMapSprite_tabCloseRequested(int index);

    void on_tabMap_currentChanged(int index);


private:
    void uniteDockWidget();
    Ui::MapWindow *ui;

    //当前tab
    MapTab* curMap;
    //打开的地图tab
    QList<MapTab*> mapTabList;
    //当前地图数据
    MapData *curMapData;
    //打开的地图的数据
    QList<MapData*> mapDataList;
    //当前选中地图精灵
    StChooseMapSprite curStMapSprite;
    MapSprite* curMapSprite;
    //生成新的mapLayerData
    MapLayerData* newMapLayerData;
    //当前选中的item
    MapSpriteItem* curChoosedMapSpriteItem;
    //当前选中精灵的列表
    QList<MapSpriteItem *> curChoosedMapSpriteItemList;

    //当前选中的mapLayerData
    MapLayerData* curChoosedMapLayerData;

    //当前选中的mapLayerData列表
    QList<MapLayerData *> curChoosedMapLayerDataList;



    //所需的spritedata
    QList<SpriteData*> spriteDataList;
    //编辑模式 0---编辑 1---拖拽 2 -- 路线 3 -- 事件
    int intEditMode;
    //当前路线对象节点
    MapObjectData mapObjectData;
    //事件数据
    QList<EventData*>* eventList;

};


class MapSettingDialog : public QDialog
{
    Q_OBJECT
public:
    MapSettingDialog();

    // new add by phileas.chen in 2013/5/9
    QLineEdit *leMapWidth;
    QLineEdit *leMapHeight;
    // new add end:

    QLineEdit *leMapName;
    QLineEdit *leGridW;
    QLineEdit *leGridH;
    QLineEdit *leGridWNum;
    QLineEdit *leGridHNum;
    QLineEdit *leGridExtendLeft;
    QLineEdit *leGridExtendUp;
    //向左扩展
    int extend_left_count;
    //向上扩张
    int extend_up_count;
private slots:
    //等比例窗口
    void on_btnConfirm_clicked();

    void onChangMapHeight();
    void onChangMapWidth();
    void onChangeGridHeight();
    void onChangeGridWidth();
    void onChangeGridHNum();
    void onChangeGridWNum();
private:
//    SpriteWindow* spriteWindow_;
};

class ModifyMapSpriteInfoDialog : public QDialog
{
    Q_OBJECT
public:
    ModifyMapSpriteInfoDialog(MapWindow* mapWindow);
    QSpinBox *spinBoxZ;
    QSpinBox *spinBoxLay;
private slots:
    void on_btnCancel_clicked();
    void on_btnConfirm_clicked();
    void changeZ(int z);
    void changeLayer(int lay);
private:
    MapWindow* mapWindow;
};

class ModifyMapLayerPosDialog : public QDialog
{
    Q_OBJECT
public:
    ModifyMapLayerPosDialog(MapWindow* mapWindow);
    QSpinBox *spinBoxX;
    QSpinBox *spinBoxY;
private slots:
    void on_btnCancel_clicked();
    void on_btnConfirm_clicked();
    void changeOffsetX(int x);
    void changeOffsetY(int y);
    void changePos();
private:
    MapWindow* mapWindow;
    int offsetX;
    int offsetY;
};

#endif // MAPWINDOW_H
