#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d.h"
#include <vector>

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum ObjectType
{
	UNKNOWN,
	MAP_COLLIDER,
	ROUTE_SENSOR,
	CAR
};

class PhysicEntity;

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{
	}

	//void GetPosition(int& x, int& y) const;
	void GetPhysicPosition(int& x, int &y) const;
	Vector2 GetPosition() const;
	float GetRotation() const;
	Vector2 GetWorldVector(Vector2 axis) const;
	Vector2 GetLinearVelocity() const;
	Vector2 GetLinearVelocityNormalized() const;
	Vector2 GetLateralVelocity() const;
	Vector2 GetForwardVelocity() const;
	float GetAngularVelocity() const;
	float GetVelocity() const;
	float GetInertia() const;
	float GetMass() const;
	void SetLinearVelocity(Vector2 velocity) const;
	void SetRotation(float rotation) const;
	void ApplyForce(Vector2 force);
	void ApplyTorque(float torque);
	void ApplyAngularImpulse(float angular);
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& distance) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	ObjectType objectType;
	int width, height;
	b2Body* body;
	PhysicEntity* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();
	bool ClearWorld();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType colliderType = b2_dynamicBody, ObjectType objectType = UNKNOWN);
	PhysBody* CreateCircleSensor(int x, int y, int radius, b2BodyType colliderType = b2_dynamicBody, ObjectType objectType = UNKNOWN);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType colliderType = b2_dynamicBody, ObjectType objectType = UNKNOWN);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType colliderType = b2_dynamicBody, ObjectType objectType = UNKNOWN);
	PhysBody* CreateChain(int x, int y, const int* points, int size, b2BodyType colliderType = b2_dynamicBody, ObjectType objectType = UNKNOWN);

	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 anchor, b2Vec2 angle = b2Vec2_zero);
	b2PrismaticJoint* CreatePrismaticJoint(PhysBody* bodyA, int p1X, int p2X, int p1Y, int p2Y);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	std::vector<PhysBody*> list_physBodys;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};