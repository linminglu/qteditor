/********************************************************************
    created:    2011/07/21
    filename:   CommandSpriteDownSubClip.cpp
    author:     bqlf
    purpose:    精灵编辑中 下移(切片集合中)子切片 命令
*********************************************************************/
#include "commandspritedownsubclip.h"

CommandSpriteDownSubClip::CommandSpriteDownSubClip(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

void CommandSpriteDownSubClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_downMapSpriteSubClip();
    }
}
