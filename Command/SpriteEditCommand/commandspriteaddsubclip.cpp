/********************************************************************
    created:    2011/07/21
    filename:   CommandSpriteAddSubClip.cpp
    author:     bqlf
    purpose:    精灵编辑中 添加精灵(切片集合中)子切片的命令
*********************************************************************/
#include "commandspriteaddsubclip.h"

CommandSpriteAddSubClip::CommandSpriteAddSubClip(SpriteWindow* receiver)
{
    this->receiver_ = receiver;
}

void CommandSpriteAddSubClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_addMapSpriteSubClip();
    }
}
