#ifndef COMMANDSPRITESETSUBCLIP_H
#define COMMANDSPRITESETSUBCLIP_H
#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandSpriteSetSubClip : public Command
{
public:
    CommandSpriteSetSubClip(SpriteWindow* receiver);
//    ~CommandSpriteSetSubClip();
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDSPRITESETSUBCLIP_H
