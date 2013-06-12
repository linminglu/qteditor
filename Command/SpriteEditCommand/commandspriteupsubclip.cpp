/********************************************************************
    created:    2011/07/21
    filename:   CommandSpriteUpSubClip.cpp
    author:     bqlf
    purpose:    精灵编辑中 上移(切片集合中)子切片 的命令
*********************************************************************/
#include "commandspriteupsubclip.h"

CommandSpriteUpSubClip::CommandSpriteUpSubClip(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

void CommandSpriteUpSubClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_upMapSpriteSubClip();
    }
}
