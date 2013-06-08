#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "spriteEditor/spritewindow.h"
#include "config.h"


#ifdef Q_WS_WIN
const QString imagePath = "./gameResource/images/";
const QString spritePath = "./gameResource/sprite/";
const QString spriteConfigPath = "./gameResource/sprite/config/";
const QString mapPath = "./gameResource/map/";
const QString eventPath = "./gameResource/event/";
#endif
#ifdef Q_OS_MAC
    //mac
//const QString imagePath = "./gameResource/images/";
//const QString spritePath = "./gameResource/sprite/";
//const QString spriteConfigPath = "./gameResource/sprite/config/";
//const QString mapPath = "./gameResource/map/";
//const QString eventPath = "./gameResource/event/";
#endif


QString imagePath = "./";
QString spritePath = "./";
QString spriteConfigPath = "./";
QString mapPath = "./";
QString eventPath = "./";


//当前的地图设置
QString curMapName = "mapName";

// new add by phileas.chen in 2013/5/9
int intCurMapW = 64*16;
int intCurMapH = 64*12;
// new add end

int intCurGridW = 64;
int intCurGridH = 64;
int intCurGridWNum = 16;
int intCurGridHNum = 12;
int intExtendLeftGridNum = 0;
int intExtendUpGridNum = 0;
//视图中标志
bool isShowLayer = true;
bool isShowHistory = false;
bool isShowSprite = true;
bool isShowEvent = false;
bool isMoveChooseSprite = false;
bool isShowChooseSpriteRect = false;
bool isShowGrid = false;//显示网格
bool isPasteGrid = false;//网格吸附
bool isMidPasteGid=false;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mapWin_ = NULL;
    sprWin_ = NULL;
    subWin_mapWin = NULL;
    subWin_sprWin = NULL;
    //绑定信号和槽
    connectSignalsAndSlots();

    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
//    setStyleSheet();

    //初始化视图标志
    ui->actShowLayer->setChecked(isShowLayer);
    ui->actShowHistory->setChecked(isShowHistory);
    ui->actShowSprite->setChecked(isShowSprite);
    ui->actShowEvent->setChecked(isShowEvent);
    ui->actIsMoveChooseSprite->setChecked(isMoveChooseSprite);
    ui->actShowChooseSpriteRect->setChecked(isShowChooseSpriteRect);
    ui->actShowGrid->setChecked(isShowGrid);//isPasteGrid
   // ui->actPasteGrid->setCheckable(true);
    ui->actPasteGrid->setChecked(isPasteGrid);
    ui->actionPasteGridMiddle->setCheckable(true);

    // init setting path:
   // ui->actPasteGrid->setShortcut(QKeySequence(tr("Ctrl+b")));
    this->readPathSetting();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete mapWin_;
    mapWin_ = NULL;
    delete sprWin_;
    sprWin_ = NULL;
    if(subWin_mapWin != NULL) {
        delete subWin_mapWin;
        subWin_mapWin = NULL;
    }
    if(subWin_sprWin != NULL) {
        delete subWin_sprWin;
        subWin_sprWin = NULL;
    }
}


//slots
//新建
void MainWindow::newFile()
{
    chooseFileType fileType(this);
    int result = fileType.exec();
    if(result == QDialog::Accepted) {
        if(intFileType == 0) {
            newMapFile();
        }
        else if(intFileType == 1) {
            newSpriteFile();
        }
    }
}
//打开文件
void MainWindow::openFile()
{
    chooseFileType fileType(this);
    int result = fileType.exec();
    if(result == QDialog::Accepted) {
        if(intFileType == 0) {
            openMapFile();
        }
        else if(intFileType == 1) {
            openSpriteFile();
        }
    }
}
//保存文件
void MainWindow::saveFile()
{
    if(ui->mdiArea->currentSubWindow() == subWin_mapWin) {
         if(mapWin_ != NULL) {
             mapWin_->saveMapFile();
             qDebug("save Map");
         }
     }
    else if(ui->mdiArea->currentSubWindow() == subWin_sprWin) {
        if(sprWin_ != NULL) {
           sprWin_->saveSpriteFile();
           qDebug() << ("Save sprite");
        }
    }
}
//文件另存为
void MainWindow::saveAsFile()
{
    if(ui->mdiArea->currentSubWindow() == subWin_sprWin) {
        if(sprWin_ != NULL) {
           sprWin_->saveSpriteFileAs();
        }
    }
    else if(ui->mdiArea->currentSubWindow() == subWin_mapWin) {
        if(mapWin_ != NULL) {
            mapWin_->saveMapFileAs();
        }
    }
}

