#pragma once
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Globals.h"
#include "Timer.h"
#include "ModuleGame.h"


class Car : public PhysicEntity
{

public:
	Car(ModulePhysics* physics, int _x, int _y, int w, int h, Texture2D _texture, int numCar = 0, bool _ia = true, bool _draw = true, ObjectType objectType = CAR, b2BodyType colliderType = b2_dynamicBody)
		: PhysicEntity(physics->CreateRectangle(_x, _y, w, h, colliderType, objectType))
		, texture(_texture), width(w), height(h), draw(_draw), carNumber(numCar), ia(_ia)
	{

	}

	void Start();

	void Update(float dt) override;

	void Input();

	void Ia();

	void Draw();

	void SetRoute(std::vector<RoutePoint*> routePoints);

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override;

	Ranking GetRank();

	int GetCarNum();
	
	std::string GetCarName();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

	void SetKeys(KeyboardKey keyUp, KeyboardKey keyDown, KeyboardKey keyRight, KeyboardKey keyLeft, KeyboardKey keyBoost);
	
	void SetGame(ModuleGame* game);

	void SetRotation(float degrees);

	void SetName(std::string name);

	float GetTime();

	void CleanUp() override;

private:
	
	ModuleGame* game;
	
	std::string name;
	float raceTime;

	//TEXTURE
	Texture2D texture;
	int carNumber;
	int width;
	int height;
	bool draw;

	//BEHAVIOUR
	bool ia;
	bool drs;
	bool ended;
	KeyboardKey forwardKey;
	KeyboardKey backKey;
	KeyboardKey rightKey;
	KeyboardKey leftKey;
	KeyboardKey boostKey;
	Vector2 input;
	Timer stuckedTimer;


	//PHYSICS
	float turnSpeed;
	float turnSpeedDrs;
	float acceleration;
	float deceleration;
	float maxSpeed;
	float drsMaxSpeed;
	float reverseMaxSpeed;

	//PARAMETERS
	pugi::xml_node carNode;

	//TRACK
	std::vector<RoutePoint*> routePoints;
	int nextWaypointIndex;
	int currentWaypointIndex;
	int previousWaypointIndex;
	Vector2 nextWaypointPos;
	Ranking ranking;

	//AUDIO
	Sound carSound;
	float defaultPitch;
};

