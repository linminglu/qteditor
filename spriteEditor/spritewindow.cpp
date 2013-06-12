#include "spritewindow.h"
#include "ui_spritewindow.h"
//#include <QtGui>
#include <QGraphicsItem>
#include "Utility.h"
#include "iodata.h"
#include "Command/ClipEditCommand/ClipEditCommand.h"
#include "Command/SpriteEditCommand/SpriteEditCommand.h"
#include "config.h"

using namespace Utility;

SpriteWindow::SpriteWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpriteWindow)
{
    ui->setupUi(this);

    cmdInvoker_ = new CommandInvoker();
    spriteData_ = new SpriteData();
    taker_spriteData_ = new careTaker_SpriteData(spriteData_);

    //设置切片splitter比列
    ui->splitter_clip->setStretchFactor(0, 1);
    ui->splitter_clip->setStretchFactor(1, 3);

    //设置精灵splitter比列
    ui->splitter_sprite->setStretchFactor(0, 1);
    ui->splitter_sprite->setStretchFactor(1, 2);
    ui->splitter_sprite->setStretchFactor(2, 1);

    //初始化clip GraphicsView
    initClipGraphicsView();
    //初始化mapSprite GraphicsView
    initMapSpriteGraphicsView();

    //切片面板 切片列表右键菜单 = new Action();
    actDelClip = new QAction(tr("删除"), this);
    actDelClip->setShortcuts(QKeySequence::Delete);
    actDelClip->setStatusTip(tr("删除切片"));
    connect(actDelClip, SIGNAL(triggered()), this, SLOT(cmd_delClip()));
    ui->clipListW->setContextMenuPolicy(Qt::CustomContextMenu);

    initData();

    //绑定槽
    // 改动选中切片
    connect(this,SIGNAL(signal_choosedClipChanged(int)),this,SLOT(slot_updateClipListW(int)));
    connect(this,SIGNAL(signal_choosedClipChanged(int)),this,SLOT(slot_updateClipProperty(int)));
    connect(this,SIGNAL(signal_choosedClipChanged(int)),ui->graphicsView,SLOT(slot_updateChoosedClip(int)));




    ///////精灵面板
    //绑定槽
    // 改动选中的精灵
    connect(this,SIGNAL(signal_choosedMapSpriteChanged(int)),this,SLOT(slot_updateMapSpriteName(int)));
    connect(this,SIGNAL(signal_choosedMapSpriteChanged(int)),this, SLOT(slot_updateMapSpriteSubClipListW(int)));
    connect(this,SIGNAL(signal_choosedMapSpriteChanged(int)),this,SLOT(slot_updateMapSpriteView(int)));
    //改动选中的子切片
    connect(this,SIGNAL(signal_choosedMapSpriteSubClipChanged(int)),this,SLOT(slot_updateMapSpriteSubClipListWCurRow(int)));
    connect(this,SIGNAL(signal_choosedMapSpriteSubClipChanged(int)),this,SLOT(slot_updateMapSpriteSubClipProperty(int)));



}

SpriteWindow::~SpriteWindow()
{
    delete ui;
    delete cmdInvoker_;
    delete clipScene_;
    delete mapSpriteScene_;
    delete spriteData_;
//    delete clipChoosedDialog;
//    clipChoosedDialog = NULL;
}

void SpriteWindow::initData() {
    //初始化
    intRealRow = -1;
    //放大缩小
    intZoomTimes = 1;
    ui->graphicsView->setZoomTimes(intZoomTimes);
    //修改标志
//    winTitle_ = this->windowTitle();
    winTitle_ = tr("精灵编辑窗口");
    setFileModify(false);
//    isModify = false;
    isSetMemento_ = false;
    choosedClip_ = NULL;
    choosedMapSprite_ = NULL;
    choosedMapSpriteSubClip_ = NULL;
    copyMapSpriteSubClip = NULL;

}

void SpriteWindow::initComponent()
{
    //清空控件值
    ui->clipListW->clear();
    ui->leName->setText("");
    ui->leImagePath->setText("");
    ui->leClipName->setText("");
    ui->clipX->setValue(0);
    ui->clipY->setValue(0);
    ui->clipW->setValue(0);
    ui->clipH->setValue(0);
    clipScene_->clear();

    ui->mapSpriteListW->clear();
    ui->mapSprite_SubClipListW->clear();
    ui->leMapSpriteName->setText("");
    ui->subClipPosX->setValue(0);
    ui->subClipPosY->setValue(0);
    ui->subClipFlipType->setCurrentIndex(0);
    ui->subClipRotate->setValue(0);
    ui->subClipScale->setValue(1.0);
    mapSpriteScene_->clear();

    this->setFocus();
}

//数据修改标志
void SpriteWindow::setFileModify(bool modify)
{
    isModify = modify;
    this->setWindowModified(isModify);
    if(isModify) {
        this->setWindowTitle(winTitle_+"*");
    }
    else {
        this->setWindowTitle(winTitle_);
    }
//    qDebug()<<(this->windowTitle());

}

void SpriteWindow::openNewSpriteFile()
{
    //先判断旧数据是否有保存
    if(isModify) {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Save:
            // Save was clicked
            saveSpriteFile();
            break;
        case QMessageBox::Discard:
            // Don't Save was clicked
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            return;
        default:
            // should never be reached
            break;
        }
    }

    delete spriteData_;
    spriteData_ = new SpriteData;
    ui->graphicsView->setSpriteData(spriteData_);
    ui->mapSpriteView->setSpriteData(spriteData_);
    taker_spriteData_->setSpriteData(spriteData_);

    //清空控件值
    initComponent();

    initData();

    IOData* io = IOData::getInstance();
    io->do_newSpriteFile();

    //保存一份数据
    save_Memento_ClipData();
}

