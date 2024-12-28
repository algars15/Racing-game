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
	state = MENU;

	LOG("Loading Intro assets");
	bool ret = true;
	App->renderer->camera.x = App->renderer->camera.y = 0;

    state = MENU;
    toMenu = false;
    toGame = false;
    toLose = false;
    fadeAlpha = 0.0f;
    fadeSpeed = 255.0f / 1.0f;
    fadeState = FADEIN;

	return ret;
}

update_status ModuleScene::Update(float dt)
{
    if (toMenu || toGame || toLose)
    {
        if (fadeState == FADEIN)
        {
            fadeAlpha -= fadeSpeed * dt;

            if (fadeAlpha <= 0.0f)
            {
                fadeAlpha = 0.0f;
                toMenu = false;
                toGame = false;
                toLose = false;
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
        else if (fadeState == BETWEEN)
        {
            if (toGame) game->LoadGame();
            else if (toMenu) menu->LoadHightScore();

            if (toMenu) state = MENU;
            if (toGame) state = GAME;
            if (toLose) state = LOSE;

            fadeState = FADEIN;
        }
    }
    else
    {
        if (state == MENU)
        {
            if (menu->GetGoGame()) toGame = true;
        }
        else if (state == GAME)
        {
            if (game->GetReturnMain()) toMenu = true;
        }
        if (toMenu || toGame || toLose)
        {
            fadeState = FADEOUT;
            fadeAlpha = 0;
        }
    }

    DrawRectangle(0, 0, App->window->GetWidth(), App->window->GetHeight(), { 0, 0, 0, static_cast<unsigned char>(fadeAlpha) });

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