//
// Created by turlakovai on 25.03.2022.
//

#ifndef RAYLIBTEMPLATE_BUTTONSPROCESSOR_H
#define RAYLIBTEMPLATE_BUTTONSPROCESSOR_H

#include "raylib.h"
#include "Events.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

void KeyBoard() {
    if (IsKeyPressed(KEY_F7)) {
        events.push_back({PLAY_PAUSE_EVENT});
    }
    if (IsKeyPressed(KEY_SPACE)) {
        events.push_back({PLAY_PAUSE_EVENT});
    }
    if (IsKeyPressed(KEY_P)) {
        events.push_back({RESTART_MUSIC_EVENT});
    }

    if (IsKeyDown(KEY_MINUS)) events.push_back({CHANGE_PITCH_EVENT, -0.01});
    else if (IsKeyDown(KEY_EQUAL)) events.push_back({CHANGE_PITCH_EVENT, +0.01});
    else if (IsKeyDown(KEY_L)) events.push_back({RESET_PITCH_EVENT});

    if (IsKeyDown(KEY_UP)) events.push_back({CHANGE_VOLUME_EVENT, +0.020f});
    else if (IsKeyDown(KEY_DOWN)) events.push_back({CHANGE_VOLUME_EVENT, -0.020f});

    if (IsKeyPressed(KEY_RIGHT)) events.push_back({REWIND_EVENT, +5.0f});
    else if (IsKeyPressed(KEY_LEFT)) events.push_back({REWIND_EVENT, -5.0f});


}


#endif //RAYLIBTEMPLATE_BUTTONSPROCESSOR_H