void SpriteWindow::openSpriteFile(QString fileName)
{
    IOData* io = IOData::getInstance();
    spriteData_ = io->do_ReadSprite(fileName,false);
    //spriteData_地址变了,要重新设置
    ui->graphicsView->setSpriteData(spriteData_);
    ui->mapSpriteView->setSpriteData(spriteData_);
    taker_spriteData_->setSpriteData(spriteData_);

    updateSpriteProperty(spriteData_);

    //保存一份数据
    save_Memento_ClipData();
}

void SpriteWindow::updateSpriteProperty(SpriteData* spriteData)
{
    ui->leName->setText(spriteData->spriteName_);
    ui->leImagePath->setText(spriteData->imagePath_);
    this->changeImage();
    //添加切片
    for(int i = 0; i < spriteData->getClipList().size();i++) {
        this->addClip(spriteData->getClipList()[i]);
    }
    //添加地图精灵
    for(int i = 0; i < spriteData->getMapSpriteList().size();i++) {
        this->addMapSprite(spriteData->getMapSpriteList()[i]);
    }

}

void SpriteWindow::saveSpriteFile()
{
    IOData* io = IOData::getInstance();
    io->do_SaveSprite(spriteData_);
    setFileModify(false);
}

void SpriteWindow::saveSpriteFileAs()
{
    IOData* io = IOData::getInstance();
    io->do_SaveSprite_As(spriteData_);
    setFileModify(false);
}


void SpriteWindow::changeFileName()
{
   spriteData_->spriteName_ = ui->leName->text();
}

void SpriteWindow::changeImage()
{
    spriteData_->imagePath_ = ui->leImagePath->text();

    //删除原来的item
    clipScene_->clear();
//    QList<QGraphicsItem*> itemList = clipScene_->items();
//    QList<QGraphicsItem*>::iterator itemIndex;
//    for(itemIndex = itemList.begin();itemIndex != itemList.end();++itemIndex) {
//        QGraphicsItem* item = (QGraphicsItem*)*itemIndex;
//        clipScene_->removeItem(item);
//    }

    //添加新的item
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
    item->setPixmap(QPixmap(spriteData_->imagePath_));
    item->setPos(0,0);
    originalClipSceneRect = item->pixmap().rect();
    clipScene_->addItem(item);
    clipScene_->setSceneRect(originalClipSceneRect);

}

void SpriteWindow::addClip()
{
    QString clipName = QString("clip%1").arg(spriteData_->getClipList().size());
    Clip* clip = new Clip(clipName,ui->graphicsView->getClipRect());
    spriteData_->addClip(clip);

    choosedClip_ = clip;
    ui->clipListW->addItem(clip->clipName);
    ui->clipListW->setCurrentRow(spriteData_->getClipList().size() - 1);

    //自动添加 地图精灵(由切片生成)
    addMapSpriteFromClip(clip);
}

void SpriteWindow::addClip(Clip* clip)
{
//    spriteData_->addClip(clip);
//    choosedClip_ = clip;
    ui->clipListW->addItem(clip->clipName);
//    ui->clipListW->setCurrentRow(spriteData_->getClipList().size() - 1);
}

void SpriteWindow::delClip()
{

    Clip* clip = spriteData_->getClipList()[ui->clipListW->currentRow()];
    spriteData_->delClip(clip);

    //takeItem 比如删除第0行,先切到第1行(这时发的信号的第1行,但数据已经删掉了,会报错,所以标记realrow,不发送信号),再删除第0行
    ui->clipListW->takeItem(ui->clipListW->currentRow());
    intRealRow = ui->clipListW->currentRow();
    //发送 改变切片 信号
    emit_choosedClipChanged(intRealRow);
//    intRealRow = -1;
    delSubClipFromClip(clip);

    qDebug("delClip");
}

void SpriteWindow::setClip()
{
    QString name = ui->leClipName->text();
    if(isRepeatClipName(name) && ui->leClipName->hasFocus()) {
        QMessageBox msgBox;
        msgBox.setText(tr("有重名"));
        msgBox.exec();
        return;
    }
    choosedClip_->clipName = ui->leClipName->text();
    choosedClip_->clipRect.setX(ui->clipX->value());
    choosedClip_->clipRect.setY(ui->clipY->value());
    choosedClip_->clipRect.setWidth(ui->clipW->value());
    choosedClip_->clipRect.setHeight(ui->clipH->value());

    QListWidgetItem* item = ui->clipListW->currentItem();
    item->setText(ui->leClipName->text());
    ui->graphicsView->scene()->update();
}


void SpriteWindow::addLotClip()
{
    QList<QGraphicsItem*> item = clipScene_->items();
//    item[0]
    int col = item[0]->boundingRect().width()/intClipW;
    int row = item[0]->boundingRect().height()/intClipH;

    QRect rect;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {

            rect.setTopLeft(QPoint(j*intClipW,i*intClipH));
            rect.setBottomRight(QPoint((j+1)*intClipW,(i+1)*intClipH));
            ui->graphicsView->setClipRect(rect);
            this->addClip();
        }
    }
}

//槽-修改名字
void SpriteWindow::on_leName_editingFinished()
{
    Command *cmdChangeName = new CommandClipChangeName(this);
    cmdInvoker_->setCommand(cmdChangeName);
    cmdInvoker_->execCmd();
}

//槽-修改图像
void SpriteWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("选择图片"),imagePath,tr("图像(*.png *.jpg)") );


    if(!filePath.isEmpty()) {

        QFileInfo fileInfo(filePath);
        QDir dirfrom;
        QString fileName = dirfrom.relativeFilePath(fileInfo.absoluteFilePath());

        ui->leImagePath->setText(fileName);
        if(ui->leName->text().isEmpty()) {

            ui->leName->setText(fileInfo.baseName());
            this->changeFileName();
        }

        Command *cmdChangeImage = new CommandClipChangeImage(this);
        cmdInvoker_->setCommand(cmdChangeImage);
        cmdInvoker_->execCmd();
    }
}

//添加切片命令
void SpriteWindow::cmd_addClip()
{
    Command *cmdAddClip = new CommandClipAddClip(this);
    cmdInvoker_->setCommand(cmdAddClip);
    cmdInvoker_->execCmd();
}

