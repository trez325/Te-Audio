#include "raylib.h"
#include "settings.h"
#include "audiof.h"
#include <set>
#include <iostream>
#include "ButtonsProcessor.h"

#define RAYGUI_IMPLEMENTATION

#include "extras/raygui.h"
#include "ResourceLoader.hpp"
#include "darkstyle.h"
//#define MAX_FONTS   8
using namespace std;
std::set<string> musicName;
std::vector<const char *> fullPlayListNames;
std::vector<const char *> shortPlayListNames;
float heightBar = 5;
float yyposition;
float initalPosition1 = -200;
float initalPosition2 = -200;
float initalPosition3 = -200;
bool uptime;
bool set = false;
int waitingTime1 = 0;
int waitingTime2 = 0;
int waitingTime3 = 0;

void DragonDropAndRecord(int coun) {
    if (IsFileDropped()) {
        char **files = GetDroppedFiles(&coun);
        for (int i = 0; i < coun; ++i) {
            string s(strlen(files[i]), '\0');
            strcpy(s.data(), files[i]);
            musicName.insert(s);
        }
        ClearDroppedFiles();
    }
    if (fullPlayListNames.size() != musicName.size()) {
        fullPlayListNames.clear();
        shortPlayListNames.clear();
        for (int i = 0; i < shortPlayListNames.size(); ++i) {
            delete[] shortPlayListNames[i];
        }
        for (auto el = musicName.begin(); el != musicName.end(); ++el) {
            fullPlayListNames.push_back(el->c_str());
            if (string((GetFileName(el->c_str()))).size() >= 23) {
                string name = string((GetFileName(el->c_str()))).substr(0, 23) + "...";
                char *real_name = new char[27];
                for (int i = 0; i < 26; ++i) {
                    real_name[i] = name[i];
                }
                real_name[26] = '\0';
                shortPlayListNames.push_back(real_name);
            } else {
                shortPlayListNames.push_back(GetFileName(el->c_str()));
            }
        }
    }
}

void LoadCurrentMusic(int Active) {
    events.push_back({CHANGE_TRACK_EVENT, 0, fullPlayListNames[Active]});
}
void LoadCurrentPitch(int ActiveBox) {
    float pitchValue[6] = {0.5,0.75,1,1.5,2,5};
    events.push_back({SWITCHING_PITCH_EVENT, pitchValue[ActiveBox]});
}
void CheckCollisionMain() {
    Vector2 mousePoint = {0.0f, 0.0f};
    mousePoint = GetMousePosition();
    mediator.xmouse = GetMouseX();
    float xmouse = GetMouseX();
    Rectangle rec3 = {(float) 65, (float) 372, 18, 18};
    Rectangle rec2 = {(float) 42, (float) 372, 18, 18};
    Rectangle rec1 = {(float) 19, (float) 372, 18, 18};
    Rectangle settings = {(float) 5, (float) 5, 58, 20};
    Rectangle mainBoard = {(float) 10, (float) 28, 540, 338};
    Rectangle timepoll = {(float) 100, (float) 379, 400, 5};
    Rectangle volumeBar = {(float) 475, (float) 11, 50, 10};
    if (CheckCollisionPointRec(mousePoint, rec3)) {
        waitingTime3++;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            events.push_back({RESTART_MUSIC_EVENT});
        }
        if (waitingTime3 >= 30) {
            initalPosition3 = 0;
        }
    } else {
        waitingTime3 = 0;
        initalPosition3 = -200;
    }
    if (CheckCollisionPointRec(mousePoint, rec2)) {
        waitingTime2++;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            //PLAY
            events.push_back({PLAY_BUTTON_EVENT});
        }
        if (waitingTime2 >= 30) {
            initalPosition2 = 0;
        }
    } else {
        waitingTime2 = 0;
        initalPosition2 = -200;
    }
    if (CheckCollisionPointRec(mousePoint, rec1)) {
        waitingTime1++;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            events.push_back({STOP_BUTTON_EVENT});
        }
        if (waitingTime1 >= 30) {
            initalPosition1 = 0;
        }
    } else {
        waitingTime1 = 0;
        initalPosition1 = -200;
    }

    //            SETTINS***
    if (CheckCollisionPointRec(mousePoint, settings)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            //setting(); !!!!!!!!!!!!! OR !!!!!!!!!!!!!
            //            set = true;
        }
    }


    if (CheckCollisionPointRec(mousePoint, timepoll)) {
        heightBar = 10;
        yyposition = 377;
        uptime = 1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            events.push_back({REWIND_BAR_EVENT});
        }
    }
    if (!CheckCollisionPointRec(mousePoint, timepoll)) {
        heightBar = 5;
        yyposition = 379;
        uptime = 0;
    }
    if (CheckCollisionPointRec(mousePoint, mainBoard)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            events.push_back({PLAY_PAUSE_EVENT});
        }
    }
    if (CheckCollisionPointRec(mousePoint, volumeBar)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            events.push_back({VOLUME_BAR_EVENT, 1.5f / 50 * (xmouse - 475)});
        }
    }
}

