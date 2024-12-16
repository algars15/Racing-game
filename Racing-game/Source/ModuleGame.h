#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;

class Box; 
class Circle;
class Shape;
class CircleSensor;

class b2RevoluteJoint;
class b2PrismaticJoint;
class ModuleUI;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, Vector2 normal);
	void UpdateFlipper(b2RevoluteJoint* joint, bool isPressed, bool right);
	bool GetReturnMain();
	void SaveGame();
	void RestartGame();

public:

	std::vector<PhysicEntity*> entities;
	ModuleUI* ui;
	
	PhysBody* sensor;
	bool sensed;

	Texture2D pimball_map;
	Texture2D pimball_map2;
	Texture2D circle;
	Texture2D circle_extra;
	Texture2D box;
	Texture2D palancaTexture;
	Texture2D palanca_invertida;
	Texture2D spring;
	Texture2D springTop;
	Texture2D springBottom;
	Texture2D vidasTexture;

	Circle* bola;
	PhysBody* bolaToDisable;
	PhysBody* bolaToEnable;
	std::vector<Circle*> bolasExtras;
	Box* palancaDerecha;
	Box* palancaIzquierda;
	PhysBody* unionPalancaDerecha;
	PhysBody* unionPalancaIzquierda;
	CircleSensor* vida;

	Box* mollaTop;
	Box* mollaBottom;
	Box* molla;
	Shape* base;


	b2RevoluteJoint* jointPalancaIzquierda;
	b2RevoluteJoint* jointPalancaDerecha;

	b2PrismaticJoint* jointMolla;

	//uint32 bonus_fx;
	uint32 springSound;
	uint32 flipperSound;
	uint32 bouncerSound;
	uint32 hitSound;
	uint32 fallSound;
	uint32 newBallSound;
	uint32 song;
	uint32 pasarela;
	uint32 boingSound;
	uint32 healSound;

	bool mort;
	bool respawn;
	bool returnMain;
	bool createNewBall;
	bool hideVida;
	int vides;
	int puntuacio;

	vec2<int> ray;
	bool ray_on;
	int velocitatPalanca;
	int forcaImpuls;
	Vector2 startPos;
	bool mollaLliberada;
	float timerCombo;
	float timeToCombo;
	int comboCounter;
	float timeToVida;
	float timerVida;
	Vector2 vidaSpawns[3];

};
