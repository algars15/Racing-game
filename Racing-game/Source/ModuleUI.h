#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"


class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update(float dt);
	void Draw();
	void SetGame(ModuleGame* game);
	bool CleanUp();

private:
	
	ModuleGame* game;

	//FONTS
	Font fontRushDriver;

	//TRAFFIC LIGHT
	Texture2D trafficLight;
	std::vector<Texture2D> lights;
};
