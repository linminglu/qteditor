/********************************************************************
    created:    2011/07/21
    filename:   CommandClipDelClip.cpp
    author:     bqlf
    purpose:    切片编辑中 删除切片数据(clip<name,x,y,w,h>)的命令
*********************************************************************/
#include "commandclipdelclip.h"

CommandClipDelClip::CommandClipDelClip(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

CommandClipDelClip::~CommandClipDelClip()
{
//    delete receiver_;
}

void CommandClipDelClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->delClip();
        receiver_->save_Memento_ClipData();
        receiver_->setFileModify(true);
    }
}
