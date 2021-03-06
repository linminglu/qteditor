﻿#ifndef SPRITEWINDOW_H
#define SPRITEWINDOW_H
#include <QtWidgets>
#include <QMainWindow>
#include <QGraphicsScene>
#include "Data/spritedata.h"
#include "Command/commandinvoker.h"
#include "Data/caretaker.h"
#include "MainWindow.h"
#include <QtWidgets>
#include<QtWidgets/QtWidgets>
#include <QMainWindow>
#include <QDialog>
#include <QLineEdit>
#include <QtGui>
#include <QtWidgets/QMessageBox>

class ClipSettingDialog;
class MainWindow;

namespace Ui {
    class SpriteWindow;
}

class SpriteWindow : public QMainWindow
{
    Q_OBJECT
    friend class ClipSettingDialog;
public:
    explicit SpriteWindow(QWidget *parent = 0);
    ~SpriteWindow();
    //打开文件
    void openNewSpriteFile();
    void openSpriteFile(QString fileName);
    void updateSpriteProperty(SpriteData* spriteData);
    //保存文件
    void saveSpriteFile();
    void saveSpriteFileAs();

    ////////已知spriteData,刷新各个界面
    void addClip(Clip* clip);
    void addMapSprite(MapSprite* mapSprite);

    //caretaker
    void save_Memento_ClipData();
    void undo_Memento_ClipData();
    void redo_Memento_ClipData();

    bool isSetMemento() {
        return isSetMemento_;
    }

    //打开修改标志
    void setFileModify(bool modify);
    //设置主窗口
    void setMainWindow(MainWindow* mainWindow) {
        mainWindow_ = mainWindow;
    }
    SpriteData* getSpriteDate() {
        return spriteData_;
    }

    //改变编辑区底色
    void changeEditAreaBackGround();
////////// 切片面板 ->start
    void changeFileName();
    void changeImage();
    void addClip();
    void delClip();
    void setClip();
    void addLotClip();
    void cmd_addClip();
    void cmd_addLotClip();//等比例切片生成多个切片
    //发送 当前切片修改 的信号
    void emit_choosedClipChanged(int index);
    //放大
    void zoom_in();
    //缩小
    void zoom_out();
    //实际大小
    void zoom_original();
    //检查重名
    bool isRepeatClipName(QString name);

////////// 切片面板 ->end







////////// 精灵面板 ->start

    void addMapSprite();
    void addMapSpriteSubClip(Clip* clip);
    void delMapSpriteSubClip(Clip* clip);
    void addMapSpriteFromClip(Clip* clip);
    //删除mapSprite中已删除的子切片
    void delSubClipFromClip(Clip* clip);
    void delMapSprite();

    void setMapSpriteSubClipPos(QPoint pos);
    //发送 当前精灵修改 的信号
    void emit_choosedMapSpriteChanged(int index);
    void emit_choosedMapSpriteSubClipChanged(int index);
    //命令
    void cmd_addMapSpriteSubClip();
    void cmd_setMapSpriteSubClip();
    void cmd_delMapSpriteSubClip();
    void cmd_upMapSpriteSubClip();
    void cmd_downMapSpriteSubClip();
    void cmd_changeMapSpriteName();
    void cmd_AddMapSprite();
    void cmd_DelMapSprite();
    void cmd_CopyMapSprite();
    void cmd_CopyMapSpriteSubClip();
    void cmd_PasteMapSpriteSubClip();
    //命令实际执行的方法
    void cmd_exec_addMapSpriteSubClip();
    void cmd_exec_setMapSpriteSubClip();
    void cmd_exec_delMapSpriteSubClip();
    void cmd_exec_upMapSpriteSubClip();
    void cmd_exec_downMapSpriteSubClip();
    void cmd_exec_changeMapSpriteName();
    void cmd_exec_AddMapSprite();
    void cmd_exec_DelMapSprite();
    void cmd_exec_CopyMapSprite();
    void cmd_exec_CopyMapSpriteSubClip();
    void cmd_exec_PasteMapSpriteSubClip();

    //检查重名
    bool isRepeatMapSpriteName(QString name);
    //由子切片生成item
    QGraphicsPixmapItem* createPixmapItem(MapSpriteSubClip* subClip);
    //设置item属性
    void setPixmapItemProperty(QGraphicsPixmapItem* item,MapSpriteSubClip* subClip);
    //item镜像
    void setPixmapItemFlipType(QGraphicsPixmapItem* item,MapSpriteSubClip* subClip);
    void setAddSubClipIndex(int index) {
        intAddSubClipIndex = index;
    }

////////// 精灵面板 ->end
protected:
private:
    //初始化变量
    void initData();
    //清空控件值
    void initComponent();
////////// 切片面板 ->start
    void initClipGraphicsView();
    //缩放底层操作
    void scale(int times);
////////// 切片面板 ->end

