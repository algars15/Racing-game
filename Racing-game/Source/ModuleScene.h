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
	update_status Update(float dt);
	void SetMenu(ModuleMenu* menu);
	void SetGame(ModuleGame* game);
	bool CleanUp();
	State GetState();
	bool GetToMenu();
	FadeState GetFadeState();

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
