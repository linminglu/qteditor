#include "spritedata.h"
#include <QPainter>
#include "utility.h"

using namespace Utility;

SpriteData::SpriteData()
{
//    spriteImage = NULL;
}

void SpriteData::addClip(Clip *clip)
{
    clipList_.append(clip);
}

QList<Clip *> SpriteData::getClipList()
{
    return clipList_;
}


Clip * SpriteData::getClipByName(QString clipName)
{
    for(int i = 0; i < clipList_.size(); i++) {
        Clip* clip = clipList_[i];
        if(clip->clipName == clipName) {
            return clip;
        }
    }
    return NULL;
}

Clip::Clip(){}

Clip::Clip(QString name, QRect rect)
{
    clipName = name;
    clipRect = rect;
}

void SpriteData::delClip(Clip *clip)
{
    clipList_.removeOne(clip);
}

void SpriteData::addMapSprite(MapSprite *sprite)
{
    mapSpriteList_.append(sprite);
}

void SpriteData::delMapSprite(MapSprite *sprite)
{
    mapSpriteList_.removeOne(sprite);
}

QList<MapSprite *> SpriteData::getMapSpriteList()
{
    return mapSpriteList_;
}

MapSprite::MapSprite()
{}

MapSprite::MapSprite(QString name, Clip *clip)
{
    mapSpriteName = name;
    MapSpriteSubClip* subClip = new MapSpriteSubClip(clip);
    subClipList_.append(subClip);
    choosedSpritePos = QPoint(0,0);
}

MapSprite * MapSprite::getMapSpriteClone()
{
    MapSprite* mapSprite = new MapSprite;
    mapSprite->mapSpriteName = this->mapSpriteName;
    for(int i = 0; i < this->subClipList_.size(); i++) {
        MapSpriteSubClip* subClip = subClipList_.at(i)->getMapSpriteSubClipClone();
        mapSprite->addMapSpriteSubClip(subClip);
    }
    return mapSprite;
}

QList<MapSpriteSubClip *> MapSprite::getSubClipList()
{
    return subClipList_;
}

void MapSprite::addMapSpriteSubClip(MapSpriteSubClip *subClip)
{
    subClipList_.append(subClip);
}

void MapSprite::delMapSpriteSubClip(MapSpriteSubClip *subClip)
{
    subClipList_.removeOne(subClip);
}

void MapSprite::insertMapSpriteSubClip(MapSpriteSubClip *subClip,int index)
{
    subClipList_.insert(index,subClip);
}

