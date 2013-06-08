/********************************************************************
    created:    2011/07/21
    filename:   CommandClipSetClip.cpp
    author:     bqlf
    purpose:    切片编辑中 修改切片数据(clip<name,x,y,w,h>)的命令
*********************************************************************/
#include "commandclipsetclip.h"

CommandClipSetClip::CommandClipSetClip(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

CommandClipSetClip::~CommandClipSetClip()
{
//    delete receiver_;
}

void CommandClipSetClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->setClip();
        receiver_->save_Memento_ClipData();
        receiver_->setFileModify(true);
    }
}
