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
	std::vector<Font> fonts;

	//TRAFFIC LIGHT
	Texture2D trafficLight;
	std::vector<Texture2D> lights;

	//LEADER BOARD
	Texture2D leaderBoard;
	std::vector<Texture2D> numbers;
	std::vector<Texture2D> names;
	Vector2 numbersPosition;
	Vector2 nameDisplacement;
	int numbersSpacing;

	//INGAME LEADERBOARD
	std::vector<Texture2D> pilots;
	Texture2D checkeredLine;
	Vector2 inGameLeaderBoardPosition;

	
};
