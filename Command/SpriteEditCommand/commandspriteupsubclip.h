#ifndef COMMANDSPRITEUPSUBCLIP_H
#define COMMANDSPRITEUPSUBCLIP_H
#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandSpriteUpSubClip : public Command
{
public:
    CommandSpriteUpSubClip(SpriteWindow* receiver);
//    ~CommandSpriteUpSubClip();
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDSPRITEUPSUBCLIP_H
