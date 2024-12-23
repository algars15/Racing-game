#pragma once
#include "ModulePhysics.h"
#include "p2Point.h"


class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body)
	{
		body = _body;
		body->listener = this;
	}
	
	

public:
	virtual ~PhysicEntity() = default;
	
	virtual void Update(float dt) = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	};

	void SetParameters(pugi::xml_node p)
	{
		parameters = p;
	};

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB)
	{

	};

public:
	PhysBody* body;
	Module* listener;

protected:
	pugi::xml_node parameters;
};