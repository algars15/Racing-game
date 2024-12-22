#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
#include "fstream"
#include "string"
#include "Car.h"


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "game";
	ui = new ModuleUI(app);
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
	int trackNum = 1;
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
					Vector2 point = { x + width / 2, y + height / 2 };
					App->physics->CreateRectangleSensor(point.x, point.y, width, height, b2_staticBody, ROUTE_SENSOR);
					routePoints.push_back(point);
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
	for (int i = 0; i < MAX_CAR_NUM && i < startPoints.size(); i++)
	{
		int numPlayers = 1;
		bool ia = i < numPlayers ? false : true;
		Car* car = new Car(App->physics, startPoints[i].x, startPoints[i].y, carsNode.attribute("width").as_int(), carsNode.attribute("height").as_int(), carsTexture, i, ia);
		cars.push_back(car);
		car->SetParameters(carsNode);
		car->SetRoute(routePoints);
		car->Start();
	}
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
	for each (Car * car in cars)
	{
		car->Update();
	}

	//DRAW
	DrawTexture(mapTexture, 0, 0, WHITE);

	for each (Car* car in cars)
	{
		car->Draw();
	}

	return UPDATE_CONTINUE;
}

void ModuleGame::SaveGame() {
	
	
}

bool ModuleGame::GetReturnMain()
{
	return false;
}