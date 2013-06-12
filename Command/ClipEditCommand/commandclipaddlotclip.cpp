/********************************************************************
    created:    2011/07/26
    filename:   CommandClipAddLotClip.cpp
    author:     bqlf
    purpose:    切片编辑中 等比例一次性添加多条切片数据(clip<name,x,y,w,h>)的命令
*********************************************************************/
#include "commandclipaddlotclip.h"

CommandClipAddLotClip::CommandClipAddLotClip(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

CommandClipAddLotClip::~CommandClipAddLotClip()
{
//    delete receiver_;
}

void CommandClipAddLotClip::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->addLotClip();
        receiver_->save_Memento_ClipData();
        receiver_->setFileModify(true);
    }
}
