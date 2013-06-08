/********************************************************************
    created:    2011/07/21
    filename:   CommandSpriteAddSubClip.cpp
    author:     bqlf
    purpose:    精灵编辑中 修改精灵(切片集合中)子切片数据的命令
*********************************************************************/
#include "commandspritesetsubclip.h"

CommandSpriteSetSubClip::CommandSpriteSetSubClip(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

//CommandSpriteSetSubClip::~CommandSpriteSetSubClip()
//{

//}

void CommandSpriteSetSubClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_setMapSpriteSubClip();
    }
}
