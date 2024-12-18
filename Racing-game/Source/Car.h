#pragma once
#include "PhysicEntity.h"
class Car : public PhysicEntity
{

public:
	Car(ModulePhysics* physics, int _x, int _y, int w, int h, Module* _listener, Texture2D _texture, int numCar = 0, bool _draw = true, ObjectType objectType = CAR, b2BodyType colliderType = b2_dynamicBody)
		: PhysicEntity(physics->CreateRectangle(_x, _y, w, h, colliderType, objectType), _listener)
		, texture(_texture), draw(_draw), carNumber(numCar)
	{

	}

	void Start();

	void Update() override;

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override;

private:
	Texture2D texture;
	bool draw;
	int carNumber;
};

