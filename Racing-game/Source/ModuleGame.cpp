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
	ray_on = false;
	sensed = false;
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	pimball_map = LoadTexture("Assets/map.png");
	circle = LoadTexture("Assets/bola2.png"); 
	circle_extra= LoadTexture("Assets/bolaExtra.png"); 
	box = LoadTexture("Assets/crate.png");
	palancaTexture = LoadTexture("Assets/palanca.png");
	palanca_invertida = LoadTexture("Assets/palanca_inverted.png");
	spring = LoadTexture("Assets/Spring.png");
	pimball_map2 = LoadTexture("Assets/map2.png");
	vidasTexture = LoadTexture("Assets/Vidas1.png");
	
	springSound = App-> audio->LoadFx("Assets/springy.wav");
	flipperSound = App-> audio->LoadFx("Assets/flipper.wav");
	bouncerSound = App-> audio->LoadFx("Assets/bonus.wav");
	song = App-> audio->LoadFx("Assets/intro song.wav");
	hitSound = App-> audio->LoadFx("Assets/hit.wav");
	fallSound = App-> audio->LoadFx("Assets/ball-falls.wav");
	pasarela = App-> audio->LoadFx("Assets/magic.wav");
	newBallSound = App-> audio->LoadFx("Assets/new-ball.wav");
	boingSound = App-> audio->LoadFx("Assets/boing.wav");
	healSound = App-> audio->LoadFx("Assets/heal.wav");
	
	ui = new ModuleUI(App);
	ui->Start();

	velocitatPalanca = 20;
	forcaImpuls = 4;
	startPos = { 465, 310 };
	mollaLliberada = true;
	timeToCombo = 5;
	timeToVida = 30;
	vidaSpawns[0] = { 225,250 };
	vidaSpawns[1] = { 45,265 };
	vidaSpawns[2] = { 415,260 };

	//MAPA
	int map[82] = {
	480, 0,
	480, 640,
	360, 640,
	413, 605,
	413, 475,
	406, 463,
	397, 458,
	397, 433,
	416, 414,
	429, 379,
	429, 84,
	421, 71,
	402, 65,
	367, 87,
	355, 88,
	393, 51,
	417, 52,
	438, 66,
	447, 95,
	447, 620,
	480, 620,
	480, 70,
	474, 41,
	453, 15,
	432, 5,
	98, 5,
	76, 16,
	54, 42,
	35, 95,
	30, 151,
	30, 178,
	18, 192,
	18, 325,
	27, 355,
	52, 383,
	52, 443,
	33, 465,
	33, 605,
	88, 640,
	0, 640,
	0, 0
	};

	int map1[36] = {
	295, 283,
	351, 256,
	371, 233,
	373, 191,
	373, 146,
	381, 141,
	389, 141,
	395, 147,
	395, 183,
	395, 284,
	395, 348,
	388, 355,
	377, 356,
	311, 311,
	300, 311,
	304, 302,
	303, 291,
	299, 286
	};

	int map2[42] = {
	82, 206,
	90, 216,
	102, 255,
	117, 269,
	154, 284,
	141, 287,
	141, 301,
	155, 315,
	144, 316,
	106, 352,
	76, 327,
	81, 313,
	69, 298,
	80, 279,
	69, 273,
	69, 266,
	81, 248,
	68, 240,
	67, 231,
	73, 226,
	73, 212
	};

	int map3[20] = {
	65, 133,
	66, 115,
	74, 84,
	96, 47,
	106, 42,
	118, 41,
	128, 48,
	144, 61,
	145, 75,
	74, 142
	};

	int map4[14] = {
	76, 523,
	82, 517,
	90, 522,
	90, 571,
	155, 604,
	137, 607,
	77, 578
	};

	int map5[14] = {
	292, 604,
	357, 571,
	357, 523,
	363, 520,
	370, 525,
	370, 576,
	307, 605
	};

	int map6[12] = {
	169, 399,
	169, 353,
	176, 348,
	182, 353,
	182, 398,
	175, 404
	};
	
	int map7[14] = {
	216, 354,
	223, 347,
	230, 353,
	231, 395,
	223, 403,
	217, 395,
	216, 355
	};


	int map8[14] = {
	264, 357,
	271, 347,
	278, 354,
	278, 395,
	272, 401,
	265, 395,
	265, 358
	};

	int map9[10] = {
	289, 542,
	315, 497,
	321, 500,
	320, 528,
	297, 544
	};

	int map10[10] = {
	154, 546,
	125, 530,
	125, 500,
	133, 496,
	160, 540,
	};

	//MAP
	entities.emplace_back(new Shape(App->physics, 0, 0, map, 82, this, pimball_map, true));
	entities.emplace_back(new Shape(App->physics, 0, 0, map1, 36, this, pimball_map, false));
	entities.emplace_back(new Shape(App->physics, 0, 0, map2, 42, this, pimball_map, false));
	entities.emplace_back(new Shape(App->physics, 0, 0, map3, 20, this, pimball_map, false));
	entities.emplace_back(new Shape(App->physics, 0, 0, map4, 14, this, pimball_map, false));
	entities.emplace_back(new Shape(App->physics, 0, 0, map5, 14, this, pimball_map, false));
	entities.emplace_back(new Shape(App->physics, 0, 0, map6, 12, this, pimball_map, false));
	entities.emplace_back(new Shape(App->physics, 0, 0, map7, 14, this, pimball_map, false));
	entities.emplace_back(new Shape(App->physics, 0, 0, map8, 14, this, pimball_map, false));
	entities.emplace_back(new Shape(App->physics, 0, 0, map9, 10, this, pimball_map, false, REBOTADOR));
	entities.emplace_back(new Shape(App->physics, 0, 0, map10, 10, this, pimball_map, false, REBOTADOR));
	App->physics->CreateCircle(168, 168, 13, b2_staticBody, BOLA_REBOTADORA);
	App->physics->CreateCircle(296, 168, 13, b2_staticBody, BOLA_REBOTADORA);
	App->physics->CreateCircle(232, 88, 13, b2_staticBody, BOLA_REBOTADORA);
	
	//SENSOR
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 100, SCREEN_WIDTH, 50, b2_staticBody, DETECTOR_MORT);
	sensor = App->physics->CreateRectangleSensor(200, 375, 30, 2, b2_staticBody, PASARELA);
	sensor = App->physics->CreateRectangleSensor(245, 375, 30, 2, b2_staticBody, PASARELA);

	//MOLLA
	molla = new Box(App->physics, 448+spring.width/2, 480-spring.height/2, spring.width, spring.height, this, spring);
	molla->body->body->SetGravityScale(0);
	entities.emplace_back(molla);

	jointMolla = App->physics->CreatePrismaticJoint(molla->body, 448, 480, 480, 480);

	//PALANCAS
	palancaIzquierda = new Box(App->physics, 210 - palanca_invertida.width / 2, 604 + palanca_invertida.height / 2, palanca_invertida.width, palanca_invertida.height, this, palanca_invertida, true,  PALANCA);
	palancaDerecha = new Box(App->physics, 298 - palancaTexture.width / 2, 604 + palancaTexture.height / 2, palancaTexture.width, palancaTexture.height, this, palancaTexture, true, PALANCA);

	palancaIzquierda->body->body->SetGravityScale(0);
	palancaDerecha->body->body->SetGravityScale(0);

	entities.emplace_back(palancaDerecha);
	entities.emplace_back(palancaIzquierda);

	unionPalancaIzquierda = App->physics->CreateCircle(210 - palanca_invertida.width, 604, 3, b2BodyType::b2_staticBody);
	unionPalancaDerecha = App->physics->CreateCircle(298, 604, 3, b2BodyType::b2_staticBody);

	jointPalancaIzquierda = App->physics->CreateRevoluteJoint(palancaIzquierda->body, unionPalancaIzquierda, unionPalancaIzquierda->body->GetWorldCenter(), { -0.50, 0.50 });
	jointPalancaDerecha = App->physics->CreateRevoluteJoint(palancaDerecha->body, unionPalancaDerecha, unionPalancaDerecha->body->GetWorldCenter(), { -0.50, 0.50 });


	//BOLA
	bola = new Circle(App->physics, startPos.x, startPos.y, circle.width / 2, this, circle, true, BOLA);
	entities.emplace_back(bola);


	//BOLAS EXTRAS
	for (int i = 0; i < 5; i++)
	{
		Circle* extraBall = new Circle(App->physics, startPos.x, startPos.y, circle.width / 2, this, circle_extra, true, BOLA_EXTRA);
		entities.emplace_back(extraBall);
		bolasExtras.emplace_back(extraBall);
	}
	
	//VIDAS
	vida = new CircleSensor(App->physics, startPos.x, startPos.y, vidasTexture.width / 2, this, vidasTexture, true, VIDA);
	vida->body->body->SetGravityScale(0);
	entities.emplace_back(vida);

	return ret;
}

