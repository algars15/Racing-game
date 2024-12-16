#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;

class Box; 
class Circle;
class Shape;
class CircleSensor;

class b2RevoluteJoint;
class b2PrismaticJoint;
class ModuleUI;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, Vector2 normal);
	bool GetReturnMain();
	void SaveGame();
	void RestartGame();

public:

	std::vector<PhysicEntity*> entities;
	ModuleUI* ui;
};
