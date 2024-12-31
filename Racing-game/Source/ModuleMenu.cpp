#pragma once

#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleMenu.h"
#include "Module.h"
#include "Application.h"
#include "fstream"


ModuleMenu::ModuleMenu(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    name = "menu";
}

ModuleMenu::~ModuleMenu()
{

}

bool ModuleMenu::Start()
{
    LOG("Loading menu assets");

    goGame = false;
    state = ModuleMenu::MENU;
    //MENU
    menuTexture = LoadTexture(parameters.child("menu").attribute("path").as_string());

    //TRACK SELECTION 
    fontRushDriver = LoadFont(parameters.attribute("fontPath").as_string());
    raceSelectionTexture = LoadTexture(parameters.child("trackSelection").attribute("path").as_string());
    trackSelector = LoadTexture(parameters.child("trackSelection").attribute("trackSelectorPath").as_string());

    for (pugi::xml_node track : parameters.child("trackSelection").children("track")) {
       
        Vector2 position = { track.attribute("x").as_int(), track.attribute("y").as_int() };
        trackPositions.push_back(position);
    }

    //CAR SELECTION
    carSelectionBkgTexture = LoadTexture(parameters.child("carSelection").attribute("bkgPath").as_string());
    carSelectionCarsTexture = LoadTexture(parameters.child("carSelection").attribute("carsPath").as_string());
    carSelector = LoadTexture(parameters.child("carSelection").attribute("carSelectorPath").as_string());
    int carSelectorOffset = parameters.child("carSelection").attribute("selectorOffset").as_int();

    for (pugi::xml_node track : parameters.child("carSelection").children("car")) {

        Vector2 position = { track.attribute("x").as_int() - carSelectorOffset, track.attribute("y").as_int() - carSelectorOffset };
        carPositions.push_back(position);
    }

    //AUDIO
    menuSong = LoadSound(parameters.attribute("songPath").as_string());
    SetSoundVolume(menuSong, 0.5f);
    selectionSound1 = LoadSound(parameters.attribute("selectionPath").as_string());
    SetSoundPitch(selectionSound1, 2);
    selectionSound2 = LoadSound(parameters.attribute("selectionPath").as_string());
    SetSoundPitch(selectionSound2, 3);
    pressSound = LoadSound(parameters.attribute("pressPath").as_string());
    SetSoundVolume(pressSound, 0.5f);

    LoadSavedData();

	bool ret = true;
	return ret;
}

