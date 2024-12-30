#pragma once

#include "Globals.h"
#include "Module.h"
#include "p2Point.h"
#include "raylib.h"
#include "Timer.h"
#include <vector>

class PhysBody;
class PhysicEntity;

class Car;

class ModuleUI;

struct RoutePoint
{
	int pointIndex;
	Vector2 position;
	PhysBody* body;
};

struct Ranking
{
	int lap;
	int checkPoint;
	float distanceToNextCheckpoint;
};

enum TrackDirection
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	update_status Update(float dt);
	bool CleanUp();
	bool GetReturnMain();
	bool GetStarted();
	bool GetEnded();
	bool GetDebug();
	void SaveGame();
	void RestartGame();
	void LoadGame();
	void SetUI(ModuleUI* ui);
	void SetTrack(int track);
	void SetCars(int j1Car, int j2Car);
	float GetTime();
	int GetLaps();
	int GetCurrentLap();
	std::vector<std::string> GetRankingNames();
	std::vector<int> GetRankingNums();

private:

	float startTime;
	Timer startTimer;
	float raceTime;
	bool started;
	bool ended;
	bool returnToMainMenu;
	int redSoundCounter;

	int currentLap;
	int laps;

	//TEXTURES
	Texture2D mapTexture;
	Texture2D carsTexture;

	//TRACK
	int trackNum;
	std::vector<Vector2> startPoints;
	std::vector<RoutePoint*> routePoints;
	TrackDirection trackDirection;

	//CARS
	int numPlayers;
	int j1CarNum;
	int j2CarNum;
	std::vector<Car*> cars;
	std::vector<int> ranking;
	Timer sortTimer;
	float sortTime;

	//PHYSICS
	std::vector<PhysicEntity*> entities;

	//UI
	ModuleUI* ui;

	//AUDIO
	Sound greenSound;
	Sound redSound;
	Sound endSong;
};
