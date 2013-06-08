/********************************************************************
    created:    2011/07/21
    filename:   CommandSpriteChangeName.cpp
    author:     bqlf
    purpose:    精灵编辑中 修改精灵名字的命令
*********************************************************************/
#include "commandspritechangename.h"

CommandSpriteChangeName::CommandSpriteChangeName(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

void CommandSpriteChangeName::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_changeMapSpriteName();
    }
}