//修改当前切片数据命令
void SpriteWindow::cmd_setClip()
{
    Command *cmdSetClip = new CommandClipSetClip(this);
    cmdInvoker_->setCommand(cmdSetClip);
    cmdInvoker_->execCmd();
}

//删除切片命令
void SpriteWindow::cmd_delClip()
{
    Command *cmdDelClip = new CommandClipDelClip(this);
    cmdInvoker_->setCommand(cmdDelClip);
    cmdInvoker_->execCmd();
}

//等比例命令
void SpriteWindow::cmd_addLotClip()
{
    Command *cmdAddLotClip = new CommandClipAddLotClip(this);
    cmdInvoker_->setCommand(cmdAddLotClip);
    cmdInvoker_->execCmd();
}

void SpriteWindow::initClipGraphicsView()
{
    clipScene_ = new QGraphicsScene;
    clipScene_->setSceneRect(0, 0, 800, 600);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setScene(clipScene_);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->graphicsView->setBackgroundRole(QPalette::Dark);

    ui->graphicsView->setSpriteData(spriteData_);
    ui->graphicsView->setSpriteWindown(this);
}


void SpriteWindow::on_clipListW_currentRowChanged(int currentRow)
{
    //发送 改变切片 信号
    //当intRealRow == -1时,就是做了删除操作,这时取到行未删除前的,不发送信号
    if(intRealRow == -1) {
        emit_choosedClipChanged(currentRow);
    }
    intRealRow = -1;
//    qDebug("signal row = %d",ui->clipListW->currentRow());

}


void SpriteWindow::on_toolButton_clicked()
{
    //设置 宽和高
//    QDialog*clipSetting =  ClipSettingDialog(this);
    ClipSettingDialog clipSetting(this);
    clipSetting.setFixedSize(200,150);
    clipSetting.exec();
}

///////选中切片更新 信号槽 ->start

//发送信号
void SpriteWindow::emit_choosedClipChanged(int index)
{
    emit signal_choosedClipChanged(index);

    if(index >= 0 && index < spriteData_->getClipList().size()) {
        choosedClip_ = spriteData_->getClipList()[index];
    }

}

void SpriteWindow::slot_updateClipListW(int index)
{
    ui->clipListW->setCurrentRow(index);
}

void SpriteWindow::slot_updateClipProperty(int index)
{
    if(index >= 0 && index < spriteData_->getClipList().size()) {
        Clip *clip = spriteData_->getClipList()[index];
        ui->leClipName->setText(clip->clipName);
        ui->clipX->setValue(clip->clipRect.x());
        ui->clipY->setValue(clip->clipRect.y());
        ui->clipW->setValue(clip->clipRect.width());
        ui->clipH->setValue(clip->clipRect.height());
    }
}

///////选中切片更新 信号槽 ->end


///////修改切片属性 ->start
void SpriteWindow::on_leClipName_textChanged(const QString &)
{
    if(!choosedClip_ || isSetMemento_) {
        return;
    }
    if(ui->leClipName->hasFocus()) {
        this->cmd_setClip();
    }
}

void SpriteWindow::on_clipX_valueChanged(int )
{
    if(!choosedClip_ || isSetMemento_) {
        return;
    }
    if(ui->clipX->hasFocus()) {
        this->cmd_setClip();
    }
}

void SpriteWindow::on_clipY_valueChanged(int)
{
    if(!choosedClip_ || isSetMemento_) {
        return;
    }
    if(ui->clipY->hasFocus()) {
        this->cmd_setClip();
    }
}

void SpriteWindow::on_clipW_valueChanged(int )
{
    if(!choosedClip_ || isSetMemento_) {
        return;
    }
    if(ui->clipW->hasFocus()) {
        this->cmd_setClip();
    }
}

void SpriteWindow::on_clipH_valueChanged(int )
{
    if(!choosedClip_ || isSetMemento_) {
        return;
    }
    if(ui->clipH->hasFocus()) {
        this->cmd_setClip();
    }
}
///////修改切片属性 ->end



///////等比例窗口类 ->start

ClipSettingDialog::ClipSettingDialog(SpriteWindow* spriteWindow)
{
    spriteWindow_ = spriteWindow;

    QGridLayout* layout = new QGridLayout(this);
    QLabel *labW = new QLabel(tr("宽:"),this);
    layout->addWidget(labW,0,0);
    leW = new QLineEdit(this);
    layout->addWidget(leW,0,1);

    QLabel *labH = new QLabel(tr("高:"),this);
    layout->addWidget(labH,1,0);
    leH = new QLineEdit(this);
    layout->addWidget(leH,1,1);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0,0,0,0);
    QPushButton *btnConfirm = new QPushButton(tr("确定"),this);
    btnLayout->addWidget(btnConfirm);
    QPushButton *btnCancel = new QPushButton(tr("取消"),this);
    btnLayout->addWidget(btnCancel);

    layout->addLayout(btnLayout,2,0,1,2);

    connect(btnConfirm,SIGNAL(clicked()),this,SLOT(on_btnConfirm_clicked()));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(close()));
}

void ClipSettingDialog::on_btnConfirm_clicked()
{
    spriteWindow_->intClipW = leW->text().toInt();
    spriteWindow_->intClipH = leH->text().toInt();
    spriteWindow_->cmd_addLotClip();
    close();
}
///////等比例窗口类 ->end


///////右键弹出菜单 ->start
void SpriteWindow::on_clipListW_customContextMenuRequested(const QPoint &pos)
{
    if(ui->clipListW->currentItem()) {
    QMenu menu(ui->clipListW);
    menu.addAction(actDelClip);
//    menu.popup(ui->clipListW->mapToGlobal(pos));
    menu.exec(ui->clipListW->mapToGlobal(pos));
//    menu.exec(event->globalPos());
//    qDebug("弹出菜单");
//    menu.setStatusTip();
    }
}
///////右键弹出菜单 ->end


