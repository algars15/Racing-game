#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

class ModuleGame;
class ModuleMenu;

enum State
{
	MENU,
	GAME,
	TRANSITION,
	LOSE
};

enum FadeState
{
	FADEIN,
	FADEOUT,
	BETWEEN
};

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	ModuleGame* game;
	ModuleMenu* menu;

	State state;

	float fadeAlpha;
	float fadeSpeed;
	FadeState fadeState;
	bool toMenu;
	Music song;
};
