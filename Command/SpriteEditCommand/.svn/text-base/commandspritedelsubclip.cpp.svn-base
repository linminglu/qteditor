/********************************************************************
    created:    2011/07/21
    filename:   CommandSpriteDelSubClip.cpp
    author:     bqlf
    purpose:    精灵编辑中 删除精灵(切片集合中)子切片的命令
*********************************************************************/
#include "commandspritedelsubclip.h"

CommandSpriteDelSubClip::CommandSpriteDelSubClip(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

void CommandSpriteDelSubClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_delMapSpriteSubClip();
    }
}
