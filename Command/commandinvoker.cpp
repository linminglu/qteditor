#include "commandinvoker.h"

CommandInvoker::CommandInvoker()
{
    cmd_ = 0;
}

CommandInvoker::~CommandInvoker()
{
    delete cmd_;
    cmd_ = 0;
}

void CommandInvoker::setCommand(Command *cmd)
{
    cmd_ = cmd;
}

void CommandInvoker::execCmd()
{
    if(cmd_) {
        cmd_->execute();
    }
}
