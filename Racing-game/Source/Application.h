#pragma once

#include "Globals.h"
#include "Timer.h"
#include <vector>
#include "pugixml.hpp"

class Module;
class ModuleWindow;
class ModuleRender;
class ModuleAudio;
class ModulePhysics;
class ModuleScene;
class ModuleGame;
class ModuleMenu;
class ModuleUI;

class Application
{
public:

	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleAudio* audio;
	ModulePhysics* physics;
	ModuleGame* game;
	ModuleMenu* menu;
	ModuleScene* scene;
	ModuleUI* ui;

	pugi::xml_document configFile;

private:

	std::vector<Module*> list_modules;
    uint64 frame_count = 0;

	Timer ptimer;
	Timer startup_time;
	Timer frame_time;
	Timer last_sec_frame_time;

	uint32 last_sec_frame_count = 0;
	uint32 prev_last_sec_frame_count = 0;

	bool debug;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool GetDebug();
	void ChangeDebug();
	bool CleanUp();

private:

	void AddModule(Module* module);
	bool LoadConfig();
	
};