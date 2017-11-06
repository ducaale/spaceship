#ifndef AI_H
#define AI_H

#include <fstream>

#include "subject.h"
#include "events.h"

#include "json.hpp"

using json = nlohmann::json;

class AI : public Subject {
private:
    std::vector<std::pair<Events, float>> orders;
    int order_index = 0;

public:
    AI() {
        //load values from json file
        std::ifstream file("values.json");
        json values;
        file >> values;

        for(auto& value : values["orb_ai"][0]) {
            orders.push_back({value["event"], value["duration"]});
        }
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
