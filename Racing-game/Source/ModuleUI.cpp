#pragma once

#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
#include "Module.h"
#include "Application.h"


ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleUI::~ModuleUI()
{

}

bool ModuleUI::Start()
{
	
	duracionFlotante = 2;
	LOG("Loading Intro assets"); 
	vidasIcon = LoadTexture("Assets/Vidas.png");
	puntosIcon = LoadTexture("Assets/Puntos.png");
	loseScreen = LoadTexture("Assets/lose_screen.png");
	posVidas = { 10, 50 };
	posPuntos = {10, 10};
	bool ret = true;

	floatingPuntuationColors[0] = DARKBLUE;      
	floatingPuntuationColors[1] = BLUE;
	floatingPuntuationColors[2] = SKYBLUE;
	floatingPuntuationColors[3] = GREEN;
	floatingPuntuationColors[4] = LIME;
	floatingPuntuationColors[5] = YELLOW;
	floatingPuntuationColors[6] = GOLD;
	floatingPuntuationColors[7] = ORANGE;
	floatingPuntuationColors[8] = RED;
	floatingPuntuationColors[9] = MAROON;


	return ret;
}

void ModuleUI::ShowPuntuation(int p, int x, int y)
{
	PuntuacionFlotante* pf = new PuntuacionFlotante();
	pf->puntuacion = p;
	pf->x = x;
	pf->y = y;
	pf->duracion = duracionFlotante;
	pf->tiempoTranscurrido = 0;
	puntuacionesFlotantes.push_back(pf);
}

update_status ModuleUI::Update()
{
	for (int i = 0; i < puntuacionesFlotantes.size(); i++)
	{
		puntuacionesFlotantes[i]->tiempoTranscurrido += GetFrameTime();
		if (puntuacionesFlotantes[i]->tiempoTranscurrido > puntuacionesFlotantes[i]->duracion)
		{
			delete(puntuacionesFlotantes[i]);
			puntuacionesFlotantes.erase(puntuacionesFlotantes.begin() + i);
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleUI::Draw(int puntuation, int lives, bool mort)
{
	if (!mort)
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
	}
}


bool ModuleUI::CleanUp()
{
	for (auto it = puntuacionesFlotantes.rbegin(); it != puntuacionesFlotantes.rend(); ++it)
	{
		PuntuacionFlotante* item = *it;
		delete item;
	}

	LOG("Unloading Intro scene");

	return true;
}