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

    menuTexture = LoadTexture(parameters.child("background").attribute("path").as_string());

	LOG("Loading Intro assets");
	bool ret = true;
	return ret;
}

update_status ModuleMenu::Update(float dt)
{    
    if (App->scene->GetState() == MENU) {

        DrawTexture(menuTexture, 0, 0, WHITE);
    }
    
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