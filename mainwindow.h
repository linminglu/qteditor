#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include "mapEditor/mapwindow.h"

class SpriteWindow;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //新建地图
    void newMapFile();
    //打开地图文件
    void openMapFile();

    //新建精灵
    void newSpriteFile();
    //打开精灵文件
    void openSpriteFile();

    int intFileType;
//private signals:

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();

//    void on_actNewSprite_triggered();

    void on_actZoomIn_triggered();

    void on_actZoomOut_triggered();

    void on_actOriginal_triggered();

//    void on_actSave_triggered();

//    void on_actSaveAs_triggered();

//    void on_actOpenSprite_triggered();

    void on_actUndo_activated();

    void on_actRedo_triggered();

    void on_actImportSprite_triggered();

    void on_actMapEdit_triggered();

    void on_actDragMapSprite_triggered();

//    void on_actOpen_triggered();

    void on_actMapSet_triggered();

    void on_actShowGrid_triggered();

    void on_actPasteGrid_triggered();

    void on_actChangeColor_triggered();

    void on_actShowLayer_triggered();

    void on_actShowHistory_triggered();

    void on_actShowSprite_triggered();

    void on_actIsMoveChooseSprite_triggered();

    void on_actShowChooseSpriteRect_triggered();

    void on_actRefreshMap_triggered();

    void on_actPath_triggered();

    void on_actShowEvent_triggered();

    void on_actEvent_triggered();

    // setting actions:

    void on_actMapPathSetting_triggered();
    void on_actSpritePathSetting_triggered();
    void on_actImagePathSetting_triggered();
    void on_actSpriteConfigPathSetting_triggered();
    void on_actRestoresDefault_triggered();

    void on_actMapPathSetting_hovered();
    void on_actSpritePathSetting_hovered();
    void on_actImagePathSetting_hovered();
    void on_actSpriteConfigPathSetting_hovered();

    // new end:

   // void on_actPasteGridMidle_triggered();

    void on_actionPasteGridMiddle_triggered();

protected:
    void readPathSetting();
    void writePathSetting(const QString &inPath, const int &inType);

private:
    void connectSignalsAndSlots();
    MapWindow *createMapWin();
    SpriteWindow *createSprWin();
    MapWindow *mapWin_;
    SpriteWindow *sprWin_;
    QMdiSubWindow *subWin_mapWin;
    QMdiSubWindow *subWin_sprWin;
    Ui::MainWindow *ui;

};


//文件类型窗口sprite/map
class chooseFileType : public QDialog
{
    Q_OBJECT
public:
    chooseFileType(MainWindow* mainWindow);
    private slots:
    void on_itemDoubleClicked(QTreeWidgetItem * item, int column );
private:
    QTreeWidget *twFileList;
    MainWindow* mainWindow;
};
#endif // MAINWINDOW_H
