//
// Created by turlakovai on 25.03.2022.
//

#ifndef RAYLIBTEMPLATE_EVENTS_H
#define RAYLIBTEMPLATE_EVENTS_H

#include <vector>
#include <string>
#include <iostream>
using namespace std;

enum EventType {
    PLAY_PAUSE_EVENT,
    REWIND_EVENT,
    LOAD_MUSIC_EVENT,
    CHANGE_TRACK_EVENT,
    CHANGE_VOLUME_EVENT,
    CHANGE_PITCH_EVENT,
    RESET_PITCH_EVENT,
    END_PROGRAM_EVENT,
    RESTART_MUSIC_EVENT,
    REWIND_BAR_EVENT,
    STOP_BUTTON_EVENT,
    PLAY_BUTTON_EVENT,
    DRAGON_DROP_EVENT,
    VOLUME_BAR_EVENT,
    SWITCHING_PITCH_EVENT,
};

struct Event {
    EventType type;
    float value;
    string way;
    //    string path;
};

std::vector<Event> events;

#endif //RAYLIBTEMPLATE_EVENTS_H
