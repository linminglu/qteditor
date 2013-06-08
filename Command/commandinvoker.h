#ifndef COMMANDINVOKER_H
#define COMMANDINVOKER_H

#include "Command/command.h"
class CommandInvoker
{
public:
    CommandInvoker();
    ~CommandInvoker();
    void setCommand(Command* cmd);
    void execCmd();
private:
    Command* cmd_;
};

#endif // COMMANDINVOKER_H
