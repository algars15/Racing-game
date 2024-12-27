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
	update_status Update(float dt);
	void Draw(int puntuation, int lives, bool mort);
	bool CleanUp();

private:

};
