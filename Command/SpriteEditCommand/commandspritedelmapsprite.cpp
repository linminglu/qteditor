#include "commandspritedelmapsprite.h"

CommandSpriteDelMapSprite::CommandSpriteDelMapSprite(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

void CommandSpriteDelMapSprite::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_DelMapSprite();
    }
}
