#ifndef COMMANDSPRITECHANGENAME_H
#define COMMANDSPRITECHANGENAME_H
#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandSpriteChangeName : public Command
{
public:
    CommandSpriteChangeName(SpriteWindow* receiver);
//    ~CommandSpriteChangeName();
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDSPRITECHANGENAME_H
