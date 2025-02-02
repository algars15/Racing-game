
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleGame.h"
#include "ModuleScene.h"
#include "ModuleMenu.h"
#include "ModuleUI.h"

#include "Application.h"

Application::Application()
{
	bool ret = true;
	LoadConfig();

	window = new ModuleWindow(this);
	renderer = new ModuleRender(this);
	audio = new ModuleAudio(this);
	physics = new ModulePhysics(this);
	scene = new ModuleScene(this);
	menu = new ModuleMenu(this);
	game = new ModuleGame(this, false);
	ui = new ModuleUI(this);

	scene->SetGame(game);
	scene->SetMenu(menu);
	game->SetUI(ui);
	ui->SetGame(game);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(physics);
	AddModule(audio);
	
	// Scenes

	AddModule(menu);
	AddModule(game);
	AddModule(ui);
	AddModule(scene);

	
	// Rendering happens at the end
	AddModule(renderer);
	debug = false;
	

	for (auto it = list_modules.rbegin(); it != list_modules.rend(); ++it)
	{
		Module* module = *it;
		module->SetParameters(configFile.child("config"));
	}
}

Application::~Application()
{
	for (auto it = list_modules.rbegin(); it != list_modules.rend(); ++it)
	{
		Module* item = *it;
		delete item;
	}
	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (auto it = list_modules.begin(); it != list_modules.end() && ret; ++it)
	{
		Module* module = *it;
		if (module->IsEnabled()) ret = module->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (auto it = list_modules.begin(); it != list_modules.end() && ret; ++it)
	{
		Module* module = *it;
		ret = module->Start();
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (IsKeyPressed(KEY_F1)) ChangeDebug();

	for (auto it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		Module* module = *it;
		if (module->IsEnabled())
		{
			ret = module->PreUpdate();
		}
	}

	for (auto it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		
		Module* module = *it;
		
   		if (module->IsEnabled())
		{
			ret = module->Update(GetFrameTime());
		}
	}

	for (auto it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		Module* module = *it;
		if (module->IsEnabled())
		{
			ret = module->PostUpdate();
		}
	}

	if (WindowShouldClose()) ret = UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (auto it = list_modules.rbegin(); it != list_modules.rend() && ret; ++it)
	{
		Module* item = *it;
		ret = item->CleanUp();
	}
	
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.emplace_back(mod);
}

bool Application::LoadConfig()
{
	bool ret = true;

	pugi::xml_parse_result result = configFile.load_file("config.xml");
	if (result)
	{
		LOG("config.xml parsed without errors");
	}
	else
	{
		LOG("Error loading config.xml: %s", result.description());
	}

	return ret;
}

bool Application::GetDebug()
{
	return debug;
}

void Application::ChangeDebug()
{
	debug = !debug;
}