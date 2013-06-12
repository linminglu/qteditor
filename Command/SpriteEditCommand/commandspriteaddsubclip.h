#ifndef COMMANDSPRITEADDSUBCLIP_H
#define COMMANDSPRITEADDSUBCLIP_H
#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandSpriteAddSubClip : public Command
{
public:
    CommandSpriteAddSubClip(SpriteWindow* receiver);
//    ~CommandSpriteAddSubClip();
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDSPRITEADDSUBCLIP_H
