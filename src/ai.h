#ifndef AI_H
#define AI_H

#include "subject.h"
#include "events.h"

class AI : public Subject {
private:
    std::queue<std::pair<Events, float>> orders;

public:
    AI() {
        orders.push(std::make_pair(Events::orb_open_laser, 6));
        orders.push(std::make_pair(Events::orb_close_laser, 5));
        orders.push(std::make_pair(Events::orb_open_laser, 6));
    }

    void next() {
        if(!orders.empty()) {
            auto [event, sleep] = orders.front();
            notify(event, sleep, [&]() { this->next(); });
            orders.pop();
        }
    }

    void init() {
        next();
    }

};

#endif /* AI_H */