void ModuleGame::RestartGame()
{
	timerVida = timeToVida;
	puntuacio = 0;
	mort = false;
	respawn = false;
	returnMain = false;
	vides = 4;
	bola->body->body->SetLinearVelocity({ 0,0 });
	bola->body->body->SetAngularVelocity({ 0 });
	bola->body->body->SetTransform({ PIXEL_TO_METERS(startPos.x),PIXEL_TO_METERS(startPos.y) }, 0);
	timerCombo = 0;
	comboCounter = 0;
	for (Circle* bola : bolasExtras)
	{
		bola->body->body->SetTransform({ PIXEL_TO_METERS(-100),PIXEL_TO_METERS(-100) }, 0);
		bola->body->body->SetEnabled(false);
	}
	bolaToDisable = nullptr;
	bolaToEnable = nullptr;
	
	vida->body->body->SetTransform({ PIXEL_TO_METERS(-200),PIXEL_TO_METERS(-200) }, 0);
	vida->body->body->SetEnabled(false);
	hideVida = false;
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
	ui->CleanUp();
	delete ui;
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	float translation = jointMolla->GetJointTranslation();
	float upperLimit = jointMolla->GetUpperLimit();
	float lowerLimit = jointMolla->GetLowerLimit();

	if (!mort)
	{
		if (IsKeyDown(KEY_DOWN) && translation < upperLimit - 0.01f)
		{
			jointMolla->SetMotorSpeed(5.0f);
			jointMolla->SetMaxMotorForce(5.0f);
			mollaLliberada = false;

		}
		else if (IsKeyUp(KEY_DOWN) && translation > lowerLimit + 0.01f)
		{
			jointMolla->SetMotorSpeed(-200.0f);
			jointMolla->SetMaxMotorForce(200.0f);
			if (!mollaLliberada)
			{
				mollaLliberada = true;
				App->audio->PlayFx(springSound);
			}
		}
		else
		{
			molla->body->body->SetLinearVelocity({ 0,0 });
			jointMolla->SetMotorSpeed(0);
			jointMolla->SetMaxMotorForce(0);
		}

		UpdateFlipper(jointPalancaIzquierda, IsKeyDown(KEY_LEFT), false);
		UpdateFlipper(jointPalancaDerecha, IsKeyDown(KEY_RIGHT), true);
	}
	else
	{
		if (IsKeyPressed(KEY_SPACE)) returnMain = true;
	}

	timerCombo -= GetFrameTime();
	if (timerCombo <= 0)
	{
		comboCounter = 0;
	}

	timerVida -= GetFrameTime();
	if (timerVida < 0 && !vida->body->body->IsEnabled())
	{
		int randomSpawn = GetRandomValue(0, 2);
		vida->body->body->SetEnabled(true);
		vida->body->body->SetLinearVelocity({ 0,0 });
		vida->body->body->SetAngularVelocity({ 0 });
		vida->body->body->SetTransform({ PIXEL_TO_METERS(vidaSpawns[randomSpawn].x),PIXEL_TO_METERS(vidaSpawns[randomSpawn].y) }, 0);
	}

	if (hideVida)
	{
		hideVida = false;
		vides++;
		App->audio->PlayFx(healSound);
		vida->body->body->SetTransform({ PIXEL_TO_METERS(-200),PIXEL_TO_METERS(-200) }, 0);
		vida->body->body->SetEnabled(false);
		timerVida = timeToVida;
	}

	if (bolaToDisable != nullptr)
	{
		bolaToDisable->body->SetTransform({ PIXEL_TO_METERS(-100),PIXEL_TO_METERS(-100) }, 0);
		bolaToDisable->body->SetEnabled(false);
		bolaToDisable = nullptr;
	}

	if (bolaToEnable != nullptr)
	{
		bolaToEnable->body->SetEnabled(true);
		bolaToEnable->body->SetLinearVelocity({ 0,0 });
		bolaToEnable->body->SetAngularVelocity({ 0 });
		bolaToEnable->body->SetTransform({ PIXEL_TO_METERS(startPos.x),PIXEL_TO_METERS(startPos.y) }, 0);
		App->audio->PlayFx(newBallSound);

		
		bolaToEnable = nullptr;
	}

	if (respawn) {
		vides--;
		respawn = false;
		if (vides <= 0) {
			mort = true;
			SaveGame();
		}

		else {

			bola->body->body->SetLinearVelocity({ 0,0 });
			bola->body->body->SetAngularVelocity({ 0 });
			bola->body->body->SetTransform({ PIXEL_TO_METERS(startPos.x),PIXEL_TO_METERS(startPos.y) }, 0);
		}

		//TraceLog(LOG_INFO, "hola");
	}

	ui->Update();

	// Prepare for raycast ------------------------------------------------------
	
	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	for (PhysicEntity* entity : entities)
	{
		entity->Update();
		if (ray_on)
		{
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)
			{
				ray_hit = hit;
			}
		}
		
	}
	DrawTexture(pimball_map2, 0, 0, WHITE);
	ui->Draw(puntuacio, vides, mort);
	

	// ray -----------------
	if(ray_on == true)
	{
		vec2f destination((float)(mouse.x-ray.x), (float)(mouse.y-ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}



	return UPDATE_CONTINUE;
}

void ModuleGame::UpdateFlipper(b2RevoluteJoint* joint, bool isPressed, bool right)
{
	if (isPressed) {
		joint->SetMotorSpeed(right ? -velocitatPalanca : velocitatPalanca);
		//App->audio->PlayFx(flipperSound);
	}
	else {
		joint->SetMotorSpeed(right ? velocitatPalanca/4 : -velocitatPalanca/4);
	}
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB, Vector2 normal)
{
	if (!mort)
	{
		if (bodyA->objectType == ObjectType::BOLA || bodyA->objectType == ObjectType::BOLA_EXTRA || bodyB->objectType == ObjectType::BOLA || bodyB->objectType == ObjectType::BOLA_EXTRA)
		{
			PhysBody* bola = bodyA->objectType == ObjectType::BOLA || bodyA->objectType == ObjectType::BOLA_EXTRA ? bodyA : bodyA;
			PhysBody* object = bodyA->objectType == ObjectType::BOLA || bodyA->objectType == ObjectType::BOLA_EXTRA ? bodyB : bodyA;

			switch (object->objectType)
			{
			case REBOTADOR:
			{
				b2Vec2 impulseForce;
				impulseForce.x = normal.x * forcaImpuls;
				impulseForce.y = normal.y * forcaImpuls;
				bola->body->ApplyLinearImpulseToCenter(impulseForce, true);

				App->audio->PlayFx(boingSound);


				break;
			}
			case BOLA_REBOTADORA:
			{
				b2Vec2 impulseForce;
				impulseForce.x = normal.x * forcaImpuls;
				impulseForce.y = normal.y * forcaImpuls;
				bola->body->ApplyLinearImpulseToCenter(impulseForce, true);

				App->audio->PlayFx(bouncerSound);

				comboCounter++;
				comboCounter = (comboCounter > 10) ? 10 : comboCounter;
				puntuacio += 100 * comboCounter;
				timerCombo = timeToCombo;
				ui->ShowPuntuation(100 * comboCounter, METERS_TO_PIXELS(bola->body->GetTransform().p.x), METERS_TO_PIXELS(bola->body->GetTransform().p.y));

				if (comboCounter == 3)
				{
					for (Circle* b : bolasExtras)
					{
						if (!b->body->body->IsEnabled())
						{
							bolaToEnable = b->body;
							break;
						}
					}
				}

				break;
			}
			case PASARELA:
			{

				comboCounter++;
				comboCounter = (comboCounter > 10) ? 10 : comboCounter;

				puntuacio += 100 * comboCounter;
				timerCombo = timeToCombo;
				ui->ShowPuntuation(100 * comboCounter, METERS_TO_PIXELS(bola->body->GetTransform().p.x), METERS_TO_PIXELS(bola->body->GetTransform().p.y));

				if (comboCounter == 3)
				{
					for (Circle* b : bolasExtras)
					{
						if (!b->body->body->IsEnabled())
						{
							bolaToEnable = b->body;
							break;
						}
					}
				}

				App->audio->PlayFx(pasarela);

				break;
			}
			case DETECTOR_MORT:
				if (bola->objectType == BOLA) respawn = true;
				if (bola->objectType == BOLA_EXTRA)
				{
					bolaToDisable = bola;
				}

				App->audio->PlayFx(fallSound);

				break;
			case VIDA:
				hideVida = true;

				break;
		
			default:
				break;
			}
		}
	}
}

void ModuleGame::SaveGame() {
	
	int highScore = 0;
	int previousScore;
	std::ifstream file("HighScore.txt");

	if (file.is_open()) {
		file >> highScore >> previousScore;
		file.close();
	}
	else {

		std::ofstream newFile("HighScore.txt");
		newFile << "0\n0" << std::endl;
		newFile.close();
	}
	
	previousScore = puntuacio;
	highScore = puntuacio > highScore ? puntuacio : highScore;

	TraceLog(LOG_INFO, "Guardando puntuajes.");
	std::ofstream fileOpen("HighScore.txt");

	if (fileOpen.is_open()) {
		fileOpen << highScore << "\n" << previousScore << std::endl;
		fileOpen.close();
	}
	else {
		TraceLog(LOG_INFO, "Error al guardar el archivo de puntuaje.");
	}
}

bool ModuleGame::GetReturnMain()
{
	return returnMain;
}