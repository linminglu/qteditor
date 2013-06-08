#ifndef COMMANDCLIPSETCLIP_H
#define COMMANDCLIPSETCLIP_H

#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandClipSetClip : public Command
{
public:
    CommandClipSetClip(SpriteWindow* receiver);
    ~CommandClipSetClip();
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDCLIPSETCLIP_H
