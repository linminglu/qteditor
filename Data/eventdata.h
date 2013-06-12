#ifndef EVENTDATA_H
#define EVENTDATA_H

#include <QString>

class EventData
{
public:
    EventData();

    int id;
    int event_type;
    QString event_content;
};

#endif // EVENTDATA_H
