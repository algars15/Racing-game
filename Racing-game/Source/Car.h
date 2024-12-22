#pragma once
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Globals.h"


class Car : public PhysicEntity
{

public:
	Car(ModulePhysics* physics, int _x, int _y, int w, int h, Module* _listener, Texture2D _texture, int numCar = 0, bool _draw = true, ObjectType objectType = CAR, b2BodyType colliderType = b2_dynamicBody)
		: PhysicEntity(physics->CreateRectangle(_x, _y, w, h, colliderType, objectType), _listener)
		, texture(_texture), width(w), height(h), draw(_draw), carNumber(numCar)
	{

	}

	void Start();

	void Update() override;

	void Draw();

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override;

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

	//PHYSICS
	float accelerationForce;

};

