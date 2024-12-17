#pragma once

#include "Globals.h"

class Application;
class PhysBody;

class Module
{
protected :
	bool enabled;
	pugi::xml_node parameters;
public:
	Application* App;
	std::string name;

	Module(Application* parent, bool start_enabled = true) : App(parent), enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return enabled;
	}

	void SetParameters(pugi::xml_node node)
	{
		parameters = node.child(name.c_str());
		if (parameters.empty()) LOG("Failed to load parameters from module %c", name.c_str());
	}

	void Enable()
	{
		if(enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if(enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB, Vector2 normal = { 0.f,0.f })
	{

	}
};