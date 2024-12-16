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
	
}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Start()
{
	menu = new ModuleMenu(App);
    menu->Start();
    game = new ModuleGame(App);
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
    song = LoadMusicStream("Assets/intro song.wav");
    SetMusicVolume(song, 0.8f);
    PlayMusicStream(song);

	return ret;
}

update_status ModuleScene::Update()
{
    UpdateMusicStream(song);

    if (state == MENU)
    {
        menu->Update();
        

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
        game->Update();
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
            fadeAlpha -= fadeSpeed * GetFrameTime(); 
            if (toMenu) menu->Update();
            else game->Update();

            if (fadeAlpha <= 0.0f)
            {
                fadeAlpha = 0.0f;
                if (toMenu) state = MENU;
                else state = GAME;
            }
        }
        else if (fadeState == FADEOUT)
        {
            fadeAlpha += fadeSpeed * GetFrameTime();
            if (toMenu) game->Update();
            else menu->Update();
            
            if (fadeAlpha >= 255.0f)
            {
                fadeState = BETWEEN;
                fadeAlpha = 255.0f;
            }
           
        }
        else
        {
            fadeState = FADEIN;
            if (!toMenu) game->RestartGame();
            else menu->LoadHightScore();
        }

        // Dibujar fade
        DrawRectangle(0, 0, App->window->GetWidth(), App->window->GetHeight(), { 0, 0, 0, static_cast<unsigned char>(fadeAlpha)});
    }

    return UPDATE_CONTINUE;
}


// Load assets
bool ModuleScene::CleanUp()
{
    UnloadMusicStream(song);
    menu->CleanUp();
    game->CleanUp();
    delete menu;
    delete game;
	LOG("Unloading Intro scene");

	return true;
}