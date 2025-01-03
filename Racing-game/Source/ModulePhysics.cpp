#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

#include "p2Point.h"

#include <math.h>

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	name = "physics";
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	int x = (int)(SCREEN_WIDTH / 2);
	int y = (int)(SCREEN_HEIGHT / 1.5f);
	int diameter = SCREEN_WIDTH / 2;

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			b2BodyUserData data1 = c->GetFixtureA()->GetBody()->GetUserData();
			b2BodyUserData data2 = c->GetFixtureB()->GetBody()->GetUserData();

			PhysBody* pb1 = (PhysBody*)data1.pointer;
			PhysBody* pb2 = (PhysBody*)data2.pointer;
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType colliderType, ObjectType objectType)
{
	PhysBody* pbody = new PhysBody();
	

	b2BodyDef body;
	body.type = colliderType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = pbody->height = radius;
	pbody->objectType = objectType;

	list_physBodys.push_back(pbody);
	return pbody;
}
PhysBody* ModulePhysics::CreateCircleSensor(int x, int y, int radius, b2BodyType colliderType, ObjectType objectType)
{
	PhysBody* pbody = new PhysBody();


	b2BodyDef body;
	body.type = colliderType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = pbody->height = radius;
	pbody->objectType = objectType;

	list_physBodys.push_back(pbody);
	return pbody;
}


PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType colliderType, ObjectType objectType)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = colliderType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);
	pbody->objectType = objectType;

	list_physBodys.push_back(pbody);
	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, b2BodyType colliderType, ObjectType objectType)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = colliderType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = width;
	pbody->height = height;
	pbody->objectType = objectType;

	list_physBodys.push_back(pbody);
	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, const int* points, int size, b2BodyType colliderType, ObjectType objectType)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(int i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	pbody->body = b;
	pbody->width = pbody->height = 0;
	pbody->objectType = objectType;

	list_physBodys.push_back(pbody);
	return pbody;
}

update_status ModulePhysics::PostUpdate()
{
	if (!App->GetDebug())
	{
		if (mouse_joint)
		{
			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
		}
		return UPDATE_CONTINUE;
	}
	b2Body* mouseSelect = nullptr;
	Vector2 mousePosition = GetMousePosition();
	b2Vec2 pMousePosition = b2Vec2(PIXEL_TO_METERS(mousePosition.x), PIXEL_TO_METERS(mousePosition.y));

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					
					DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{0, 0, 0, 128});
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->m_count;
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->m_vertices[i]);
						if(i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
				}
				break;
			}

			//Mouse Joint
			if (mouse_joint == nullptr && mouseSelect == nullptr && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

				if (f->TestPoint(pMousePosition)) {
					mouseSelect = b;
				}
			}
		}
	}

	if (mouseSelect) {
		b2MouseJointDef def;

		def.bodyA = ground;
		def.bodyB = mouseSelect;
		def.target = pMousePosition;
		def.damping = 0.5f;
		def.stiffness = 20.f;
		def.maxForce = 100.f * mouseSelect->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	
	else if (mouse_joint && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		mouse_joint->SetTarget(pMousePosition);
		b2Vec2 anchorPosition = mouse_joint->GetBodyB()->GetPosition();
		anchorPosition.x = METERS_TO_PIXELS(anchorPosition.x);
		anchorPosition.y = METERS_TO_PIXELS(anchorPosition.y);

		DrawLine(anchorPosition.x, anchorPosition.y, mousePosition.x, mousePosition.y, RED);
	}

	
	else if (mouse_joint && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");
	for (auto it = list_physBodys.rbegin(); it != list_physBodys.rend(); ++it)
	{
		PhysBody* item = *it;
		delete item;
	}

	// Delete the whole physics world!
	delete world;
	return true;
}

bool ModulePhysics::ClearWorld()
{
	TraceLog(LOG_INFO,"Clearing physics");

	if (mouse_joint)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}

	// Destroy all bodies and listeners
	for (auto it = list_physBodys.rbegin(); it != list_physBodys.rend(); ++it)
	{
		PhysBody* item = *it;

		if (item->body != nullptr)
		{
			world->DestroyBody(item->body);  // Destroy Box2D body
			item->body = nullptr;
		}

		item->listener = nullptr;  // Unlink listener
		delete item;               // Free PhysBody memory
	}

	list_physBodys.clear(); // Free body list

	return true;
}

void PhysBody::GetPhysicPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
}

Vector2 PhysBody::GetPosition() const
{
	b2Vec2 pos = body->GetPosition();
	return { (float)METERS_TO_PIXELS(pos.x) , (float)METERS_TO_PIXELS(pos.y) };
}

float PhysBody::GetRotation() const
{
	return body->GetAngle();
}

