#include "observer.h"
#include "subject.h"

void Observer::unregister() {
    this->subject->removeObserver(this);
}
