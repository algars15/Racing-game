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
	fonts.push_back(fontRushDriver);

	//TRAFFIC LIGHT
	trafficLight = LoadTexture(parameters.child("trafficLight").attribute("path").as_string());

	for (pugi::xml_node lightNode : parameters.child("trafficLight").children("light")) {

		Texture2D light = LoadTexture(lightNode.attribute("path").as_string());
		lights.push_back(light);
	}

	//LEADER BOARD
	leaderBoard = LoadTexture(parameters.child("leaderBoard").attribute("path").as_string());
	for (pugi::xml_node nameNode : parameters.child("leaderBoard").children("names")) {

		Texture2D name = LoadTexture(nameNode.attribute("path").as_string());
		names.push_back(name);
	}
	for (pugi::xml_node numberNode : parameters.child("leaderBoard").children("number")) {

		Texture2D number = LoadTexture(numberNode.attribute("path").as_string());
		numbers.push_back(number);
	}
	numbersPosition = { parameters.child("leaderBoard").attribute("x").as_float(), parameters.child("leaderBoard").attribute("y").as_float() };
	nameDisplacement = { parameters.child("leaderBoard").attribute("displacementX").as_float(), parameters.child("leaderBoard").attribute("displacementY").as_float() };
	numbersSpacing = parameters.child("leaderBoard").attribute("numbersSpacing").as_int();

	//INGAME LEADER BOARD
	checkeredLine = LoadTexture(parameters.child("inGameLeaderBoard").attribute("path").as_string());
	for (pugi::xml_node pilotNode : parameters.child("inGameLeaderBoard").children("pilot")) {

		Texture2D pilot = LoadTexture(pilotNode.attribute("path").as_string());
		pilots.push_back(pilot);
	}
	inGameLeaderBoardPosition = { parameters.child("inGameLeaderBoard").attribute("x").as_float() , parameters.child("inGameLeaderBoard").attribute("y").as_float() };

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
			
			
			std::vector<int> rankingNumbers = game->GetRankingNums();

			Vector2 currentNumPosition = numbersPosition;
			for (int i = 0; i < rankingNumbers.size(); i++)
			{
				DrawTexture(numbers[i], currentNumPosition.x, currentNumPosition.y, WHITE);
				DrawTexture(names[rankingNumbers[i]], currentNumPosition.x + nameDisplacement.x, currentNumPosition.y + nameDisplacement.y, WHITE);
				currentNumPosition.y += numbersSpacing;
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
					int redLights = 6 + game->GetTime(); // From -5 to 0, they turn off progressively
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

			Vector2 currentPosition = inGameLeaderBoardPosition;
			DrawTexture(checkeredLine, currentPosition.x, currentPosition.y, WHITE);
			currentPosition.y += checkeredLine.height;
			for (int i = 0; i < game->GetRankingNums().size(); i++)
			{
				DrawTexture(pilots[game->GetRankingNums()[i]], currentPosition.x, currentPosition.y, WHITE);
				currentPosition.y += pilots[game->GetRankingNums()[i]].height;
			}
			DrawTexture(checkeredLine, currentPosition.x, currentPosition.y, WHITE);
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
	
}

void ModuleUI::SetGame(ModuleGame* g)
{
	game = g;
}

bool ModuleUI::CleanUp()
{
	for each (Font font in fonts)
	{
		UnloadFont(font);
	}

	LOG("Unloading Intro scene");

	return true;
}