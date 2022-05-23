#include "raylib.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include "ButtonsProcessor.h"
#include "Events.h"

using namespace std;

struct AudioMediator {
    float timePlayed = 0.0f;
    int sec = 0;
    int minutes2;
    int seconds2;
    float xmouse;
    float volume;
    string fileName;
    int endOfPlayback = -1;
};

AudioMediator mediator;
string fileName;

void audioPlayer() {
    InitAudioDevice(); // Initialize audio device
    Music music;

    // ***    variables    ***
    float MusicTimePlayed = 0.0;
    float tress;
    float timePlayed = 0.0f;
    float volume = 1.5f;
    float pitch = 1.0f;
    bool pause = false;
    float maxvol = 1.5;
    float xmouse;
    bool shouldClose = false;


    while (!shouldClose) {
        while (!events.empty()) {
            Event currentEvent = events[0];
            switch (currentEvent.type) {
                case END_PROGRAM_EVENT:
                    shouldClose = true;
                    break;
                case PLAY_PAUSE_EVENT:
                    // todo play pause implementation
                    pause = !pause;

                    if (pause) PauseMusicStream(music);
                    else ResumeMusicStream(music);
                    break;
                case RESTART_MUSIC_EVENT:
                    PlayMusicStream(music);
                    StopMusicStream(music);
                    PlayMusicStream(music);
                    PauseMusicStream(music);
                    pause = true;
                    break;
                case CHANGE_PITCH_EVENT:
                    pitch += currentEvent.value;
                    SetMusicPitch(music, pitch);
                    break;
                case SWITCHING_PITCH_EVENT:
                    pitch = currentEvent.value;
                    SetMusicPitch(music,pitch);
                    break;
                case RESET_PITCH_EVENT:
                    pitch = 1.0f;
                    SetMusicPitch(music, pitch);
                    break;
                case CHANGE_VOLUME_EVENT:
                    if (currentEvent.value > 0) {
                        if (volume < maxvol) {
                            volume += currentEvent.value;
                        } else {}
                    } else {
                        if (volume >= 0.00) {
                            volume += currentEvent.value;
                            if (volume < 0.030f) {
                                volume = 0;
                            }
                        } else {}
                    }
                    break;
                case REWIND_EVENT:
                    if (currentEvent.value > 0) {
                        MusicTimePlayed += currentEvent.value;
                    } else {
                        if (GetMusicTimePlayed(music) < 5) {
                            MusicTimePlayed = 0;
                        } else {
                            MusicTimePlayed += currentEvent.value;
                        }
                    }
                    SeekMusicStream(music, MusicTimePlayed);
                    break;

                case REWIND_BAR_EVENT:
                    tress = GetMusicTimeLength(music) / 400 * (xmouse - 100);
                    MusicTimePlayed = tress;
                    SeekMusicStream(music, MusicTimePlayed);
                    break;

                case STOP_BUTTON_EVENT:
                    PauseMusicStream(music);
                    pause = true;
                    break;
                case PLAY_BUTTON_EVENT:
                    PlayMusicStream(music);
                    ResumeMusicStream(music);
                    break;
                case LOAD_MUSIC_EVENT:
                    fileName = currentEvent.way;
                    music = LoadMusicStream(fileName.c_str());
                    PlayMusicStream(music);
                    break;
                case CHANGE_TRACK_EVENT:
                    UnloadMusicStream(music);
                    fileName = currentEvent.way;
                    music = LoadMusicStream(fileName.c_str());
                    PlayMusicStream(music);
                    break;
                case VOLUME_BAR_EVENT:

                    volume = currentEvent.value;
                    break;

                case DRAGON_DROP_EVENT:
                    int count;
                    char **files = GetDroppedFiles(&count);
                    string s;
                    for (int i = 0; i < count; ++i) {
                        s = files[i];
                        events.push_back({CHANGE_TRACK_EVENT, 0, s});
                    }
                    currentEvent.way = s;
                    break;

            }
            events.erase(events.begin());
        }
        MusicTimePlayed = GetMusicTimePlayed(music);
        UpdateMusicStream(music);

        xmouse = GetMouseX();
        mediator.sec = GetMusicTimePlayed(music);
        mediator.timePlayed = timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music) * 400;
        mediator.volume = volume / 1.5 * 50;
        mediator.minutes2 = GetMusicTimeLength(music) / 400 * (mediator.xmouse - 100) / 60;
        mediator.seconds2 = GetMusicTimeLength(music) / 400 * (mediator.xmouse - 100) - (mediator.minutes2 * 60);
        SetMusicVolume(music, volume);
        // Pause/Resume fara playing
        if (timePlayed >= 399) {
            mediator.endOfPlayback = 1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}