///////放大缩小 ->start
void SpriteWindow::zoom_in()
{
    if(ui->tabSprite->currentWidget() == ui->tabClips) {
        QList<QGraphicsItem *> itemList = ui->graphicsView->items();
        if(itemList.size() == 0 || intZoomTimes >= 4)
            return;
        intZoomTimes ++;
        scale(intZoomTimes);
    }
}

void SpriteWindow::zoom_out()
{
    if(ui->tabSprite->currentWidget() == ui->tabClips) {
        QList<QGraphicsItem *> itemList = ui->graphicsView->items();
        if(itemList.size() == 0 || intZoomTimes <= 1)
            return;
        intZoomTimes --;
        scale(intZoomTimes);
    }
}

void SpriteWindow::zoom_original()
{
    intZoomTimes = 1;
    scale(intZoomTimes);
}

void SpriteWindow::scale(int times)
{
    QList<QGraphicsItem *> itemList = ui->graphicsView->items();
    for(int i = 0; i < itemList.size(); i++) {
        QGraphicsItem *item = itemList[i];
        item->setScale(times);
    }

    QRect rect = originalClipSceneRect;
    QRect newRect = QRect(rect.x()*times,rect.y()*times,rect.width()*times,rect.height()*times);
    clipScene_->setSceneRect(newRect);

//        qDebug("intZoomTimes = %d",intZoomTimes);
    ui->graphicsView->setZoomTimes(times);
}

///////放大缩小 ->end

////检查clip重名
bool SpriteWindow::isRepeatClipName(QString name)
{
    for(int i = 0; i < spriteData_->getClipList().size(); i++) {
        Clip* clip = spriteData_->getClipList().at(i);
        if(clip->clipName == name) {
            return true;
        }
    }
    return false;
}














/***************************
 *精灵方法
 **************************/

void SpriteWindow::addMapSprite()
{
//    MapSprite *mapSprite = new MapSprite;

}

void SpriteWindow::addMapSprite(MapSprite *mapSprite)
{
    ui->mapSpriteListW->addItem(mapSprite->mapSpriteName);
}

void SpriteWindow::delMapSprite()
{
}

void SpriteWindow::addMapSpriteSubClip(Clip *)
{
}

void SpriteWindow::delMapSpriteSubClip(Clip *)
{
}

void SpriteWindow::addMapSpriteFromClip(Clip *clip)
{
    MapSprite *mapSprite = new MapSprite(clip->clipName,clip);
    spriteData_->addMapSprite(mapSprite);

    ui->mapSpriteListW->addItem(mapSprite->mapSpriteName);
}

void SpriteWindow::delSubClipFromClip(Clip *clip)
{
    QList<MapSprite*> spriteList = spriteData_->getMapSpriteList();

    //    int index = -1;
    //删除子切片
//    QList<int> delIndexList;
    for(int i = 0; i < spriteList.size();i++) {
//        delIndexList.clear();
        MapSprite* mapSprite = spriteList.at(i);
        int size = mapSprite->getSubClipList().size();
        for(int j = size - 1; j >= 0; j--) {
            MapSpriteSubClip* subClip = mapSprite->getSubClipList().at(j);

            if(subClip->clip->clipName == clip->clipName) {
                mapSprite->delMapSpriteSubClip(subClip);
//                //添加index
//                delIndexList.append(j);
            }
        }
//        //根据index删除相应的子切片
//        for(int k = 0; k < delIndexList.size(); k++) {

//        }
    }


    //    for(int i = 0; i < spriteList.size();i++) {
    //        if(spriteList[i]->getSubClipList().size() == 1) {
    //            MapSpriteSubClip* subClip = spriteList[i]->getSubClipList()[0];
    //            if(clip->clipName.compare(subClip->clip->clipName) == 0) {
    //                spriteData_->delMapSprite(spriteList[i]);
    //                ui->mapSpriteListW->takeItem(i);
    //                return;
    //            }
    //        }
    //    }
}

void SpriteWindow::emit_choosedMapSpriteChanged(int index)
{
    if(index >= 0 && index < spriteData_->getMapSpriteList().size()) {
        choosedMapSprite_ = spriteData_->getMapSpriteList()[index];
    }
    emit signal_choosedMapSpriteChanged(index);
//    qDebug("changeMapSprite");
}

void SpriteWindow::emit_choosedMapSpriteSubClipChanged(int index)
{
    if(index >= 0 && index < choosedMapSprite_->getSubClipList().size()) {
        choosedMapSpriteSubClip_ = choosedMapSprite_->getSubClipList()[index];
    }
    emit signal_choosedMapSpriteSubClipChanged(index);
//    qDebug("changeSubClip");
}

//选中精灵改变 -->start
void SpriteWindow::slot_updateMapSpriteName(int )
{
    ui->leMapSpriteName->setText(choosedMapSprite_->mapSpriteName);
}

void SpriteWindow::slot_updateMapSpriteSubClipListW(int )
{
    ui->mapSprite_SubClipListW->clear();
    for(int i = 0; i < choosedMapSprite_->getSubClipList().size(); i++) {
        MapSpriteSubClip* subClip = choosedMapSprite_->getSubClipList()[i];
        ui->mapSprite_SubClipListW->addItem(subClip->clip->clipName);
    }
    ui->mapSprite_SubClipListW->setCurrentRow(0);
}

void SpriteWindow::slot_updateMapSpriteView(int )
{
    mapSpriteScene_->clear();

    for(int i = 0; i < choosedMapSprite_->getSubClipList().size(); i++) {
        MapSpriteSubClip* subClip = choosedMapSprite_->getSubClipList()[i];
        QGraphicsPixmapItem* item = createPixmapItem(subClip);
        item->setFlag(QGraphicsItem::ItemIsMovable);
        mapSpriteScene_->addItem(item);
//        qDebug("z is %f",item->zValue());
    }
    ui->mapSpriteView->scene()->update();
//    qDebug("size = %d",mapSpriteScene_->items().size());
}
//选中精灵改变 -->end