//绑定信号和槽
void MainWindow::connectSignalsAndSlots()
{
    connect(ui->actNew,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(ui->actOpen,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(ui->actSave,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(ui->actSaveAs,SIGNAL(triggered()),this,SLOT(saveAsFile()));

    connect(ui->actQuit,SIGNAL(triggered()),qApp,SLOT(closeAllWindows()));
}


//新建地图
void MainWindow::newMapFile()
{
    //新建地图窗口
    if(mapWin_ == NULL) {
        mapWin_ = createMapWin();
        if(mapWin_ == NULL) {
            return;
        }
        mapWin_->show();
    }
    //新建地图
    mapWin_->newMap();
}
//打开地图文件
void MainWindow::openMapFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,QObject::tr("打开map文件"),mapPath,QObject::tr("map文件 (*.map *.plist *.xml)") );
    if(!fileName.isEmpty()) {
        if(mapWin_ == NULL) {
            MapWindow *mapWin = new MapWindow;
            subWin_mapWin = new QMdiSubWindow;
            // new add:
        //    mapWin->openMapFile(fileName);
            // new end:
            subWin_mapWin->setWidget(mapWin);
            ui->mdiArea->addSubWindow(subWin_mapWin);
            mapWin_ = mapWin;
            mapWin_->show();
        }
        if(mapWin_ != NULL) {
            mapWin_->openMapFile(fileName);
        }
    }
}

//新建精灵
void MainWindow::newSpriteFile()
{
    //新建精灵窗口
    if(sprWin_ == NULL) {
        sprWin_ = createSprWin();
        sprWin_->show();
    }
    //新建精灵文件
    sprWin_->openNewSpriteFile();
}
//打开精灵文件
void MainWindow::openSpriteFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,QObject::tr("打开sprite文件"),spritePath,QObject::tr("sprite文件 (*.sprite *.plist *.xml)") );
    if(!fileName.isEmpty()) {
        newSpriteFile();
        if(sprWin_ != NULL) {
            sprWin_->openSpriteFile(fileName);
        }
    }
}


//新建地图
MapWindow *MainWindow::createMapWin()
{
    MapSettingDialog mapSetting;
    int rec = mapSetting.exec();

    if(rec == QDialog::Accepted) {

        MapWindow *mapWin = new MapWindow;
        subWin_mapWin = new QMdiSubWindow;
        subWin_mapWin->setWidget(mapWin);
        ui->mdiArea->addSubWindow(subWin_mapWin);
        return mapWin;
    }
    return NULL;
}

SpriteWindow * MainWindow::createSprWin()
{
    SpriteWindow *sprWin = new SpriteWindow;
    sprWin->setMainWindow(this);
    subWin_sprWin = new QMdiSubWindow;
    subWin_sprWin->setWidget(sprWin);
    ui->mdiArea->addSubWindow(subWin_sprWin);
    return sprWin;
}