//生成图片
void MapSprite::setImage(const QImage &img)
{
    //暂时只处理一个切片
    if(subClipList_.size() > 0) {
        MapSpriteSubClip* subClip = subClipList_.at(0);
        QImage tempImage = img.copy(subClip->clip->clipRect);

        int intFlipType = subClip->intFlipType;
        //无翻转
        if(intFlipType == 0) {
            image = tempImage.mirrored(false,false);
        }
        //水平翻转
        else if(intFlipType == 1) {
            image = tempImage.mirrored(true,false);
        }
        //垂直翻转
        else if(intFlipType == 2) {
            image = tempImage.mirrored(false,true);
        }
        //水平+垂直
        else if(intFlipType == 3) {
            image = tempImage.mirrored(true,true);
        }

        //计算旋转和缩放后的rect大小
        QPointF anchor = subClip->anchor;
        float scale = subClip->scale;
        float rotate = subClip->rotate;
        //旋转后的rect
        QRect rotateAfterRect = rotateRect(image.rect(),QPointF(image.width()*anchor.x(),image.height()*(1-anchor.y())),rotate);
        //继续缩放后的rect
        QRect resultRect = QRect(0,0,rotateAfterRect.width()*scale,rotateAfterRect.height()*scale);

        QPointF beforePoint = QPointF(image.width()*anchor.x(),image.height()*(1-anchor.y()));
//        qDebug() << "before " << beforePoint;
        QPointF rateteAfterPoint = pointFromRotateRect(image.rect(),beforePoint,rotate);
        anchorOffsetPoint = QPointF(rateteAfterPoint.x()*scale,rateteAfterPoint.y()*scale);
//        qDebug() << "after " << rateteAfterPoint;
        //新建一张图片，在该图片上画图（做旋转，缩放等）
        QImage fixedImage(resultRect.size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&fixedImage);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(fixedImage.rect(), Qt::transparent);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//        //测试锚点，坐标系统未缩放，旋转前
//        painter.setPen(Qt::green);
//        painter.drawRect(QRect(anchorOffsetPoint.x() - 5,anchorOffsetPoint.y() - 5,10,10));

        //平移原点到中心点 坐标轴缩放，旋转
        painter.translate(QPointF(resultRect.width()/2,resultRect.height()/2));
        painter.scale(scale,scale);
        painter.rotate(rotate);
        //平移原点回来 此时的坐标轴是经过缩放和旋转的
        painter.translate(QPointF(-resultRect.width()/2,-resultRect.height()/2));
        //画在中心点上，图片的锚点还是左上角，所以相减居中，因为坐标轴是缩放和旋转的，所以画出来的图片是也是缩放和旋转的
        painter.drawImage(resultRect.width()/2 - image.size().width()/2,resultRect.height()/2 - image.size().height()/2,image);
//        //测试画设点的锚点
//        painter.setPen(Qt::red);
//        //在image在fixedImage的左下角
//        QPoint imageOriginPoint = QPoint(resultRect.width()/2 - image.size().width()/2,
//                                         resultRect.height()/2 + image.size().height()/2);
//        painter.drawRect(QRect(imageOriginPoint.x() - 4,imageOriginPoint.y() - 4,8,8));
//        //锚点是基于原点imageOriginPoint 记住坐标系统x→ y↓(正)
//        QPoint imageAnchor = QPoint(imageOriginPoint.x() + image.width()*anchor.x(),imageOriginPoint.y() - image.height()*anchor.y());
//        painter.setPen(Qt::blue);
//        painter.drawRect(QRect(imageAnchor.x() - 4,imageAnchor.y() - 4,8,8));
        //绘画结束
        painter.end();


        image = fixedImage;

    }
}

MapSpriteSubClip::MapSpriteSubClip()
{
    initProperty();
}

MapSpriteSubClip::MapSpriteSubClip(Clip *clip)
{
    this->clip = clip;
    initProperty();
}

MapSpriteSubClip *MapSpriteSubClip::getMapSpriteSubClipClone()
{
    MapSpriteSubClip* mapSpriteSubClip = new MapSpriteSubClip;
    mapSpriteSubClip->position = this->position;
    mapSpriteSubClip->intFlipType = this->intFlipType;
    mapSpriteSubClip->rotate = this->rotate;
    mapSpriteSubClip->scale = this->scale;

    mapSpriteSubClip->clip = new Clip(this->clip->clipName,this->clip->clipRect);

    return mapSpriteSubClip;
}

void MapSpriteSubClip::initProperty()
{
    //位置
    position = QPoint(0,0);
    //翻转类型 0---无翻转 0--水平 1--垂直
    intFlipType = 0;
    //缩放1.0--1倍
    scale = 1.0;
    //旋转[0,360]
    rotate = 0;
    //锚点0-1.0
    anchor = QPointF(0,1.0);
}


//memento
Memento_ClipData* SpriteData::getMemento_ClipData() {
    return new Memento_ClipData(this->spriteName_,this->imagePath_,this->clipList_);
}

void SpriteData::setMemento_ClipData(Memento_ClipData* memento_clipdata) {
    spriteName_ = memento_clipdata->spriteFileName_;
    imagePath_ = memento_clipdata->imagePath_;
    this->clipList_.clear();
    QList<Clip*> tmpClipList = memento_clipdata->clipList_;
    for(int i = 0; i < tmpClipList.size(); i++) {
        Clip* clip = new Clip(tmpClipList.at(i)->clipName,tmpClipList.at(i)->clipRect);
        this->clipList_.append(clip);
    }
}


