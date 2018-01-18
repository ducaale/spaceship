#ifndef OBSERVER_H
#define OBSERVER_H

#include <functional>

#include "events.h"

class Subject;

class Observer {
public:
    virtual ~Observer() { unregister(); }
    virtual void setSubject(Subject *subject) { this->subject = subject; }
    virtual void onNotify(Events event, float sleep, std::function<void()> next) = 0;

protected:
    float counter = 0, sleep = 0;
    std::function<void()> next;

private:
    Subject *subject;
    virtual void unregister();
};

#endif /* OBSERVER_H */
