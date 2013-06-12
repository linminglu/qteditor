/********************************************************************
    created:    2011/07/21
    filename:   CommandClipChangeName.cpp
    author:     bqlf
    purpose:    切片编辑中 修改名字(*.sprite文件)的命令
*********************************************************************/

#include "commandclipchangename.h"

CommandClipChangeName::CommandClipChangeName(SpriteWindow* receiver)
{
//    receiver_ = new SpriteWindow;
    receiver_ = receiver;
}

CommandClipChangeName::~CommandClipChangeName()
{
//    delete receiver_;
}

void CommandClipChangeName::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->changeFileName();
        receiver_->save_Memento_ClipData();
        receiver_->setFileModify(true);
    }
 }