//选中精灵子切片改变 -->start
void SpriteWindow::slot_updateMapSpriteSubClipProperty(int )
{
//    QString name = QString("切片名字:%1").arg(choosedMapSpriteSubClip_->clip->clipName);
//    ui->subClipPosX->setMinimum(-choosedMapSpriteSubClip_->clip->clipRect.width());
//    ui->subClipPosX->setMaximum(0);
//    ui->subClipPosY->setMinimum(-choosedMapSpriteSubClip_->clip->clipRect.height());
//    ui->subClipPosY->setMaximum(0);

    ui->labSubClipName->setText(choosedMapSpriteSubClip_->clip->clipName);
    ui->subClipPosX->setValue(choosedMapSpriteSubClip_->position.x());
    ui->subClipPosY->setValue(choosedMapSpriteSubClip_->position.y());
    ui->subClipFlipType->setCurrentIndex(choosedMapSpriteSubClip_->intFlipType);
    ui->subClipRotate->setValue(choosedMapSpriteSubClip_->rotate);
    ui->subClipScale->setValue(choosedMapSpriteSubClip_->scale);
    ui->subClipAnchorX->setValue(choosedMapSpriteSubClip_->anchor.x());
    ui->subClipAnchorY->setValue(choosedMapSpriteSubClip_->anchor.y());
}

void SpriteWindow::slot_updateMapSpriteSubClipListWCurRow(int row)
{
    ui->mapSprite_SubClipListW->setCurrentRow(row);
    ui->mapSpriteView->setChoosedItemIndex(row);
}

//选中精灵子切片改变 -->end

void SpriteWindow::on_mapSpriteListW_currentRowChanged(int currentRow)
{
    if(intRealRow == -1) {
        emit_choosedMapSpriteChanged(currentRow);
        //第一次切片到精灵切换时 不显示选中的,所以加这行
//        ui->mapSpriteListW->setCurrentRow(currentRow);
    }
    intRealRow = -1;
}

void SpriteWindow::initMapSpriteGraphicsView()
{
    mapSpriteScene_ = new QGraphicsScene;
    mapSpriteScene_->setSceneRect(0, 0, 800, 400);

    ui->mapSpriteView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->mapSpriteView->setScene(mapSpriteScene_);
    ui->mapSpriteView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->mapSpriteView->setBackgroundRole(QPalette::Dark);

    QPoint origin = QPoint(ui->leOriginX->text().toInt(),ui->leOriginY->text().toInt());
    ui->mapSpriteView->setOriginPoint(origin);
    ui->mapSpriteView->setSpriteData(spriteData_);
    ui->mapSpriteView->setSpriteWindown(this);
}

void SpriteWindow::on_mapSprite_SubClipListW_currentRowChanged(int currentRow)
{
    if(intRealRow == -1) {
        emit_choosedMapSpriteSubClipChanged(currentRow);
    }
    intRealRow = -1;
}

void SpriteWindow::on_leOriginX_textChanged(const QString &arg1)
{
    QPoint origin = QPoint(arg1.toInt(),ui->leOriginY->text().toInt());
    ui->mapSpriteView->setOriginPoint(origin);
}

void SpriteWindow::on_leOriginY_textChanged(const QString &arg1)
{
    QPoint origin = QPoint(ui->leOriginX->text().toInt(),arg1.toInt());
    ui->mapSpriteView->setOriginPoint(origin);
}

QGraphicsPixmapItem * SpriteWindow::createPixmapItem(MapSpriteSubClip *subClip)
{
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem;
    QPixmap clipPixmap;
    QImage image;
    image.load(spriteData_->imagePath_);
    QImage clipImage = image.copy(subClip->clip->clipRect);
    clipPixmap.convertFromImage(clipImage);
    item->setPixmap(clipPixmap);
//    QPixmap pixMap(spriteData_->imagePath_);
//    item->setPixmap(pixMap.copy(subClip->clip->clipRect));
//    QPoint origin = ui->mapSpriteView->getOriginPoint();
    //设置属性
    setPixmapItemProperty(item,subClip);
//    item->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

    return item;
}

void SpriteWindow::setPixmapItemFlipType(QGraphicsPixmapItem* item,MapSpriteSubClip* subClip)
{
    QPixmap clipPixmap;
    QImage image(spriteData_->imagePath_);
    QImage clipImage = image.copy(subClip->clip->clipRect);
//    QImage clipImage = item->pixmap().toImage();
    int intFlipType = subClip->intFlipType;
    //无翻转
    if(intFlipType == 0) {
        clipPixmap.convertFromImage(clipImage.mirrored(false,false));
    }
    //水平翻转
    else if(intFlipType == 1) {
        clipPixmap.convertFromImage(clipImage.mirrored(true,false));
    }
    //垂直翻转
    else if(intFlipType == 2) {
        clipPixmap.convertFromImage(clipImage.mirrored(false,true));
    }
    //水平+垂直
    else if(intFlipType == 3) {
        clipPixmap.convertFromImage(clipImage.mirrored(true,true));
    }
    item->setPixmap(clipPixmap);
    item->update(item->boundingRect());
}

void SpriteWindow::cmd_setMapSpriteSubClip()
{
    Command* cmdsetMapSpriteSubClip = new CommandSpriteSetSubClip(this);
    cmdInvoker_->setCommand(cmdsetMapSpriteSubClip);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_exec_setMapSpriteSubClip()
{
    //修改数据
    choosedMapSpriteSubClip_->intFlipType = ui->subClipFlipType->currentIndex();
    int x = ui->subClipPosX->value();
    int y = ui->subClipPosY->value();
    //只有一个切片暂时先去掉此功能
    choosedMapSpriteSubClip_->position = QPoint(x,y);
    choosedMapSpriteSubClip_->rotate = ui->subClipRotate->value();
    choosedMapSpriteSubClip_->scale = ui->subClipScale->value();
    float anchorX = ui->subClipAnchorX->value();
    float anchorY = ui->subClipAnchorY->value();
    choosedMapSpriteSubClip_->anchor = QPointF(anchorX,anchorY);

    //修改视图(界面)
    int index = ui->mapSprite_SubClipListW->currentRow();

//    QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)mapSpriteScene_->items()[index];
    QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)ui->mapSpriteView->getItemAt(index);
    setPixmapItemProperty(item,choosedMapSpriteSubClip_);
}

