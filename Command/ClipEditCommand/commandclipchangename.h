#ifndef COMMANDCLIPCHANGENAME_H
#define COMMANDCLIPCHANGENAME_H
#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandClipChangeName : public Command
{
public:
    CommandClipChangeName(SpriteWindow* receiver);
    ~CommandClipChangeName();
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDCLIPCHANGENAME_H
