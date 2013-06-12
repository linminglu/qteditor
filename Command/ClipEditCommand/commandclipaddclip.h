#ifndef COMMANDCLIPADDCLIP_H
#define COMMANDCLIPADDCLIP_H

#include "Command/command.h"
#include "spriteEditor/spritewindow.h"
class CommandClipAddClip : public Command
{
public:
    CommandClipAddClip(SpriteWindow* receiver);
    ~CommandClipAddClip();
    void execute();
private:
    SpriteWindow* receiver_;
};

#endif // COMMANDCLIPADDCLIP_H
