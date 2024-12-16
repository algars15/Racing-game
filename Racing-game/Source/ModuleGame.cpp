#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
#include "fstream"



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
	}

public:
	PhysBody* body;
	Module* listener;
};

class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, int radious, Module* _listener, Texture2D _texture, bool _draw = true, ObjectType objectType = UNKNOWN, b2BodyType colliderType = b2_dynamicBody)
		: PhysicEntity(physics->CreateCircle(_x, _y, radious, colliderType, objectType), _listener)
		, texture(_texture), draw(_draw)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		if (draw)
		{
			Vector2 position{ (float)x, (float)y };
			float scale = 1.0f;
			Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
			Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
			Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f};
			float rotation = body->GetRotation() * RAD2DEG;
			DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
		}
		
	}

private:
	Texture2D texture;
	bool draw;
};

class CircleSensor : public PhysicEntity
{
public:
	CircleSensor(ModulePhysics* physics, int _x, int _y, int radious, Module* _listener, Texture2D _texture, bool _draw = true, ObjectType objectType = UNKNOWN, b2BodyType colliderType = b2_dynamicBody)
		: PhysicEntity(physics->CreateCircleSensor(_x, _y, radious, colliderType, objectType), _listener)
		, texture(_texture), draw(_draw)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		if (draw)
		{
			Vector2 position{ (float)x, (float)y };
			float scale = 1.0f;
			Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
			Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
			Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
			float rotation = body->GetRotation() * RAD2DEG;
			DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
		}

	}

private:
	Texture2D texture;
	bool draw;
};

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, int w, int h, Module* _listener, Texture2D _texture, bool _draw = true, ObjectType objectType = UNKNOWN, b2BodyType colliderType = b2_dynamicBody)
		: PhysicEntity(physics->CreateRectangle(_x, _y, w, h, colliderType, objectType), _listener)
		, texture(_texture), draw(_draw)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		if (draw)
		{
			DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
				Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
				Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
		}
		
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;
	bool draw;
};

class Shape : public PhysicEntity
{
public:
	// Pivot 0, 0


	Shape(ModulePhysics* physics, int _x, int _y, int points[], int numberOfPoints, Module* _listener, Texture2D _texture, bool _draw = true, ObjectType objectType = UNKNOWN,  b2BodyType colliderType = b2_dynamicBody)
		: PhysicEntity(physics->CreateChain(_x , _y , points, numberOfPoints, colliderType, objectType), _listener)
		, texture(_texture), draw(_draw)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		if(draw) DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 1.0f, WHITE);
	}
private:
	Texture2D texture;
	bool draw;
};


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "game";
	ui = new ModuleUI(app);
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	

	return ret;
}

void ModuleGame::RestartGame()
{
	
}


// Load assets
bool ModuleGame::CleanUp()
{
	for (auto it = entities.rbegin(); it != entities.rend(); ++it)
	{
		PhysicEntity* item = *it;
		delete item;
	}
	entities.clear();
	/*ui->CleanUp();
	delete ui;*/
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	
	return UPDATE_CONTINUE;
}


void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB, Vector2 normal)
{
	
}

void ModuleGame::SaveGame() {
	
	
}

bool ModuleGame::GetReturnMain()
{
	return false;
}