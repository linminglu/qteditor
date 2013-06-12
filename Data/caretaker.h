/********************************************************************
    created:    2011/08/03
    filename:   careTaker.h
    author:     bqlf
    purpose:    管理memento,保存和撤销
*********************************************************************/
#ifndef CARETAKER_H
#define CARETAKER_H
#include "Data/spritedata.h"
#include "Data/mapdata.h"
#include "Data/memento.h"

class careTaker_SpriteData
{
public:
    careTaker_SpriteData(SpriteData* spriteData);
    ~careTaker_SpriteData();

    void setSpriteData(SpriteData* spriteData);
    void save_Memento_ClipData();
    void undo_Memento_ClipData();
    void redo_Memento_ClipData();
private:
    QList<Memento_ClipData*> memento_clipdata_list_;
    SpriteData* spriteData_;
    int intMementoIndx;
    const static int MAX_MEMENTO_Count = 20;
};

#endif // CARETAKER_H
