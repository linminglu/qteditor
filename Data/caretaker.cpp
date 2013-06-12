/********************************************************************
    created:    2011/08/03
    filename:   careTaker.cpp
    author:     bqlf
    purpose:    管理memento,保存和撤销
*********************************************************************/
#include "caretaker.h"

careTaker_SpriteData::careTaker_SpriteData(SpriteData* spriteData)
{
    spriteData_ = spriteData;
    intMementoIndx = -1;
}

careTaker_SpriteData::~careTaker_SpriteData()
{}

void careTaker_SpriteData::setSpriteData(SpriteData *spriteData)
{
    spriteData_ = spriteData;
}

void careTaker_SpriteData::save_Memento_ClipData()
{
    Memento_ClipData* pMemento = spriteData_->getMemento_ClipData();
    //到达保存上限
    if(intMementoIndx >= MAX_MEMENTO_Count) {
        //删除第一个
        memento_clipdata_list_.pop_front();
        intMementoIndx--;
    }

    memento_clipdata_list_.append(pMemento);
    intMementoIndx = memento_clipdata_list_.size() - 1;
//    qDebug("save memento size = %d",memento_clipdata_list_.size());
}

void careTaker_SpriteData::undo_Memento_ClipData()
{
    if(memento_clipdata_list_.size() > 0 && intMementoIndx > 0)
    {
        intMementoIndx--;
        Memento_ClipData* pMemento = memento_clipdata_list_.at(intMementoIndx);
//        memento_clipdata_list_.pop_back();
        spriteData_->setMemento_ClipData(pMemento);
//        qDebug("unsavememento size = %d",memento_clipdata_list_.size());
    }
    else {
        qDebug("no undo memento");
    }
}

void careTaker_SpriteData::redo_Memento_ClipData()
{
    if(intMementoIndx < memento_clipdata_list_.size() - 1)
    {
        intMementoIndx++;
        Memento_ClipData* pMemento = memento_clipdata_list_.at(intMementoIndx);
//        memento_clipdata_list_.pop_back();
        spriteData_->setMemento_ClipData(pMemento);
//        qDebug("unsavememento size = %d",memento_clipdata_list_.size());

    }
    else {
        qDebug("no redo memento");
    }
}

