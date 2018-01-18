#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

#include "observer.h"
#include "events.h"

class Subject {
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer *observer) {
        observer->setSubject(this);
        observers.push_back(observer);
    }

    void removeObserver(Observer *observer) {
        observers.erase(
            std::remove_if(std::begin(observers), std::end(observers),
                [observer](Observer *o) {
                    return observer == o;
                }),
            std::end(observers));
    } 

    void notify(Events event, float sleep, std::function<void()> next) {
        for(auto& observer : observers) observer->onNotify(event, sleep, next);
    }
};

#endif /* SUBJECT_H */