//放大
void MainWindow::on_actZoomIn_triggered()
{
    if(sprWin_ != NULL) {
       sprWin_->zoom_in();
    }
    if(mapWin_)
    {
        mapWin_->toScaleViewAdd();
    }

}
//缩小
void MainWindow::on_actZoomOut_triggered()
{
    if(sprWin_ != NULL) {
       sprWin_->zoom_out();
    }
    if(mapWin_)
    {
        mapWin_->toScaleViewSub();
    }
}
//原始大小
void MainWindow::on_actOriginal_triggered()
{
    if(sprWin_ != NULL) {
       sprWin_->zoom_original();
    }
    if(mapWin_)
    {
        mapWin_->toScaleHuanyuan();
    }
}


void MainWindow::on_actUndo_activated()
{
    if(sprWin_ != NULL) {
        qDebug("undo");
       sprWin_->undo_Memento_ClipData();
    }
}

void MainWindow::on_actRedo_triggered()
{
    if(sprWin_ != NULL) {
        qDebug("redo");
       sprWin_->redo_Memento_ClipData();
    }
}
//导入sprite文件
void MainWindow::on_actImportSprite_triggered()
{
    if(mapWin_ != NULL) {
        QString fileName = QFileDialog::getOpenFileName(this,QObject::tr("打开sprite文件"),spritePath,QObject::tr("sprite文件 (*.sprite *.plist *.xml)") );
        mapWin_->importSpriteFile(fileName);
    }
}
//编辑模式
void MainWindow::on_actMapEdit_triggered()
{
    if(mapWin_ != NULL) {
        mapWin_->setEditMode(0);
        ui->actDragMapSprite->setChecked(false);
        ui->actPath->setChecked(false);
        ui->actEvent->setChecked(false);
    }
}
//拖拽模式
void MainWindow::on_actDragMapSprite_triggered()
{
    if(mapWin_ != NULL) {
        mapWin_->setEditMode(1);
        ui->actMapEdit->setChecked(false);
        ui->actPath->setChecked(false);
        ui->actEvent->setChecked(false);
    }
}
//路线模式
void MainWindow::on_actPath_triggered()
{
    if(mapWin_ != NULL) {
        mapWin_->setEditMode(2);
        ui->actMapEdit->setChecked(false);
        ui->actDragMapSprite->setChecked(false);
        ui->actEvent->setChecked(false);
    }
}
//事件模式
void MainWindow::on_actEvent_triggered()
{
    if(mapWin_ != NULL) {
        mapWin_->setEditMode(3);
        ui->actMapEdit->setChecked(false);
        ui->actDragMapSprite->setChecked(false);
        ui->actPath->setChecked(false);
    }
}

void MainWindow::on_actMapPathSetting_triggered()
{
    QString tmp =  QFileDialog::getExistingDirectory(this,"选择Map文件路径",mapPath,QFileDialog::ShowDirsOnly);
    if(tmp.isEmpty() == false)
    {
        mapPath = tmp+"/";
        this->writePathSetting(mapPath, 1);
    }

}

void MainWindow::on_actSpritePathSetting_triggered()
{

    QString tmp = QFileDialog::getExistingDirectory(this,"选择Sprite文件路径",spritePath,QFileDialog::ShowDirsOnly);
    if(tmp.isEmpty() == false)
    {
        spritePath =  tmp+"/";
        this->writePathSetting(spritePath, 2);
    }
}

void MainWindow::on_actImagePathSetting_triggered()
{
     QString tmp = QFileDialog::getExistingDirectory(this,"选择Image文件路径",imagePath,QFileDialog::ShowDirsOnly);
     if(tmp.isEmpty() == false)
     {
         imagePath =  tmp+"/";
         this->writePathSetting(imagePath, 3);
     }
}

void MainWindow::on_actSpriteConfigPathSetting_triggered()
{
  QString tmp = QFileDialog::getExistingDirectory(this,"选择SpriteConfig文件路径",spriteConfigPath,QFileDialog::ShowDirsOnly);
  if(tmp.isEmpty() == false)
  {
      spriteConfigPath =  tmp+"/";
      this->writePathSetting(spriteConfigPath, 4);

  }
}