void SpriteWindow::setMapSpriteSubClipPos(QPoint pos)
{
    ui->subClipPosX->setValue(pos.x());
    ui->subClipPosY->setValue(pos.y());
    cmd_setMapSpriteSubClip();
}

void SpriteWindow::setPixmapItemProperty(QGraphicsPixmapItem* item,MapSpriteSubClip *subClip)
{
    QPoint origin = ui->mapSpriteView->getOriginPoint();
    //设置属性
    setAnchor(item,subClip->anchor);
    item->setPos(origin.x()+subClip->position.x(),origin.y() + subClip->position.y());
    item->setRotation(subClip->rotate);
    item->setScale(subClip->scale);
    setPixmapItemFlipType(item,subClip);

    mapSpriteScene_->update();
    ui->mapSpriteView->repaint();
//    ui->mapSpriteView->scene()->update();
}

void SpriteWindow::on_subClipPosX_valueChanged(int )
{
    if(!choosedMapSpriteSubClip_) {
        return;
    }
    if(ui->subClipPosX->hasFocus()) {
        //只有一个切片，暂时先去掉移动
       this->cmd_setMapSpriteSubClip();
    }
}

void SpriteWindow::on_subClipPosY_valueChanged(int )
{
    if(!choosedMapSpriteSubClip_) {
        return;
    }
    if(ui->subClipPosY->hasFocus()) {
        this->cmd_setMapSpriteSubClip();
    }
}

void SpriteWindow::on_subClipFlipType_currentIndexChanged(int )
{
    if(!choosedMapSpriteSubClip_) {
        return;
    }
#ifdef Q_WS_WIN
    //window
    if(ui->subClipFlipType->hasFocus()) {
        this->cmd_setMapSpriteSubClip();
    }
#endif
#ifdef Q_WS_MAC
    //mac
    if(ui->subClipFlipType->view()->hasFocus()) {
        this->cmd_setMapSpriteSubClip();
    }
#endif
}

void SpriteWindow::on_subClipRotate_valueChanged(int )
{
    if(!choosedMapSpriteSubClip_) {
        return;
    }
    if(ui->subClipRotate->hasFocus()) {
        this->cmd_setMapSpriteSubClip();
    }
}

void SpriteWindow::on_subClipScale_valueChanged(double )
{
    if(!choosedMapSpriteSubClip_) {
        return;
    }
    if(ui->subClipScale->hasFocus()) {
        this->cmd_setMapSpriteSubClip();
    }
}

void SpriteWindow::save_Memento_ClipData()
{
    if(taker_spriteData_ != NULL) {
        taker_spriteData_->save_Memento_ClipData();
    }
}

void SpriteWindow::undo_Memento_ClipData()
{
    if(taker_spriteData_ != NULL) {
        isSetMemento_ = true;
        taker_spriteData_->undo_Memento_ClipData();
        //TODO 刷新界面
        initComponent();
        updateSpriteProperty(spriteData_);
        isSetMemento_ = false;
    }
}

void SpriteWindow::redo_Memento_ClipData()
{
    if(taker_spriteData_ != NULL) {
        isSetMemento_ = true;
        taker_spriteData_->redo_Memento_ClipData();
        //TODO 刷新界面
        initComponent();
        updateSpriteProperty(spriteData_);
        isSetMemento_ = false;
    }
}

/////////////切片选择窗口
ClipChoosedForm::ClipChoosedForm(SpriteWindow* spriteWindow)
{
//    this->spriteData_ = spriteData;
//    pixmap_ = new QPixmap(spriteData_->imagePath_);
//    intChoosedClipIndex = -1;
//    setMouseTracking(true);

    QGridLayout *layout = new QGridLayout(this);;
    layout->setContentsMargins(0, 0, 0, 0);

    clipLabel = new ClipChoosedLabel(this,spriteWindow);
//    clipLabel->spriteData_ = spriteData;
//    clipLabel->intChoosedClipIndex = -1;
//    clipLabel->setMouseTracking(true);
    QImage image(spriteWindow->getSpriteDate()->imagePath_);
    clipLabel->setPixmap(QPixmap::fromImage(image));

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(clipLabel);

    layout->addWidget(scrollArea, 0, 0, 1, 1);
}

ClipChoosedForm::~ClipChoosedForm()
{
//    delete pixmap_;
//    pixmap_ = NULL;
    delete clipLabel;
    clipLabel = NULL;
    delete scrollArea;
    scrollArea = NULL;
}


ClipChoosedLabel::ClipChoosedLabel(ClipChoosedForm *form,SpriteWindow *spriteWindow)
{
    this->form_ = form;
    this->spriteWin_ = spriteWindow;
    this->spriteData_ = spriteWindow->getSpriteDate();
    intChoosedClipIndex = -1;
    setMouseTracking(true);
}

void ClipChoosedLabel::paintEvent(QPaintEvent *)
{
//    QPolygon converPolygon;
//    ClipChoosedLabel::paintEvent(event);
//    this->paintEvent(event);
    QPainter painter(this);
    //画切片
    painter.drawPixmap(0,0,*pixmap());
    painter.setPen(Qt::green);
    QList<Clip*> clipList = spriteData_->getClipList();
    QList<Clip*>::iterator clipIndex;
    for(clipIndex = clipList.begin();clipIndex != clipList.end(); ++clipIndex) {
        Clip* clip = (Clip*)*clipIndex;
        painter.drawRect(clip->clipRect);
    }

    //选中的切片
    if(intChoosedClipIndex != -1) {
        painter.setPen(Qt::red);
        Clip* choosedClip = spriteData_->clipList_[intChoosedClipIndex];
        painter.drawRect(choosedClip->clipRect);
    }
}

