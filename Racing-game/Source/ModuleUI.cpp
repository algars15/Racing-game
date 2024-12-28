#pragma once

#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
#include "Module.h"
#include "Application.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ui";
}

ModuleUI::~ModuleUI()
{

}

bool ModuleUI::Start()
{
	//FONTS
	fontRushDriver = LoadFont(parameters.child("fonts").child("rushDriver").attribute("path").as_string());

	//TRAFFIC LIGHT
	trafficLight = LoadTexture(parameters.child("trafficLight").attribute("path").as_string());

	for (pugi::xml_node lightNode : parameters.child("trafficLight").children("light")) {

		Texture2D light = LoadTexture(lightNode.attribute("path").as_string());
		lights.push_back(light);
	}

	//LEADER BOARD
	leaderBoard = LoadTexture(parameters.child("leaderBoard").attribute("path").as_string());

	bool ret = true;
	return ret;
}


update_status ModuleUI::Update(float dt)
{
	if (App->scene->GetState() == GAME)
	{
		if (game->GetEnded())
		{
			DrawTexture(leaderBoard, App->window->GetWidth() / 2 - leaderBoard.width / 2, 0, WHITE);
			
			
			std::vector<std::string> rankingNames = game->GetRankingNames();

			for (int i = 0; i < rankingNames.size(); i++)
			{
				char carName[30];
				sprintf_s(carName, "%s", rankingNames[i].c_str());
				DrawTextEx(fontRushDriver, carName, { App->window->GetWidth() - MeasureTextEx(fontRushDriver,carName, 50, 5).x - 30, (float)30*i }, 50, 5, WHITE);
			}
		}
		else
		{
			//TRAFFIC LIGHT
			DrawTexture(trafficLight, App->window->GetWidth() / 2 - trafficLight.width / 2, 0, WHITE);
			if (game->GetTime() < 2)
			{
				if (game->GetTime() < 0)
				{
					int redLights = 5 + game->GetTime(); // De -5 a 0, se apagan progresivamente
					for (int i = 0; i < 5; i++)
					{
						if (i < redLights)
						{
							DrawTexture(lights[i], App->window->GetWidth() / 2 - trafficLight.width / 2, 0, GRAY);
						}
						else
						{
							DrawTexture(lights[i], App->window->GetWidth() / 2 - trafficLight.width / 2, 0, RED);
						}
					}
				}
				else
				{
					for (int i = 0; i < 5; i++)
					{
						DrawTexture(lights[i], App->window->GetWidth() / 2 - trafficLight.width / 2, 0, GREEN);
					}
				}
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					DrawTexture(lights[i], App->window->GetWidth() / 2 - trafficLight.width / 2, 0, GRAY);
				}
			}
		}
		

		//POSITIONS
		char lapsText[30];
		sprintf_s(lapsText, "LAP: %d / %d", game->GetCurrentLap(), game->GetLaps());
		DrawTextEx(fontRushDriver, lapsText, { App->window->GetWidth() - MeasureTextEx(fontRushDriver,lapsText, 50, 5).x - 30, 30 }, 50, 5, WHITE);

	}
	return UPDATE_CONTINUE;
}

void ModuleUI::Draw()
{
	/*if (!mort)
	{
		for (int i = 0; i < puntuacionesFlotantes.size(); i++)
		{
			char buffer[16];
			sprintf_s(buffer, "%d", puntuacionesFlotantes[i]->puntuacion);
			DrawText(buffer, puntuacionesFlotantes[i]->x, puntuacionesFlotantes[i]->y, 32, floatingPuntuationColors[(puntuacionesFlotantes[i]->puntuacion / 100)-1]);
		}
		DrawTexture(puntosIcon, posPuntos.x, posPuntos.y, WHITE);
		char puntuacion[16];
		sprintf_s(puntuacion, "%d", puntuation);
		DrawText(puntuacion, posPuntos.x + puntosIcon.width + 10, posPuntos.y + 5, 32, WHITE);
		DrawTexture(vidasIcon, posVidas.x, posVidas.y, WHITE);
		char vidas[16];
		sprintf_s(vidas, "%d", lives);
		DrawText(vidas, posVidas.x + vidasIcon.width + 10, posVidas.y + 5, 32, WHITE);
	}
	else
	{
		DrawTexture(loseScreen, 0, 0, WHITE);

		char textoPuntuacion[30];
		sprintf_s(textoPuntuacion, "Score : %i", puntuation);
		DrawText(textoPuntuacion, App->window->GetWidth()/2-MeasureText(textoPuntuacion,32)/2, 600, 32, BLACK);
	}*/
}

void ModuleUI::SetGame(ModuleGame* g)
{
	game = g;
}

bool ModuleUI::CleanUp()
{

	LOG("Unloading Intro scene");

	return true;
}