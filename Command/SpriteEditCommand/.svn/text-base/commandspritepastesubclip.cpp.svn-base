#include "commandspritepastesubclip.h"

CommandSpritePasteSubClip::CommandSpritePasteSubClip(SpriteWindow* receiver)
{
    this->receiver_ = receiver;
}

void CommandSpritePasteSubClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->cmd_exec_PasteMapSpriteSubClip();
    }
}
