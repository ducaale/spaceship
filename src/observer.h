#ifndef OBSERVER_H
#define OBSERVER_H

#include <functional>

#include "events.h"

class Observer {
public:
    virtual ~Observer() {}
    virtual void onNotify(Events event, float sleep, std::function<void()> next) = 0;
protected:
    float counter = 0, sleep = 0;
    std::function<void()> next;
};

#endif /* OBSERVER_H */
