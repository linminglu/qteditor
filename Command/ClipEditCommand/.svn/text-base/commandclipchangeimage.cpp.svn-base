/********************************************************************
    created:    2011/07/21
    filename:   commandclipchangeimage.cpp
    author:     bqlf
    purpose:    切片编辑中 修改图像(*.sprite文件中)的命令
*********************************************************************/
#include "commandclipchangeimage.h"

CommandClipChangeImage::CommandClipChangeImage(SpriteWindow* receiver)
{
    receiver_ = receiver;
}

CommandClipChangeImage::~CommandClipChangeImage()
{
//    delete receiver_;
}

void CommandClipChangeImage::execute()
{
    if(receiver_ && !receiver_->isSetMemento()) {
        receiver_->changeImage();
        receiver_->save_Memento_ClipData();
        receiver_->setFileModify(true);
    }
}