void MainWindow::on_actRestoresDefault_triggered()
{
    imagePath = "./";
    spritePath = "./";
    spriteConfigPath = "./";
    mapPath = "./";
    //eventPath = "./";
    QDir tmpDir = QDir(mapPath);
    mapPath = tmpDir.absolutePath()+"/";
    tmpDir.setPath(spritePath);
    spritePath = tmpDir.absolutePath()+"/";
    tmpDir.setPath(imagePath);
    imagePath = tmpDir.absolutePath()+"/";
    tmpDir.setPath(spriteConfigPath);
    spriteConfigPath = tmpDir.absolutePath()+"/";

    this->writePathSetting(mapPath, 1);
    this->writePathSetting(spritePath,2);
    this->writePathSetting(imagePath, 3);
    this->writePathSetting(spriteConfigPath, 4);
}

void MainWindow::on_actMapPathSetting_hovered()
{
    qDebug() << "hello, world" << mapPath;
    this->ui->actMapPathSetting->setStatusTip(mapPath);
    this->ui->actMapPathSetting->setToolTip(mapPath);
}

void MainWindow::on_actSpritePathSetting_hovered()
{
    this->ui->actSpritePathSetting->setStatusTip(spritePath);
}

void MainWindow::on_actImagePathSetting_hovered()
{
    this->ui->actImagePathSetting->setStatusTip(imagePath);
}

void MainWindow::on_actSpriteConfigPathSetting_hovered()
{
    this->ui->actSpriteConfigPathSetting->setStatusTip(spriteConfigPath);
}

void MainWindow::readPathSetting()
{
    QSettings settings("Setting.ini", QSettings::IniFormat);
    mapPath = settings.value("Path/mapPath", mapPath).toString();
    spritePath = settings.value("Path/spritePath", spritePath).toString();
    imagePath = settings.value("Path/imagePath", imagePath).toString();
    spriteConfigPath = settings.value("Path/spriteConfigPath", spriteConfigPath).toString();

    qDebug() << "mapPath" << mapPath;
    qDebug() << "spritePath" << spritePath;
    qDebug() << "imagePath" << imagePath;
    qDebug() << "spriteConfig" << spriteConfigPath;
}

void MainWindow::writePathSetting(const QString &inPath, const int &inType)
{
    QSettings settings("Setting.ini", QSettings::IniFormat); // 当前目录的INI文件
    settings.beginGroup("Path");
    switch(inType)
    {
    case 1:
        settings.setValue("mapPath", inPath);
        break;
    case 2:
        settings.setValue("spritePath", inPath);
        break;
    case 3:
         settings.setValue("imagePath", inPath);
        break;
    case 4:
         settings.setValue("spriteConfigPath", inPath);
        break;
    default:
        break;

    }

    settings.endGroup();

}

//地图设置
void MainWindow::on_actMapSet_triggered()
{
    MapSettingDialog mapSetting;
    int rec = mapSetting.exec();

    if(rec == QDialog::Accepted)
    {
        if(mapWin_ == NULL)
        {
            return;
        }
        mapWin_->modifyMapSize(intExtendLeftGridNum,intExtendUpGridNum);
        mapWin_->setMapName(curMapName);
        mapWin_->setMapSize(QRect(0,0,intCurGridW*intCurGridWNum,intCurGridH*intCurGridHNum));
    }
}
//显示网格
void MainWindow::on_actShowGrid_triggered()
{
    isShowGrid = ui->actShowGrid->isChecked();
    if(mapWin_) {
        mapWin_->setShowGrid(isShowGrid);
    }
}




void MainWindow::on_actionPasteGridMiddle_triggered()
{
    if(isPasteGrid==false)
    {
        isPasteGrid=true;
         ui->actPasteGrid->setChecked(true);
    }

        isMidPasteGid= ui->actionPasteGridMiddle->isChecked();
        if(mapWin_)
        {
            mapWin_->setPasteGrid(isMidPasteGid);
        }
}

