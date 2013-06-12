#ifndef COMMANDSPRITEPASTESUBCLIP_H
#define COMMANDSPRITEPASTESUBCLIP_H
#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandSpritePasteSubClip : public Command
{
public:
    CommandSpritePasteSubClip(SpriteWindow* receiver);
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDSPRITEPASTESUBCLIP_H
