#pragma once

#include "Globals.h"
#include "Module.h"
#include "p2Point.h"
#include "raylib.h"
#include "Timer.h"
#include <vector>

class PhysBody;
class PhysicEntity;

class Car;

class ModuleUI;

struct RoutePoint
{
	int pointIndex;
	Vector2 position;
	PhysBody* body;
};

struct Ranking
{
	int lap;
	int checkPoint;
	float distanceToNextCheckpoint;
};


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	update_status Update(float dt);
	bool CleanUp();
	bool GetReturnMain();
	void SaveGame();
	void RestartGame();
	void LoadGame();
	void SetUI(ModuleUI* ui);
	void SetTrack(int track);
	void SetCars(int j1Car, int j2Car);

private:

	//TEXTURES
	Texture2D mapTexture;
	Texture2D carsTexture;

	//TRACK
	int trackNum;
	std::vector<Vector2> startPoints;
	std::vector<RoutePoint*> routePoints;

	//CARS
	int numPlayers;
	int j1CarNum;
	int j2CarNum;
	std::vector<Car*> cars;
	std::vector<int> ranking;
	Timer sortTimer;
	float sortTime;

	//PHYSICS
	std::vector<PhysicEntity*> entities;

	//UI
	ModuleUI* ui;
};
