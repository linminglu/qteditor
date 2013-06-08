#include "commandspriteaddmapsprite.h"

CommandSpriteAddMapSprite::CommandSpriteAddMapSprite(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

void CommandSpriteAddMapSprite::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_AddMapSprite();
    }
}


