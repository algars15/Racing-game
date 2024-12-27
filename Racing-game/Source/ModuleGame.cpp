#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleScene.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
#include "fstream"
#include "string"
#include "Car.h"
#include "Timer.h"


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "game";
}

ModuleGame::~ModuleGame()
{}


void ModuleGame::LoadGame()
{
	//EnableModule
	enabled = true;

	//Loading Assets
	TraceLog(LOG_INFO, "Loading Game Assets");

	//Loading Track
	char nodeName[16];
	sprintf_s(nodeName, "c%d", trackNum);
	pugi::xml_node trackNode = parameters.child("tracks").child(nodeName);
		//Image
	mapTexture = LoadTexture(trackNode.attribute("path").as_string());
	
		//GroupObjects
	pugi::xml_document mapFileXML;
	pugi::xml_parse_result result = mapFileXML.load_file(trackNode.attribute("tmx").as_string());

	if (result == NULL)
	{
		TraceLog(LOG_INFO, "Could not load map xml file %s. pugi error: %s", trackNode.attribute("path").as_string(), result.description());
	}
	else {

		int pointIndex = 0;
		for (pugi::xml_node objectGroupNode = mapFileXML.child("map").child("objectgroup"); objectGroupNode != NULL; objectGroupNode = objectGroupNode.next_sibling("objectgroup")) {

			std::string objectGroupName = objectGroupNode.attribute("name").as_string();

			for (pugi::xml_node objectNode = objectGroupNode.child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {
				int x = objectNode.attribute("x").as_int();
				int y = objectNode.attribute("y").as_int();
				int width = objectNode.attribute("width").as_int();
				int height = objectNode.attribute("height").as_int();

				if (objectGroupName == "Collisions") App->physics->CreateRectangle(x + width / 2, y + height / 2, width, height, b2_staticBody, MAP_COLLIDER);
				else if (objectGroupName == "Route")
				{
					RoutePoint* routePoint = new RoutePoint();
					routePoint->position = { (float)x + width / 2, (float)y + height / 2 };
					routePoint->pointIndex = pointIndex;
					routePoint->body = App->physics->CreateRectangleSensor(routePoint->position.x, routePoint->position.y, width, height, b2_staticBody, ROUTE_SENSOR);
					routePoints.push_back(routePoint);
					pointIndex++;
				}
				else if (objectGroupName == "StartPoints")
				{
					Vector2 pos = { x,y };
					startPoints.push_back(pos);
					TraceLog(LOG_INFO,"%d,%d", pos.x, pos.y);
				}
			}	
		}
	}

	if (startPoints.size() != MAX_CAR_NUM)
	{
		TraceLog(LOG_INFO, "number of start points different than max cars");
	}

	//Loading Cars
	pugi::xml_node carsNode = parameters.child("cars");
	carsTexture = LoadTexture(carsNode.attribute("path").as_string());
	for (int i = 0; i < MAX_CAR_NUM && i < startPoints.size(); i++) {
		bool ia = (i >= numPlayers); // IA solo si no es un jugador
		Car* car = nullptr;

		if (i == 0) {
			car = new Car(App->physics, startPoints[i].x, startPoints[i].y, carsNode.attribute("width").as_int(), carsNode.attribute("height").as_int(), carsTexture, j1CarNum, ia);
			car->SetKeys(KEY_W, KEY_S, KEY_D, KEY_A);
		}
		else if (i == 1 && numPlayers == 2) {
			car = new Car(App->physics, startPoints[i].x, startPoints[i].y, carsNode.attribute("width").as_int(), carsNode.attribute("height").as_int(), carsTexture, j2CarNum, ia);
			car->SetKeys(KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT);
		}
		else {
			// Selecci�n de un n�mero de carro �nico
			bool foundDifferent = false;
			int carNum = 0;

			do {
				carNum = GetRandomValue(0, startPoints.size() - 1); // N�mero aleatorio v�lido
				foundDifferent = true; // Asumimos que es diferente

				for (Car* existingCar : cars) {
					if (existingCar->GetCarNum() == carNum) {
						foundDifferent = false; // Si coincide, no es diferente
						break;
					}
				}
			} while (!foundDifferent);

			car = new Car(App->physics, startPoints[i].x, startPoints[i].y, carsNode.attribute("width").as_int(), carsNode.attribute("height").as_int(), carsTexture, carNum, ia);
		}

		// Configurar y empezar el carro
		car->SetParameters(carsNode);
		car->SetRoute(routePoints);
		car->Start();
		cars.push_back(car);
	}

	//Loading Properties
	sortTime = parameters.child("properties").attribute("sortTime").as_float();
	sortTimer.Start();
	
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
update_status ModuleGame::Update(float dt)
{
	if (App->scene->GetState() == GAME) {
		
		for each (Car * car in cars)
		{
			car->Update(dt);
		}

		if (sortTimer.ReadSec() > sortTime)
		{
			bool endedSwap;
			do {
				endedSwap = true;
				for (int i = 0; i < cars.size() - 1; i++) {
					bool swap = false;
					Ranking carRank1 = cars[i]->GetRank();
					Ranking carRank2 = cars[i + 1]->GetRank();

					// Ordenar por vueltas
					if (carRank1.lap < carRank2.lap) swap = true;
					// Si las vueltas son iguales, ordenar por puntos de control
					else if (carRank1.lap == carRank2.lap && carRank1.checkPoint < carRank2.checkPoint) swap = true;
					// Si las vueltas y los checkpoints son iguales, ordenar por distancia (cambiado a mayor prioridad)
					else if (carRank1.lap == carRank2.lap && carRank1.checkPoint == carRank2.checkPoint &&
						carRank1.distanceToNextCheckpoint > carRank2.distanceToNextCheckpoint) swap = true;

					if (swap) {
						std::swap(cars[i], cars[i + 1]);
						endedSwap = false; // Si ocurre un intercambio, necesitamos otra pasada
					}
				}
			} while (!endedSwap);
			sortTimer.Start();
		}

		//DRAW
		DrawTexture(mapTexture, 0, 0, WHITE);

		for each (Car * car in cars)
		{
			car->Draw();
		}

		return UPDATE_CONTINUE;
	}
}

void ModuleGame::SaveGame() {
	
	
}

void ModuleGame::SetUI(ModuleUI* _ui)
{
	ui = _ui;
}

bool ModuleGame::GetReturnMain()
{
	return false;
}

void ModuleGame::SetCars(int j1Car, int j2Car)
{
	j1CarNum = j1Car;
	j2CarNum = j2Car;
	numPlayers = j2CarNum < 0 ? 1 : 2;
}

void ModuleGame::SetTrack(int track)
{
	trackNum = track;
}