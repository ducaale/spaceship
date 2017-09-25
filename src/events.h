#ifndef EVENTS_H
#define EVENTS_H

enum Events : std::size_t {
    //orb actions
    orb_start_firing,
    orb_end_firing,

    orb_open_laser,
    orb_close_laser,

    orb_start_targeting,
    orb_end_targeting,

    orb_start_lunching_rockets,
    orb_end_lunching_rockets,
};


#endif /* EVENTS_H */
