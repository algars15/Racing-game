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

    //MENU
    menuTexture = LoadTexture(parameters.child("menu").attribute("path").as_string());

    //TRACK SELECTION 
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

	bool ret = true;
	return ret;
}

update_status ModuleMenu::Update(float dt)
{    
    if (App->scene->GetState() == MENU) {

        switch (state)
        {
        case ModuleMenu::MENU:
            if (IsKeyPressed(KEY_SPACE))
            {
                state = ModuleMenu::RACE_SELECTION; 
                trackSelected = 0;
            }
            DrawTexture(menuTexture, 0, 0, WHITE);
            break;
        case ModuleMenu::RACE_SELECTION:
            if (IsKeyPressed(KEY_SPACE))
            {
                App->game->SetTrack(trackSelected);
                state = ModuleMenu::CAR_SELECTION;
                j1CarSelected = 0;
                j2CarSelected = -1;
            }
            else
            {
                if (IsKeyPressed(KEY_D)) trackSelected = trackSelected + 1 >= trackPositions.size() ? trackSelected : trackSelected + 1;
                else if (IsKeyPressed(KEY_A)) trackSelected = trackSelected - 1 < 0 ? trackSelected : trackSelected - 1;
                else if (IsKeyPressed(KEY_W)) trackSelected = trackSelected - 4 < 0 ? trackSelected : trackSelected - 4;
                else if (IsKeyPressed(KEY_S)) trackSelected = trackSelected + 4 >= trackPositions.size() ? trackSelected : trackSelected + 4;
            }
            
            DrawTexture(raceSelectionTexture, 0, 0, WHITE);
            DrawTexture(trackSelector, trackPositions[trackSelected].x, trackPositions[trackSelected].y, WHITE);
            
            break;
        case ModuleMenu::CAR_SELECTION:
            if (IsKeyPressed(KEY_SPACE))
            {
                App->game->SetCars(j1CarSelected, j2CarSelected);
                goGame = true;

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

void ModuleMenu::LoadHightScore() {
    /*std::ifstream file("HighScore.txt");

    TraceLog(LOG_INFO, "Cargando informacion del archivo de puntuaje");
    
    if (!file.is_open()) {
        std::ofstream newFile("HighScore.txt");  
        newFile << "0\n0" << std::endl;
        newFile.close();  
        TraceLog(LOG_INFO, "Archivo de puntaje no encontrado. Se ha creado uno nuevo...");
        hightScore = 0;
    }
    else {
        
        file >> hightScore >> previousScore;
        if (file.fail()) {
            
            TraceLog(LOG_INFO, "Error al leer el archivo. Estableciendo el puntaje a 0.");
            hightScore = 0;
        }
        file.close();  
    }*/
}



bool ModuleMenu::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}