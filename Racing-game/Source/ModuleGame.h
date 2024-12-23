#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;

class Car;

class b2RevoluteJoint;
class b2PrismaticJoint;
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

public:

	//TEXTURES
	Texture2D mapTexture;
	Texture2D carsTexture;

	//TRACK
	std::vector<Vector2> startPoints;
	std::vector<RoutePoint*> routePoints;

	//CARS
	std::vector<Car*> cars;

	//PHYSICS
	std::vector<PhysicEntity*> entities;

	//UI
	ModuleUI* ui;
};
