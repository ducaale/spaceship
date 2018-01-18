#ifndef EVENTS_H
#define EVENTS_H

enum Events : std::size_t {
    //orb actions

    orb_start_firing = 0,
    orb_end_firing = 1,

    orb_open_laser = 2,
    orb_close_laser = 3,

    orb_start_targeting = 4,
    orb_end_targeting = 5,

    orb_start_lunching_rockets = 6,
    orb_end_lunching_rockets = 7,

    orb_teleport = 8,

    zero_offset_target = 9,
    plus_offset_target = 10,
    minus_offset_target = 11
};


#endif /* EVENTS_H */
