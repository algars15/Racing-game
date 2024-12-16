#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"


struct PuntuacionFlotante {
	int x, y;               
	int puntuacion;             
	float duracion;         
	float tiempoTranscurrido = 0;  
};

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update();
	void Draw(int puntuation, int lives, bool mort);
	void ShowPuntuation(int puntuacion, int x, int y);
	bool CleanUp();

private:

	std::vector<PuntuacionFlotante*> puntuacionesFlotantes;
	int duracionFlotante;
	Vector2 posVidas;
	Vector2 posPuntos;
	Texture2D vidasIcon;
	Texture2D puntosIcon;
	Texture2D loseScreen;
	Color floatingPuntuationColors[10];
};
