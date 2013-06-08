#ifndef COMMANDSPRITECOPYMAPSPRITE_H
#define COMMANDSPRITECOPYMAPSPRITE_H
#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandSpriteCopyMapSprite : public Command
{
public:
    CommandSpriteCopyMapSprite(SpriteWindow* receiver);
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDSPRITECOPYMAPSPRITE_H
