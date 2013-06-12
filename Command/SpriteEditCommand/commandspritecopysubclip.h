#ifndef COMMANDSPRITECOPYSUBCLIP_H
#define COMMANDSPRITECOPYSUBCLIP_H
#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandSpriteCopySubClip : public Command
{
public:
    CommandSpriteCopySubClip(SpriteWindow* receiver);
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDSPRITECOPYSUBCLIP_H
