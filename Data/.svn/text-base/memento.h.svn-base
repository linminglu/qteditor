/********************************************************************
    created:    2011/08/03
    filename:   memento.h
    author:     bqlf
    purpose:    封装数据用于做撤销还原
*********************************************************************/
#ifndef MEMENTO_H
#define MEMENTO_H
#include "Data/spritedata.h"
#include "Data/mapdata.h"

class Clip;

class Memento_ClipData
{
    friend class SpriteData;
public:
    Memento_ClipData(QString spriteFileName,QString imagePath,QList<Clip*> clipList);
    ~Memento_ClipData();
private:
    QString spriteFileName_;
    QString imagePath_;
    QList<Clip*> clipList_;
};

#endif // MEMENTO_H