update_status ModuleMenu::Update(float dt)
{    
    if (App->scene->GetState() == MENU) {

        if (!IsSoundPlaying(menuSong)) PlaySound(menuSong);

        switch (state)
        {
        case ModuleMenu::MENU:
            if (IsKeyPressed(KEY_SPACE))
            {
                state = ModuleMenu::RACE_SELECTION; 
                trackSelected = 0;
                PlaySound(pressSound);

            }
            DrawTexture(menuTexture, 0, 0, WHITE);
            break;
        case ModuleMenu::RACE_SELECTION:
        {

            if (IsKeyPressed(KEY_SPACE))
            {
                App->game->SetTrack(trackSelected);
                state = ModuleMenu::CAR_SELECTION;
                j1CarSelected = 0;
                j2CarSelected = -1;
                PlaySound(pressSound);
            }
            else
            {
                int previousTrack = trackSelected;
                if (IsKeyPressed(KEY_D)) trackSelected = trackSelected + 1 >= trackPositions.size() ? trackSelected : trackSelected + 1;
                else if (IsKeyPressed(KEY_A)) trackSelected = trackSelected - 1 < 0 ? trackSelected : trackSelected - 1;
                else if (IsKeyPressed(KEY_W)) trackSelected = trackSelected - 4 < 0 ? trackSelected : trackSelected - 4;
                else if (IsKeyPressed(KEY_S)) trackSelected = trackSelected + 4 >= trackPositions.size() ? trackSelected : trackSelected + 4;
                if (trackSelected != previousTrack)
                {
                    if (!IsSoundPlaying(selectionSound1)) PlaySound(selectionSound1);
                }
            }

            DrawTexture(raceSelectionTexture, 0, 0, WHITE);
            DrawTexture(trackSelector, trackPositions[trackSelected].x, trackPositions[trackSelected].y, WHITE);

            for (int i = 0; i < tracksTimes.size(); i++)
            {
                float time = tracksTimes[i];
                if (time != 1000)
                {
                    int minutes = (int)time / 60;
                    int seconds = (int)time % 60;
                    int milliseconds = (int)((time - (int)time) * 1000);
                    char timeText[30];
                    sprintf_s(timeText, "%d : %d : %d", minutes, seconds, milliseconds);
                    Vector2 textSize = MeasureTextEx(fontRushDriver, timeText, 20, 5);
                    DrawTextEx(fontRushDriver, timeText, { trackPositions[i].x + trackSelector.width / 2 - textSize.x / 2, trackPositions[i].y + trackSelector.height + textSize.y / 2 }, 20, 5, WHITE);
                }
            }
        }
            break;
        case ModuleMenu::CAR_SELECTION:
            if (IsKeyPressed(KEY_SPACE))
            {
                App->game->SetCars(j1CarSelected, j2CarSelected);
                goGame = true;
                PlaySound(pressSound);
            }
            else if (!goGame)
            {
                int j1Sumator = 0;
                if (IsKeyPressed(J1_KEY_RIGHT)) 
                    j1Sumator = j1CarSelected + 1 >= carPositions.size() ? 0 : 1;
                else if (IsKeyPressed(J1_KEY_LEFT))
                    j1Sumator = j1CarSelected - 1 < 0 ? 0 : - 1;
                else if (IsKeyPressed(J1_KEY_UP))
                    j1Sumator = j1CarSelected - 6 < 0 ? 0 : - 6;
                else if (IsKeyPressed(J1_KEY_DOWN))
                    j1Sumator = j1CarSelected + 6 >= carPositions.size() ? 0 : + 6;

                if (j1CarSelected + j1Sumator == j2CarSelected) 
                {
                    j1Sumator *= 2;
                    if (j1CarSelected + j1Sumator >= carPositions.size() || j1CarSelected + j1Sumator < 0) j1Sumator = 0;
                }
                j1CarSelected += j1Sumator;
                if (j1Sumator != 0)
                {
                    if (!IsSoundPlaying(selectionSound1)) PlaySound(selectionSound1);
                }

                int j2Sumator = 0;
                if (j2CarSelected < 0)
                {
                    if (IsKeyPressed(J2_KEY_RIGHT) || IsKeyPressed(J2_KEY_LEFT) || IsKeyPressed(J2_KEY_UP) || IsKeyPressed(J2_KEY_DOWN)) j2Sumator = j2CarSelected + 1 >= carPositions.size() ? 0 : +1;
                }
                else
                {
                    if (IsKeyPressed(J2_KEY_RIGHT)) j2Sumator = j2CarSelected + 1 >= carPositions.size() ? 0 : +1;
                    else if (IsKeyPressed(J2_KEY_LEFT)) j2Sumator = j2CarSelected - 1 < 0 ? 0 : -1;
                    else if (IsKeyPressed(J2_KEY_UP)) j2Sumator = j2CarSelected - 6 < 0 ? 0 : -6;
                    else if (IsKeyPressed(J2_KEY_DOWN)) j2Sumator = j2CarSelected + 6 >= carPositions.size() ? 0 : +6;
                }

                if (j2CarSelected + j2Sumator == j1CarSelected)
                {
                    j2Sumator *= 2;
                    if (j2CarSelected + j2Sumator >= carPositions.size() || j2CarSelected + j2Sumator < 0) j2Sumator = 0;
                }
                j2CarSelected += j2Sumator;
                if (j2Sumator != 0)
                {
                    if (!IsSoundPlaying(selectionSound2)) PlaySound(selectionSound2);
                }
            }
            
            DrawTexture(carSelectionBkgTexture, 0, 0, WHITE);
            DrawTexture(carSelector, carPositions[j1CarSelected].x, carPositions[j1CarSelected].y, BLUE);
            if (j2CarSelected >= 0) DrawTexture(carSelector, carPositions[j2CarSelected].x, carPositions[j2CarSelected].y, RED);
            DrawTexture(carSelectionCarsTexture, 0, 0, WHITE);
            break;
        default:
            break;
        }
        
    }
    
	return UPDATE_CONTINUE;
}

bool ModuleMenu::GetGoGame()
{
    return goGame;
}

void ModuleMenu::RestartMenu() {
    state = MENU;
    goGame = false;
    StopSound(menuSong);
    StopSound(selectionSound1);
    StopSound(selectionSound2);
    StopSound(pressSound);
}

bool ModuleMenu::CleanUp()
{
    UnloadSound(menuSong);
    UnloadSound(selectionSound1);
    UnloadSound(selectionSound2);
    UnloadSound(pressSound);
    UnloadFont(fontRushDriver);
	LOG("Unloading Intro scene");

	return true;
}

void ModuleMenu::LoadSavedData()
{
    pugi::xml_document configFile;
    pugi::xml_parse_result result = configFile.load_file("config.xml");
    if (result)
    {
        LOG("config.xml parsed without errors");
    }
    else
    {
        LOG("Error loading config.xml while loading: %s", result.description());
    }

    pugi::xml_node tracksNode = configFile.child("config").child("game").child("tracks");

    tracksTimes.clear();
    for (pugi::xml_node child : tracksNode.children())
    {
        tracksTimes.push_back(child.attribute("fastestLap").as_float());
        TraceLog(LOG_INFO, "%f", child.attribute("fastestLap").as_float());
    }

    configFile.save_file("config.xml");
}