void PhysBody::ApplyForce(Vector2 force)
{
	body->ApplyForceToCenter({ force.x, force.y }, true);
}
void PhysBody::ApplyAngularImpulse(float impulse)
{
	body->ApplyAngularImpulse( impulse, true);
}

void PhysBody::ApplyTorque(float torque)
{
	body->ApplyTorque(torque, true);
}

Vector2 PhysBody::GetLinearVelocity() const
{
	b2Vec2 velocity = body->GetLinearVelocity();
	return { velocity.x, velocity.y };
}

Vector2 PhysBody::GetLinearVelocityNormalized() const
{
	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.Normalize();
	return { velocity.x, velocity.y };
}

void PhysBody::SetLinearVelocity(Vector2 velocity) const
{
	body->SetLinearVelocity({ velocity.x, velocity.y });
}

void PhysBody::SetRotation(float rotation) const
{
	body->SetTransform({ body->GetPosition().x, body->GetPosition().y }, rotation);
}

float PhysBody::GetVelocity() const
{
	return body->GetLinearVelocity().Length();
}

Vector2 PhysBody::GetWorldVector(Vector2 axis) const
{
	b2Vec2 worldVector = body->GetWorldVector(b2Vec2(axis.x, axis.y));
	return { worldVector.x, worldVector.y };
}

Vector2 PhysBody::GetLateralVelocity() const
{
	b2Vec2 currentRightNormal = body->GetWorldVector(b2Vec2(1, 0));
	b2Vec2 lateralVelocity = b2Dot(currentRightNormal, body->GetLinearVelocity()) * currentRightNormal;
	return { lateralVelocity.x, lateralVelocity.y };
}
Vector2 PhysBody::GetForwardVelocity() const
{
	b2Vec2 currentFrontNormal = body->GetWorldVector(b2Vec2(0, 1));
	b2Vec2 forwardVelocity = b2Dot(currentFrontNormal, body->GetLinearVelocity()) * currentFrontNormal;
	return { forwardVelocity.x, forwardVelocity.y };
}
float PhysBody::GetMass() const
{
	return body->GetMass();
}
float PhysBody::GetAngularVelocity() const
{
	return body->GetAngularVelocity();
}
float PhysBody::GetInertia() const
{
	return body->GetInertia();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = (float)(x2 - x1);
			float fy = (float)(y2 - y1);
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return (int)(output.fraction * dist);
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& distance) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = (float)(x2 - x1);
			float fy = (float)(y2 - y1);
			float dist = sqrtf((fx * fx) + (fy * fy));

			distance = dist;

			return (int)(output.fraction * dist);
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void PhysBody::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	b2BodyUserData dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	b2BodyUserData dataB = contact->GetFixtureB()->GetBody()->GetUserData();

	PhysBody* physA = (PhysBody*)dataA.pointer;
	PhysBody* physB = (PhysBody*)dataB.pointer;

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	if (physA && physA->listener != nullptr)
		physA->listener->OnCollision(physA, physB);
	else {
		// Debug: Log the issue of invalid or deleted physA
		LOG("physA is invalid or has been deleted.");
	}

	if (physB && physB->listener != nullptr)
		physB->listener->OnCollision(physB, physA);
	else {
		// Debug: Log the issue of invalid or deleted physB
		LOG("physB is invalid or has been deleted.");
	}
}

b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 anchor, b2Vec2 angle)
{
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(bodyA->body, bodyB->body, anchor);
	jointDef.enableLimit = true;
	jointDef.lowerAngle = angle.x;
	jointDef.upperAngle = angle.y;
	jointDef.enableMotor = true;
	jointDef.motorSpeed = 0.0f;
	jointDef.maxMotorTorque = 300.0f;
	b2RevoluteJoint* joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
	return joint;
}

b2PrismaticJoint* ModulePhysics::CreatePrismaticJoint(PhysBody* body, int p1X, int p2X, int p1Y, int p2Y) {
	
	b2Body* ground = NULL;
	{
		b2BodyDef bd;
		ground = world->CreateBody(&bd);

		b2EdgeShape shape;
		shape.SetTwoSided(b2Vec2(p1X, p1Y), b2Vec2(p2X, p2Y));
		ground->CreateFixture(&shape, 0.0f);
	}

	

	b2PrismaticJointDef jointDef;
	jointDef.Initialize(ground, body->body, ground->GetWorldCenter(), b2Vec2(0.0f, 1.0f));
	jointDef.enableLimit = true;
	jointDef.lowerTranslation = 0;
	jointDef.upperTranslation = 2.6f;
	jointDef.enableMotor = true;
	jointDef.maxMotorForce = 1000.0f;
	jointDef.motorSpeed = 0;
	

	b2PrismaticJoint* joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);
	return joint;
}
