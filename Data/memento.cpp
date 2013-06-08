/********************************************************************
    created:    2011/08/03
    filename:   memento.cpp
    author:     bqlf
    purpose:    封装数据用于做撤销还原
*********************************************************************/
#include "memento.h"
#include <QDebug>
#include "Utility.h"
using namespace Utility;

Memento_ClipData::Memento_ClipData(QString spriteFileName,QString imagePath,QList<Clip*> clipList)
{
    this->spriteFileName_ = spriteFileName;
    this->imagePath_ = imagePath;
    for(int i = 0; i < clipList.size(); i++) {
        Clip* clip = new Clip(clipList.at(i)->clipName,clipList.at(i)->clipRect);
        this->clipList_.append(clip);
    }
//    qDebug("save-----------------------");
//    qDebug() << ("spritefilename = "+spriteFileName);
//    qDebug() << ("imagePath = "+imagePath);
//    qDebug("clip size = %d",this->clipList_.size());
//    for(int i = 0; i < this->clipList_.size(); i++) {
//        Clip* clip = this->clipList_.at(i);
//        qDebug() << ("clipname = "+clip->clipName);
//        qDebug() << ("cliprect = "+convertToStringFromRect(clip->clipRect));
//    }
//    qDebug("save-----------------------");
}
