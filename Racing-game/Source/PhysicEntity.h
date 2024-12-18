#pragma once
#include "ModulePhysics.h"
#include "p2Point.h"


class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		body->listener = listener;
	}

public:
	virtual ~PhysicEntity() = default;
	
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	};

	void SetParameters(pugi::xml_node p)
	{
		parameters = p;
	};

public:
	PhysBody* body;
	Module* listener;

protected:
	pugi::xml_node parameters;
};