void ClipChoosedLabel::mouseReleaseEvent(QMouseEvent *)
{
    spriteWin_->setAddSubClipIndex(intChoosedClipIndex);
    spriteWin_->cmd_addMapSpriteSubClip();
    form_->close();
}

void ClipChoosedLabel::mouseMoveEvent(QMouseEvent *event)
{
    intChoosedClipIndex = getChoosedClip(event->pos());
    update();
}

int ClipChoosedLabel::getChoosedClip(QPoint point)
{
    for(int i = 0; i < spriteData_->getClipList().size(); i++) {
        Clip* clip = spriteData_->clipList_[i];
        if(point.x() >= clip->clipRect.x() && point.x() <= clip->clipRect.x()+clip->clipRect.width()
                && point.y() >= clip->clipRect.y() && point.y() <= clip->clipRect.y() + clip->clipRect.height()) {
            return i;
        }
    }
    return -1;
}


///////子切片工具栏操作
void SpriteWindow::on_btnAdd_clicked()
{
    ClipChoosedForm clipChoosedDialog(this);
    clipChoosedDialog.exec();
}

void SpriteWindow::on_btnDel_clicked()
{
    cmd_delMapSpriteSubClip();
}

void SpriteWindow::on_btnUp_clicked()
{
    cmd_upMapSpriteSubClip();
}

void SpriteWindow::on_btnDown_clicked()
{
    cmd_downMapSpriteSubClip();
}

void SpriteWindow::on_btnAddMapSprite_clicked()
{
    cmd_AddMapSprite();
}

void SpriteWindow::on_btnDelMapSprite_clicked()
{
    cmd_DelMapSprite();
}

void SpriteWindow::on_btnCopyMapSprite_clicked()
{
    cmd_CopyMapSprite();
}

void SpriteWindow::on_btnCopy_clicked()
{
    cmd_CopyMapSpriteSubClip();
}

