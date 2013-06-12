#include "commandspritecopymapsprite.h"

CommandSpriteCopyMapSprite::CommandSpriteCopyMapSprite(SpriteWindow* receiver)
{
    this->receiver_ = receiver;
}

void CommandSpriteCopyMapSprite::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_CopyMapSprite();
    }
}


