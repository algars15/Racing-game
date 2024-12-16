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

}

ModuleMenu::~ModuleMenu()
{

}

bool ModuleMenu::Start()
{

	menuTexture = LoadTexture("Assets/menu.png");
    LoadHightScore();

	LOG("Loading Intro assets");
	bool ret = true;


	return ret;
}

update_status ModuleMenu::Update()
{
	DrawTexture(menuTexture, 0, 0, WHITE);

    char textoPreviousScore[50];
    sprintf_s(textoPreviousScore, "PS: %d", previousScore);
    DrawText(textoPreviousScore, App->window->GetWidth() / 2 - MeasureText(textoPreviousScore, 32) / 2, 560, 32, WHITE);

    char textoHighScore[50];
    sprintf_s(textoHighScore, "HS: %d", hightScore);
    DrawText(textoHighScore, App->window->GetWidth()/2-MeasureText(textoHighScore,32) / 2, 600, 32, GOLD);
    
	return UPDATE_CONTINUE;
}

void ModuleMenu::LoadHightScore() {
    std::ifstream file("HighScore.txt");

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
    }
}



bool ModuleMenu::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}