void SpriteWindow::on_btnPaste_clicked()
{
    cmd_PasteMapSpriteSubClip();
}
///////////精灵窗口的命令
void SpriteWindow::cmd_addMapSpriteSubClip()
{
    Command *cmdAddMapSpriteSubClip = new CommandSpriteAddSubClip(this);
    cmdInvoker_->setCommand(cmdAddMapSpriteSubClip);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_exec_addMapSpriteSubClip()
{
    if(intAddSubClipIndex != -1) {
        //添加数据
        Clip *clip = spriteData_->clipList_.at(intAddSubClipIndex);
        MapSpriteSubClip *subClip = new MapSpriteSubClip(clip);
        choosedMapSprite_->addMapSpriteSubClip(subClip);

        //修改界面
        ui->mapSprite_SubClipListW->addItem(subClip->clip->clipName);
        QGraphicsPixmapItem* item = createPixmapItem(subClip);
        mapSpriteScene_->addItem(item);
    }
}

void SpriteWindow::cmd_delMapSpriteSubClip()
{
    Command *cmdDelMapSpriteSubClip = new CommandSpriteDelSubClip(this);
    cmdInvoker_->setCommand(cmdDelMapSpriteSubClip);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_exec_delMapSpriteSubClip()
{
    //删除数据
    choosedMapSprite_->delMapSpriteSubClip(choosedMapSpriteSubClip_);

    slot_updateMapSpriteView(ui->mapSprite_SubClipListW->currentRow());
    //takeItem 比如删除第0行,先切到第1行(这时发的信号的第1行,但数据已经删掉了,会报错,所以标记realrow,不发送信号),再删除第0行
    ui->mapSprite_SubClipListW->takeItem(ui->mapSprite_SubClipListW->currentRow());
    intRealRow = ui->mapSprite_SubClipListW->currentRow();

    //发送 改变切片 信号
    emit_choosedMapSpriteSubClipChanged(intRealRow);
//    intRealRow = -1;
    //    delMapSpriteFromClip(clip);
}

void SpriteWindow::cmd_upMapSpriteSubClip()
{
    Command *cmdUpMapSpriteSubClip = new CommandSpriteUpSubClip(this);
    cmdInvoker_->setCommand(cmdUpMapSpriteSubClip);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_downMapSpriteSubClip()
{
    Command *cmdDownMapSpriteSubClip = new CommandSpriteDownSubClip(this);
    cmdInvoker_->setCommand(cmdDownMapSpriteSubClip);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_exec_upMapSpriteSubClip()
{
    int index = ui->mapSprite_SubClipListW->currentRow();
    if(index <= 0)
        return;
    //修改数据
    MapSpriteSubClip *subClip = choosedMapSpriteSubClip_->getMapSpriteSubClipClone();
    choosedMapSprite_->delMapSpriteSubClip(choosedMapSpriteSubClip_);
    choosedMapSprite_->insertMapSpriteSubClip(subClip,index-1);
    //修改界面
    slot_updateMapSpriteSubClipListW(index);
    slot_updateMapSpriteView(index);
    emit_choosedMapSpriteSubClipChanged(index - 1);
//    ui->mapSprite_SubClipListW->setCurrentRow(index - 1);
}

void SpriteWindow::cmd_exec_downMapSpriteSubClip()
{
    int index = ui->mapSprite_SubClipListW->currentRow();
    if(index < 0 || index >= choosedMapSprite_->getSubClipList().size() - 1)
        return;
    //修改数据
    MapSpriteSubClip *subClip = choosedMapSpriteSubClip_->getMapSpriteSubClipClone();
    choosedMapSprite_->delMapSpriteSubClip(choosedMapSpriteSubClip_);
    choosedMapSprite_->insertMapSpriteSubClip(subClip,index+1);
    //修改界面
    slot_updateMapSpriteSubClipListW(index);
    slot_updateMapSpriteView(index);
    emit_choosedMapSpriteSubClipChanged(index + 1);
//    ui->mapSprite_SubClipListW->setCurrentRow(index + 1);
}

void SpriteWindow::cmd_changeMapSpriteName()
{
    Command *cmdChangeMapSpriteName = new CommandSpriteChangeName(this);
    cmdInvoker_->setCommand(cmdChangeMapSpriteName);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_exec_changeMapSpriteName()
{
    QString name = ui->leMapSpriteName->text();
    if(isRepeatMapSpriteName(name) && ui->leMapSpriteName->hasFocus()) {
        QMessageBox mess;
        mess.setText(tr("有重名"));
        mess.exec();
        return;
    }
    //数据
    choosedMapSprite_->mapSpriteName = ui->leMapSpriteName->text();
    //界面
    QListWidgetItem* item = ui->mapSpriteListW->currentItem();
    item->setText(ui->leMapSpriteName->text());
}

void SpriteWindow::cmd_AddMapSprite()
{
    Command *cmdAddMapSprite = new CommandSpriteAddMapSprite(this);
    cmdInvoker_->setCommand(cmdAddMapSprite);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_DelMapSprite()
{
    Command *cmdDelMapSprite = new CommandSpriteDelMapSprite(this);
    cmdInvoker_->setCommand(cmdDelMapSprite);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_exec_AddMapSprite()
{
    //数据
    MapSprite* mapSprite = new MapSprite;
    mapSprite->mapSpriteName = "new_"+ui->leMapSpriteName->text();
    spriteData_->addMapSprite(mapSprite);

    //界面
    ui->mapSpriteListW->addItem(mapSprite->mapSpriteName);
    ui->mapSpriteListW->setCurrentRow(ui->mapSpriteListW->count() - 1);

}

void SpriteWindow::cmd_exec_DelMapSprite()
{
    if(choosedMapSprite_) {
        //数据
        spriteData_->delMapSprite(choosedMapSprite_);

        //界面
        //takeItem 比如删除第0行,先切到第1行(这时发的信号的第1行,但数据已经删掉了,会报错,所以标记realrow,不发送信号),再删除第0行
         ui->mapSpriteListW->takeItem(ui->mapSpriteListW->currentRow());
        intRealRow = ui->mapSpriteListW->currentRow();

        //发送 改变切片 信号
        emit_choosedMapSpriteChanged(intRealRow);
    }
}


void SpriteWindow::on_leMapSpriteName_textChanged(const QString &)
{
    if(!choosedMapSprite_ || isSetMemento_) {
        return;
    }
    if(ui->leMapSpriteName->hasFocus()) {
        this->cmd_changeMapSpriteName();
    }
}

void SpriteWindow::cmd_CopyMapSprite()
{
    Command *cmdCopyMapSprite = new CommandSpriteCopyMapSprite(this);
    cmdInvoker_->setCommand(cmdCopyMapSprite);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_CopyMapSpriteSubClip()
{
    Command *cmdCopyMapSpriteSubClip = new CommandSpriteCopySubClip(this);
    cmdInvoker_->setCommand(cmdCopyMapSpriteSubClip);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_PasteMapSpriteSubClip()
{
    Command *cmdPasteMapSpriteSubClip = new CommandSpritePasteSubClip(this);
    cmdInvoker_->setCommand(cmdPasteMapSpriteSubClip);
    cmdInvoker_->execCmd();
}

void SpriteWindow::cmd_exec_CopyMapSprite()
{
    if(choosedMapSprite_) {
        //数据
        MapSprite* mapSprite = choosedMapSprite_->getMapSpriteClone();
        mapSprite->mapSpriteName = ui->leMapSpriteName->text()+tr("_copy");
        spriteData_->addMapSprite(mapSprite);

        //界面
        ui->mapSpriteListW->addItem(mapSprite->mapSpriteName);
        ui->mapSpriteListW->setCurrentRow(ui->mapSpriteListW->count() - 1);
    }
}

void SpriteWindow::cmd_exec_CopyMapSpriteSubClip()
{
    if(choosedMapSpriteSubClip_) {
        copyMapSpriteSubClip = choosedMapSpriteSubClip_->getMapSpriteSubClipClone();
    }
}

void SpriteWindow::cmd_exec_PasteMapSpriteSubClip()
{
    if(copyMapSpriteSubClip) {
        //添加数据
        choosedMapSprite_->addMapSpriteSubClip(copyMapSpriteSubClip);

        //修改界面
        ui->mapSprite_SubClipListW->addItem(copyMapSpriteSubClip->clip->clipName);
        QGraphicsPixmapItem* item = createPixmapItem(copyMapSpriteSubClip);
        mapSpriteScene_->addItem(item);
    }
}

//检查重名
bool SpriteWindow::isRepeatMapSpriteName(QString name)
{
    for(int i = 0; i < spriteData_->getMapSpriteList().size(); i++) {
        MapSprite* mapSprite = spriteData_->getMapSpriteList().at(i);
        if(mapSprite->mapSpriteName == name) {
            return true;
        }
    }
    return false;
}

void SpriteWindow::changeEditAreaBackGround()
{
    QColorDialog chooseColorDialog(Qt::darkGray, this);
    int rec = chooseColorDialog.exec();
    if(rec == QDialog::Accepted) {
        QColor color = chooseColorDialog.selectedColor();

        if(ui->tabSprite->currentWidget() == ui->tabClips) {
            ui->graphicsView->setBackgroundBrush(QBrush(color));
            ui->graphicsView->setAutoFillBackground(true);
        }
        else if(ui->tabSprite->currentWidget() == ui->tabNormalSprite) {
            ui->mapSpriteView->setBackgroundBrush(QBrush(color));
            ui->mapSpriteView->setAutoFillBackground(true);
        }


    }

}

void SpriteWindow::on_subClipAnchorX_valueChanged(double )
{
    if(!choosedMapSpriteSubClip_) {
        return;
    }
    if(ui->subClipAnchorX->hasFocus()) {
        this->cmd_setMapSpriteSubClip();
    }
}

void SpriteWindow::on_subClipAnchorY_valueChanged(double )
{
    if(!choosedMapSpriteSubClip_) {
        return;
    }
    if(ui->subClipAnchorY->hasFocus()) {
        this->cmd_setMapSpriteSubClip();
    }
}
