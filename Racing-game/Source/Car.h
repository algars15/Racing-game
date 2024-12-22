#pragma once
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Globals.h"
#include "Timer.h"


class Car : public PhysicEntity
{

public:
	Car(ModulePhysics* physics, int _x, int _y, int w, int h, Texture2D _texture, int numCar = 0, bool _ia = true, bool _draw = true, ObjectType objectType = CAR, b2BodyType colliderType = b2_dynamicBody)
		: PhysicEntity(physics->CreateRectangle(_x, _y, w, h, colliderType, objectType))
		, texture(_texture), width(w), height(h), draw(_draw), carNumber(numCar), ia(_ia)
	{

	}

	void Start();

	void Update() override;

	void Input();

	void Ia();

	void Draw();

	void SetParameters(pugi::xml_node parameters);

	void SetRoute(std::vector<Vector2> routePoints);

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override;

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

private:
	//TEXTURE
	Texture2D texture;
	int carNumber;
	int width;
	int height;
	bool draw;

	//BEHAVIOUR
	bool ia;
	KeyboardKey forwardKey;
	KeyboardKey backKey;
	KeyboardKey rightKey;
	KeyboardKey leftKey;
	Vector2 input;

	//PHYSICS
	float turnSpeed;
	float acceleration;
	float deceleration;
	float maxSpeed;
	float reverseMaxSpeed;

	//PARAMETERS
	pugi::xml_node carNode;

	//TRACK
	std::vector<Vector2> routePoints;
	int currentWaypointIndex;
	float waypointRadius;
};

