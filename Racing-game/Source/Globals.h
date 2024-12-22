#pragma once

#include "raylib.h"
#include "pugixml.hpp"
#include "math.h"
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);


void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH		  1920
#define SCREEN_HEIGHT		  1080
#define SCREEN_SIZE				1
#define WIN_FULLSCREEN		false
#define WIN_RESIZABLE		false
#define WIN_BORDERLESS		false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC				true
#define TITLE "Physics 2D Playground"

//Game Configuration
#define MAX_CAR_NUM 2

class BasicOperations
{
public: 
	static Vector2 NormalizeVector(Vector2 vector)
	{
		float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
		if (magnitude > 0.0f) {
			vector.x /= magnitude;
			vector.y /= magnitude;
		}
		return vector;
	}
	static float MagnitudeVector(Vector2 vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y);;
	}
	static float DotProduct(Vector2 vector1, Vector2 vector2)
	{
		return (vector1.x * vector2.x) + (vector1.y * vector2.y);
	}
	static float CrossProduct(Vector2 vector1, Vector2 vector2)
	{
		return (vector1.x * vector2.y) - (vector1.y * vector2.x);
	}
};

