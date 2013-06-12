#include "commandspritecopysubclip.h"

CommandSpriteCopySubClip::CommandSpriteCopySubClip(SpriteWindow* receiver)
{
    this->receiver_ = receiver;
}

void CommandSpriteCopySubClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_CopyMapSpriteSubClip();
    }
}
