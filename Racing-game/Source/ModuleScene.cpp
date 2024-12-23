#pragma once

#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleMenu.h"
#include "Module.h"
#include "Application.h"


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    name = "scene"; 
}

ModuleScene::~ModuleScene()
{

}

void ModuleScene::SetMenu(ModuleMenu* m)
{
    menu = m;
}

void ModuleScene::SetGame(ModuleGame* g)
{
    game = g;
}

bool ModuleScene::Start()
{
    menu->Start();
    game->Start();

	state = MENU;

	LOG("Loading Intro assets");
	bool ret = true;
	App->renderer->camera.x = App->renderer->camera.y = 0;

    state = TRANSITION;
    fadeAlpha = 0.0f;
    fadeSpeed = 255.0f / 1.0f;
    fadeState = FADEIN;
    toMenu = true;


	return ret;
}

update_status ModuleScene::Update(float dt)
{
    if (state == MENU)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            // Iniciar fade out
            state = TRANSITION;
            fadeState = FADEOUT;
            toMenu = false;
            fadeAlpha = 0;
            
        }
    }
    else if (state == GAME)
    {
        if (game->GetReturnMain())
        {
            state = TRANSITION;
            fadeState = FADEOUT;
            toMenu = true;
            fadeAlpha = 0;
        }
    }
    else
    {
        if (fadeState == FADEIN)
        {
            fadeAlpha -= fadeSpeed * dt; 

            if (fadeAlpha <= 0.0f)
            {
                fadeAlpha = 0.0f;
                if (toMenu) state = MENU;
                else state = GAME;
            }
        }
        else if (fadeState == FADEOUT)
        {
            fadeAlpha += fadeSpeed * dt;
            
            if (fadeAlpha >= 255.0f)
            {
                fadeState = BETWEEN;
                fadeAlpha = 255.0f;
            }
           
        }
        else
        {
            fadeState = FADEIN;
            if (!toMenu) game->LoadGame();
            else menu->LoadHightScore();
        }

        DrawRectangle(0, 0, App->window->GetWidth(), App->window->GetHeight(), { 0, 0, 0, static_cast<unsigned char>(fadeAlpha)});
    }

    return UPDATE_CONTINUE;
}


// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

State ModuleScene::GetState()
{
    return state;
}

bool ModuleScene::GetToMenu()
{
    return toMenu;
}

FadeState ModuleScene::GetFadeState()
{
    return fadeState;
}