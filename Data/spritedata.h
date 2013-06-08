#ifndef SPRITE_H
#define SPRITE_H

#include <QString>
#include <QRect>
#include <QPoint>
#include <QList>
#include <QImage>
#include "Data/memento.h"

class Clip;
class MapSprite;
class MapSpriteSubClip;
class Memento_ClipData;

class SpriteData
{
//    friend class Clip;
//    friend class MapSprite;
public:
    SpriteData();
    QList<Clip*> getClipList();
    QList<MapSprite*> getMapSpriteList();
    Clip* getClipByName(QString clipName);
    void addClip(Clip* clip);
    void delClip(Clip* clip);
    void addMapSprite(MapSprite* sprite);
    void delMapSprite(MapSprite* sprite);

    //memento
    Memento_ClipData* getMemento_ClipData();
    void setMemento_ClipData(Memento_ClipData* memento_clipdata);

    QString spriteName_;
    QString imagePath_;
    QList<Clip*> clipList_;
    QList<MapSprite*> mapSpriteList_;
    QImage spriteImage;
};

class Clip
{
public:
    Clip();
    Clip(QString name,QRect rect);
//private:
    QString clipName;
    QRect clipRect;
};

class MapSprite
{
public:
    MapSprite();
    MapSprite(QString name,Clip* clip);
    MapSprite* getMapSpriteClone();
    QList<MapSpriteSubClip*> getSubClipList();
    void addMapSpriteSubClip(MapSpriteSubClip* subClip);
    void delMapSpriteSubClip(MapSpriteSubClip* subClip);
    void insertMapSpriteSubClip(MapSpriteSubClip* subClip,int index);
    //生成图片
    void setImage(const QImage &img);
    //切片集合
    QList<MapSpriteSubClip*> subClipList_;
    //名字
    QString mapSpriteName;
    //图片
    QImage image;
    //锚点在新的坐标系统中的point
    QPointF anchorOffsetPoint;
    //选择精灵时的位置
    QPoint choosedSpritePos;
};

class MapSpriteSubClip
{
public:
    MapSpriteSubClip();
    MapSpriteSubClip(Clip* clip);
    MapSpriteSubClip *getMapSpriteSubClipClone();
    void initProperty();
    //原始切片
    Clip* clip;
    //位置
    QPoint position;
    //翻转类型 0---无翻转 1--水平 2--垂直 3--水平+垂直
    int intFlipType;
    //缩放1.0--1倍
    float scale;
    //旋转[0,360]
    float rotate;
    //锚点
    QPointF anchor;
};
#endif // SPRITE_H