    MainWindow* mainWindow_;
    Ui::SpriteWindow *ui;
    CommandInvoker* cmdInvoker_;
    SpriteData* spriteData_;
    careTaker_SpriteData* taker_spriteData_;
//    ClipChoosedDialog *clipChoosedDialog;
    //修改标志
    bool isModify;
    bool isSetMemento_;
    QString winTitle_;
////////// 切片面板 ->start
    QGraphicsScene* clipScene_;
    QRect originalClipSceneRect;//保持这个用来放大缩小用
    Clip* choosedClip_;
    //等比例切割
    int intClipW,intClipH;
    //删除切片
    QAction* actDelClip;
    int intRealRow;
    //放大倍数
    int intZoomTimes;
////////// 切片面板 ->end




////////// 精灵面板 ->start
    void initMapSpriteGraphicsView();
    MapSprite* choosedMapSprite_;
    MapSpriteSubClip* choosedMapSpriteSubClip_;
    QGraphicsScene* mapSpriteScene_;
    QRect originalMapSpriteSceneRect;//保持这个用来放大缩小用
    int intAddSubClipIndex;//将添加的子切片
    MapSpriteSubClip* copyMapSpriteSubClip;//复制子切片的数据
////////// 精灵面板 ->end


//信号
signals:

////////// 切片面板 ->start
    //当前选中的切片改变 发出的signal
    void signal_choosedClipChanged(int index);
////////// 切片面板 ->end



////////// 切片面板 ->start
    //当前选中的精灵改变 发出的signal
    void signal_choosedMapSpriteChanged(int index);
    //选中子切片
    void signal_choosedMapSpriteSubClipChanged(int index);
////////// 切片面板 ->end

//槽
private slots:

////////// 切片面板 ->start
    //clipListW 收到choosedClipChanged 启动的槽
    void slot_updateClipListW(int index);
    //切片属性区域的Widget 收到choosedClipChanged 启动的槽
    void slot_updateClipProperty(int index);
    void on_clipListW_currentRowChanged(int currentRow);
    void on_leName_editingFinished();
    void on_pushButton_clicked();
    void cmd_setClip();
    void cmd_delClip();
    void on_toolButton_clicked();
    void on_leClipName_textChanged(const QString &);
    void on_clipX_valueChanged(int );
    void on_clipY_valueChanged(int arg1);
    void on_clipW_valueChanged(int arg1);
    void on_clipH_valueChanged(int arg1);
    void on_clipListW_customContextMenuRequested(const QPoint &pos);
////////// 切片面板 ->end

////////// 精灵面板 ->start
    //选中精灵改变
    void slot_updateMapSpriteName(int index);
    void slot_updateMapSpriteSubClipListW(int index);
    void slot_updateMapSpriteView(int index);
    //选中精灵子切片改变
    void slot_updateMapSpriteSubClipProperty(int index);
    //选中子切片,刷新子切片list的选中行
    void slot_updateMapSpriteSubClipListWCurRow(int row);

    void on_mapSpriteListW_currentRowChanged(int currentRow);
    void on_mapSprite_SubClipListW_currentRowChanged(int currentRow);
    void on_leOriginX_textChanged(const QString &arg1);
    void on_leOriginY_textChanged(const QString &arg1);
    void on_subClipPosX_valueChanged(int arg1);
    void on_subClipPosY_valueChanged(int arg1);
    void on_subClipFlipType_currentIndexChanged(int index);
    void on_subClipRotate_valueChanged(int arg1);
    void on_subClipScale_valueChanged(double arg1);
    void on_subClipAnchorX_valueChanged(double arg1);
    void on_subClipAnchorY_valueChanged(double arg1);

    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_leMapSpriteName_textChanged(const QString &arg1);
    void on_btnUp_clicked();
    void on_btnDown_clicked();
    void on_btnAddMapSprite_clicked();
    void on_btnDelMapSprite_clicked();
    void on_btnCopyMapSprite_clicked();
    void on_btnCopy_clicked();
    void on_btnPaste_clicked();
};

class ClipSettingDialog : public QDialog
{
    Q_OBJECT
public:
    ClipSettingDialog(SpriteWindow* spriteWindow);
    QLineEdit *leW;
    QLineEdit *leH;
private slots:
    //等比例窗口
    void on_btnConfirm_clicked();
private:
    SpriteWindow* spriteWindow_;
};

class ClipChoosedForm;
class ClipChoosedLabel : public QLabel
{
    Q_OBJECT
public:
    ClipChoosedLabel(ClipChoosedForm * form,SpriteWindow* spriteWindow);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int getChoosedClip(QPoint point);
//private:
    int intChoosedClipIndex;
    ClipChoosedForm *form_;
    SpriteWindow *spriteWin_;
    SpriteData *spriteData_;
};

class ClipChoosedForm : public QDialog
{
    Q_OBJECT
public:
    ClipChoosedForm(SpriteWindow* spriteWindow);
    ~ClipChoosedForm();
private:
    ClipChoosedLabel* clipLabel;
    QScrollArea *scrollArea;
//    SpriteData *spriteData_;
};


#endif // SPRITEWINDOW_H
