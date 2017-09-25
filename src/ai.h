#ifndef AI_H
#define AI_H

#include "subject.h"
#include "events.h"

class AI : public Subject {
private:
    std::vector<std::pair<Events, float>> orders;
    int order_index = 0;

public:
    AI() {
        orders.push_back(std::make_pair(Events::orb_start_firing, 2));
        orders.push_back(std::make_pair(Events::orb_end_firing, 0));

        orders.push_back(std::make_pair(Events::orb_end_targeting, 0));
        orders.push_back(std::make_pair(Events::orb_open_laser, 2));
        orders.push_back(std::make_pair(Events::orb_close_laser, 0));
        orders.push_back(std::make_pair(Events::orb_start_targeting, 2));

        orders.push_back(std::make_pair(Events::orb_start_lunching_rockets, 3));
        orders.push_back(std::make_pair(Events::orb_end_lunching_rockets, 2));
    }

    void next() {
        if(!orders.empty()) {
            auto [event, sleep] = orders[order_index++ % orders.size()];
            notify(event, sleep, [&]() { this->next(); });
        }
    }

    void init() {
        next();
    }

};

#endif /* AI_H */
