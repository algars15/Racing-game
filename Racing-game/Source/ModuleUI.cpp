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
	bool ret = true;
	return ret;
}


update_status ModuleUI::Update(float dt)
{
	if (App->scene->GetState() == MENU)
	{

	}
	return UPDATE_CONTINUE;
}

void ModuleUI::Draw(int puntuation, int lives, bool mort)
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


bool ModuleUI::CleanUp()
{

	LOG("Unloading Intro scene");

	return true;
}