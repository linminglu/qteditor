/********************************************************************
    created:    2011/07/21
    filename:   CommandClipAddClip.cpp
    author:     bqlf
    purpose:    切片编辑中 添加切片数据(clip<name,x,y,w,h>)的命令
*********************************************************************/
#include "commandclipaddclip.h"

CommandClipAddClip::CommandClipAddClip(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

CommandClipAddClip::~CommandClipAddClip()
{
//    delete receiver_;
}

void CommandClipAddClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->addClip();
        receiver_->save_Memento_ClipData();
        receiver_->setFileModify(true);
    }
}