//网格吸附
void MainWindow::on_actPasteGrid_triggered()
{
    isPasteGrid = ui->actPasteGrid->isChecked();
    if(isPasteGrid==false && isMidPasteGid==true)
    {
        ui->actionPasteGridMiddle->setChecked(false);
        isMidPasteGid=false;
    }
    if(mapWin_) {
        mapWin_->setPasteGrid(isPasteGrid);
    }
}


//文件类型窗口
chooseFileType::chooseFileType(MainWindow* mainWindow)
{
    this->mainWindow = mainWindow;
    this->setWindowTitle(tr("选择文件类型"));
    QStringList fileList;
    fileList << tr("地图文件") << tr("精灵文件");
    QGridLayout* layout = new QGridLayout(this);

    twFileList = new QTreeWidget(this);
    twFileList->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
    for(int i = 0; i < fileList.size(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0,fileList.at(i));
        twFileList->addTopLevelItem(item);
    }

    layout->addWidget(twFileList,0,0);

    connect(twFileList,SIGNAL(itemDoubleClicked ( QTreeWidgetItem*, int)),this,SLOT(on_itemDoubleClicked(QTreeWidgetItem *, int)));
}

void chooseFileType::on_itemDoubleClicked(QTreeWidgetItem * item, int column )
{
    int index = twFileList->indexOfTopLevelItem(item);
    if(index == 0) {
        mainWindow->intFileType = 0;
        this->accept();
    }
    else if(index == 1) {
        mainWindow->intFileType = 1;
        this->accept();
    }
}

//改变编辑区底色
void MainWindow::on_actChangeColor_triggered()
{
    if(ui->mdiArea->currentSubWindow() == subWin_sprWin) {
        if(sprWin_ != NULL) {
//           sprWin_->saveSpriteFileAs();
            sprWin_->changeEditAreaBackGround();
        }
    }
    else if(ui->mdiArea->currentSubWindow() == subWin_mapWin) {
        if(mapWin_ != NULL) {
            mapWin_->changeMapEditViewBackGround();
        }
    }
}
//显示图层
void MainWindow::on_actShowLayer_triggered()
{
    isShowLayer = ui->actShowLayer->isChecked();
    if(ui->mdiArea->currentSubWindow() == subWin_mapWin) {
        if(mapWin_ != NULL) {
            mapWin_->showLayer(isShowLayer);
        }
    }
}
//显示历史
void MainWindow::on_actShowHistory_triggered()
{
    isShowHistory = ui->actShowHistory->isChecked();
    if(ui->mdiArea->currentSubWindow() == subWin_mapWin) {
        if(mapWin_ != NULL) {
            mapWin_->showHistory(isShowHistory);
        }
    }
}
//显示精灵
void MainWindow::on_actShowSprite_triggered()
{
    isShowSprite = ui->actShowSprite->isChecked();
    if(ui->mdiArea->currentSubWindow() == subWin_mapWin) {
        if(mapWin_ != NULL) {
            mapWin_->showSprite(isShowSprite);
        }
    }
}
//移动选择精灵
void MainWindow::on_actIsMoveChooseSprite_triggered()
{
    isMoveChooseSprite = ui->actIsMoveChooseSprite->isChecked();
}
//显示光标跟随精灵的rect
void MainWindow::on_actShowChooseSpriteRect_triggered()
{
    isShowChooseSpriteRect = ui->actShowChooseSpriteRect->isChecked();
}

void MainWindow::on_actRefreshMap_triggered()
{
    if(ui->mdiArea->currentSubWindow() == subWin_mapWin) {
        if(mapWin_ != NULL) {
            mapWin_->refreshMap();
        }
    }
}

//显示事件
void MainWindow::on_actShowEvent_triggered()
{
    isShowEvent = ui->actShowEvent->isChecked();
    if(ui->mdiArea->currentSubWindow() == subWin_mapWin) {
        if(mapWin_ != NULL) {
            mapWin_->showEvent(isShowEvent);
        }
    }
}




