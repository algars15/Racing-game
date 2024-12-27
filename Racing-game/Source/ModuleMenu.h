#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"



class ModuleMenu : public Module
{
	enum State
	{
		MENU,
		RACE_SELECTION,
		CAR_SELECTION
	};

public:
	ModuleMenu(Application* app, bool start_enabled = true);
	~ModuleMenu();

	bool Start();
	update_status Update(float dt);
	void LoadHightScore();
	bool CleanUp();
	bool GetGoGame();


private:
	
	//TEXTURES
	Texture2D menuTexture;
	Texture2D raceSelectionTexture;
	Texture2D carSelectionBkgTexture;
	Texture2D carSelectionCarsTexture;
	Texture2D trackSelector;
	Texture2D carSelector;

	//BEHAVIOUR
	State state;
	int trackSelected;
	int j1CarSelected;
	int j2CarSelected;
	bool goGame;

	std::vector<Vector2> trackPositions;
	std::vector<Vector2> carPositions;
};