int main(int argsCount, char **args) {
    setlocale(LC_ALL, "Russian");

    // Initialization
    //--------------------------------------------------------------------------------------
    //Image Icon = LoadImage("C:\\Users\\trez325\\Downloads\\Aimp.ico");
    //SetWindowIcon(Icon);

    const int screenWidth = 750;
    const int screenHeight = 400;
    if (argsCount >= 2) {
        InitWindow(screenWidth, screenHeight, "te-au");
        events.push_back({LOAD_MUSIC_EVENT, 0, args[1]});
    } else {
        InitWindow(screenWidth, screenHeight, "te-au(Trez AudioPlayer)");//Block with delete
        events.push_back({LOAD_MUSIC_EVENT, 0,
                          "C:\\Users\\trez325\\music\\Yeek-Only-in-the-West.wav"});//Block with delete
    }

    std::thread t(audioPlayer);

    //SetWindowState(FLAG_WINDOW_RESIZABLE);

    //    InitAudioDevice(); // Initialize audio device
    //***    variables    ***
    float xmouse;
    bool set = false;
    int count;
    int sumCount;
    int listViewExScrollIndex = 0;
    int listViewExActive = -1;
    int listViewExFocus = -1;
    int listViewLastActive = 0;
    int comboBoxActive = 2;
    int comboBoxCheck = -1;

    //LOAD INFORMATION
    Image background = loadTextureFromMemory("/skins/audio interQT1.png");
    Image PauseRectangle = loadTextureFromMemory("/skins/pauseRec.png");
    Image PlayRectangle = loadTextureFromMemory("/skins/playRec.png");
    Image ResumeRectangle = loadTextureFromMemory("/skins/resumeRec.png");
    Image management = loadTextureFromMemory("/skins/Buildd.png");
    Image sett = loadTextureFromMemory("/skins/settin.png");
    Image asset = loadTextureFromMemory("/skins/asset.png");
    Texture2D asssets = LoadTextureFromImage(asset);
    Texture2D texture = LoadTextureFromImage(background);
    Texture2D managements = LoadTextureFromImage(management);
    Texture2D PauseRec = LoadTextureFromImage(PauseRectangle);
    Texture2D ResumeRec = LoadTextureFromImage(ResumeRectangle);
    Texture2D PlayRec = LoadTextureFromImage(PlayRectangle);
    Texture2D settin = LoadTextureFromImage(sett);
    Vector2 position = {0, 0};

    GuiLoadStyleDark();
    //GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);

    //FPS
    int fps = 60;
    SetTargetFPS(fps); // Set our game to run at 60 frames-per-second
    //-------------------------------------------------------------------------------------
    // Main game loop
    int check;
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        KeyBoard();
        DragonDropAndRecord(count);
        CheckCollisionMain();
        if (mediator.timePlayed > 399) {
            if (listViewLastActive + 1 < shortPlayListNames.size()) {
                events.push_back({CHANGE_TRACK_EVENT, 0, fullPlayListNames[listViewLastActive + 1]});
                listViewLastActive++;
                check = 100;
            } else if (listViewLastActive > 0 and listViewLastActive + 1 >= shortPlayListNames.size()) {
                events.push_back({CHANGE_TRACK_EVENT, 0, fullPlayListNames[0]});
                listViewLastActive = 0;
                check = 200;
            } else {
                events.push_back({RESTART_MUSIC_EVENT});
                events.push_back({RESET_PITCH_EVENT});
                check = 300;
            }
        }
        if (comboBoxActive != comboBoxCheck){
            LoadCurrentPitch(comboBoxActive);
            comboBoxCheck = comboBoxActive;
        }
        if (listViewExActive >= 0) {
            LoadCurrentMusic(listViewExActive);
            listViewLastActive = listViewExActive;
            listViewExActive = -1;
        }
        xmouse = GetMouseX();
        mediator.xmouse = GetMouseX();
        int minutes = mediator.sec / 60;
        int seconds = mediator.sec - (minutes * 60);


        //***    IF OR ELSE    ***
        if (IsKeyPressed(KEY_C)) {
            break;
        }
        //        for (int i = 0; i < musicName.size(); ++i)
        //            strings.push_back(musicName[i].c_str();

        // Get timePlayed scaled to bar dimensions (400 pixels)

        //----------------------------------------------------------------------------------

        if (!IsWindowHidden()) {// Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(BLACK);
            //Green interface

            //            DrawRectangleRec(rec3, GREEN);
            //            DrawRectangleRec(rec2, GREEN);
            //            DrawRectangleRec(rec1, GREEN);
            //            DrawRectangleRec(settings, GREEN);
            //            DrawRectangleRec(mainBoard, GREEN);


            //-------------------------------------------
            DrawTextureV(texture, position, WHITE);
            DrawTextureV(managements, position, WHITE);
            //Hints
            DrawTexture(PauseRec, initalPosition1, 0, WHITE);
            DrawTexture(PlayRec, initalPosition2, 0, WHITE);
            DrawTexture(ResumeRec, initalPosition3, 0, WHITE);
            DrawTexture(asssets, mediator.volume + 34, 0, WHITE);
            //DrawRectangle(100, 350, (int) timePlayed, 12, ORANGE);
            DrawRectangle(100, 379, (int) mediator.timePlayed, 5, GRAY);
            DrawRectangle(475, 13, mediator.volume, 2.5, GRAY);
            DrawRectangleLines(100, yyposition, 400, heightBar, GRAY);
            DrawRectangle(475, 13.5, 50, 1, GRAY);
            //DrawRectangleRec(timepoll, GREEN);
            //            if (sumCount == 0) DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
            //            else {
            //                int index = 0;
            //                for (auto el = musicName.begin(); el != musicName.end(); ++el) {
            //                    string value = *el;
            //                    DrawText(GetFileName(value.c_str()), 580, 35 + 40 * index++, 10, GRAY);
            //                }
            //            }

            listViewExActive = GuiListViewEx((Rectangle) {560, 26, 180, 342},
                                             shortPlayListNames.data(), shortPlayListNames.size(), &listViewExFocus,
                                             &listViewExScrollIndex,
                                             listViewExActive);
            comboBoxActive = GuiComboBox((Rectangle){ 380, 7, 80, 12 }, "0.5x;0.75x;1x;1,5x;2x;5x", comboBoxActive);
            DrawText(TextFormat("%i", minutes), 510, 377, 4, LIGHTGRAY);
            DrawText(TextFormat(" :%i", seconds), 513, 377, 4, LIGHTGRAY);
            //DrawText(TextFormat("%i", count), 550, 377, 4, LIGHTGRAY);
//            DrawText(TextFormat(" :%i", fullPlayListNames.size()), 513, 350, 4, LIGHTGRAY);
//            DrawText(TextFormat(" :%i", listViewLastActive), 513, 320, 4, LIGHTGRAY);
//            DrawText(TextFormat(" :%i", check), 513, 300, 4, LIGHTGRAY);

            if (uptime) {
                DrawText(TextFormat("%i", mediator.minutes2), mediator.xmouse - 2, 360, 4, LIGHTGRAY);
                DrawText(TextFormat(" :%i", mediator.seconds2), mediator.xmouse, 360, 4, LIGHTGRAY);
            } else {}
            if (shortPlayListNames.size() <= 0) {
                DrawText("add files", 628, 213, 5, DARKGRAY);
                DrawText("+", 630, 150, 78, DARKGRAY);
            } else {}
            if (set) {
                DrawTextureV(settin, position, WHITE);
            }
            //DrawText("PRESS C FOR LEAVE", 10, 352, 14, LIGHTGRAY);
            EndDrawing();
            //----------------------------------------------------------------------------------
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    CloseAudioDevice();// Close audio device (fara streaming is automatically stopped)

    CloseWindow();// Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    events.push_back({END_PROGRAM_EVENT});

    t.join();

    return 0;
}