#include "Car.h"
#include "math.h"

void Car::Start()
{
	//COMMON
	body->body->SetAngularDamping(parameters.attribute("angularDamping").as_float());
	body->body->SetLinearDamping(parameters.attribute("linearDamping").as_float());
	body->body->SetGravityScale(parameters.attribute("gravity").as_float());
	turnSpeed = parameters.attribute("turnSpeed").as_float();
	turnSpeedDrs = parameters.attribute("turnSpeedDrs").as_float();
	acceleration = parameters.attribute("acceleration").as_float();
	deceleration = parameters.attribute("deceleration").as_float();
	maxSpeed = parameters.attribute("maxSpeed").as_float();
	drsMaxSpeed = parameters.attribute("drsMaxSpeed").as_float();
	reverseMaxSpeed = parameters.attribute("reverseMaxSpeed").as_float();

	carSound = LoadSound(parameters.attribute("soundPath").as_string());
	float soundVolume = ia ? 0.2f : 0.4f;
	SetSoundVolume(carSound, soundVolume);
	defaultPitch = BasicOperations::GetRandomFloat(0.8f, 1.2f);

	nextWaypointIndex = 0;
	currentWaypointIndex = routePoints.size() - 1;
	previousWaypointIndex = routePoints.size() - 2;
	ranking.checkPoint = currentWaypointIndex;
	ranking.distanceToNextCheckpoint = 0;
	ranking.lap = -1;

	//INDIVIDUAL
	char nodeName[16];
	sprintf_s(nodeName, "car%d", carNumber);
	name = parameters.child(nodeName).attribute("name").as_string();
	
	drs = false;
}

void Car::SetRotation(float degrees)
{
	body->SetRotation(degrees * PI / 180);
}

void Car::SetKeys(KeyboardKey keyUp, KeyboardKey keyDown, KeyboardKey keyRight, KeyboardKey keyLeft, KeyboardKey keyBoost)
{
	forwardKey = keyUp;
	backKey = keyDown;
	rightKey = keyRight;
	leftKey = keyLeft;
	boostKey = keyBoost;
}

void Car::Update(float dt)
{
	input = { 0,0 };

	if (game->GetStarted())
	{
		if (!ia) Input();
		else (Ia());
	}

	

	Vector2 velocity = body->GetLinearVelocity();

	// Calcular la direcci�n deseada
	Vector2 forwardVector = body->GetWorldVector({ 0.0f, 1.0f }); // Forward direction
	Vector2 rightVector = body->GetWorldVector({ 1.0f, 0.0f }); // Right direction

	float currentAcceleration = input.y > 0 ? deceleration : acceleration;

	float currentMaxSpeed = 0;
	if (input.y > 0) {
		currentMaxSpeed = reverseMaxSpeed;
	}
	else if (input.y <= 0){
		if (drs) {
			currentMaxSpeed = drsMaxSpeed;
		}
		else {
			currentMaxSpeed = maxSpeed;
		}
	}

	Vector2 accelerationVector = {
		forwardVector.x * currentAcceleration * input.y,
		forwardVector.y * currentAcceleration * input.y
	};

	// Calculate velocity
	Vector2 targetVelocity = {
		velocity.x + accelerationVector.x,
		velocity.y + accelerationVector.y
	}; 

	// Eliminate lateral component of the velocity (eliminate car drift)
	Vector2 lateralVelocity = {
		rightVector.x * BasicOperations().DotProduct(velocity, rightVector),
		rightVector.y * BasicOperations().DotProduct(velocity, rightVector)
	};

	targetVelocity = {
		targetVelocity.x - lateralVelocity.x,
		targetVelocity.y - lateralVelocity.y
	};

	float speed = BasicOperations().MagnitudeVector(targetVelocity);

	// Limit maximum velocity
	if (speed > currentMaxSpeed) {
		targetVelocity = BasicOperations().NormalizeVector(targetVelocity);
		targetVelocity = {
			targetVelocity.x * currentMaxSpeed,
			targetVelocity.y * currentMaxSpeed,
		};
	}

	// Rotate depending on the velocity
	float speedFactor = speed / (maxSpeed / 2); // Range [0, 1]
	speedFactor = speedFactor > 1 ? 1 : speedFactor;
	speedFactor = speedFactor < 0 ? 0 : speedFactor;

	// Apply rotation (torque)
	if (drs) {
		if (abs(speed) > 0.1f) { // Minimum velocity values
			float dynamicTorque = turnSpeedDrs * input.x * speedFactor;
			body->ApplyTorque(dynamicTorque);
		}
	}
	else {
		if (abs(speed) > 0.1f) { // Minimum velocity values
			float dynamicTorque = turnSpeed * input.x * speedFactor;
			body->ApplyTorque(dynamicTorque);
		}
	}
	
	float pitch = (abs(speed) / (maxSpeed))/1.5f;
	if (!IsSoundPlaying(carSound)) PlaySound(carSound);
	SetSoundPitch(carSound, defaultPitch + pitch);

	// Establish new velocity
	body->SetLinearVelocity(targetVelocity);

	

	// Calculate distance to the next checkpoint(ranking)
	Vector2 VectorToRoutePoint = {
			body->GetPosition().x - routePoints[nextWaypointIndex]->position.x,
			body->GetPosition().y - routePoints[nextWaypointIndex]->position.y,
	};
	ranking.distanceToNextCheckpoint = BasicOperations().MagnitudeVector(VectorToRoutePoint);
}

void Car::Input()
{
	if (IsKeyDown(boostKey) && IsKeyDown(forwardKey))	drs = true;
	else drs = false;
	
	if (IsKeyDown(forwardKey) && !IsKeyDown(backKey)) input.y = -1;
	else if (IsKeyDown(backKey) && !IsKeyDown(forwardKey)) input.y = 1;

	if (IsKeyDown(rightKey) && !IsKeyDown(leftKey)) input.x = input.y > 0 ? -1 : 1;
	else if (IsKeyDown(leftKey) && !IsKeyDown(rightKey)) input.x = input.y > 0 ? 1 : -1;
}

void Car::Ia()
{
	
	// Get actual position and orientation of the car
	Vector2 carPosition = body->GetPosition();
	Vector2 carForward = body->GetWorldVector({ 0.0f, 1.0f }); // Forward direction

	// Next target point in the route
	Vector2 targetPoint = nextWaypointPos;

	// Vector towards objective point
	Vector2 toTarget = {
		targetPoint.x - carPosition.x,
		targetPoint.y - carPosition.y
	};

	// Normalize vector
	Vector2 toTargetNormalized = BasicOperations().NormalizeVector(toTarget);

	// Calculate inputs
	float crossProduct = BasicOperations().CrossProduct(carForward, toTargetNormalized);

	input.y = -1; // Go forward or backward
	if (abs(crossProduct)>0.1) input.x = crossProduct > 0 ? -1 : 1; // Turn left or right

	//If stucked go reverse
	if (body->GetVelocity() < 4 || stuckedTimer.ReadSec()>1)
	{
		if (stuckedTimer.ReadSec() > 1)
		{
			input.y = 1;
			if (stuckedTimer.ReadSec() > 1.5f)
			{
				stuckedTimer.Start();
			}
		}
	}
	else stuckedTimer.Start();
}

void Car::Draw()
{
	int x, y;
	body->GetPhysicPosition(x, y);
	if (draw)
	{
		if (ia && game->GetDebug()) DrawCircle(nextWaypointPos.x, nextWaypointPos.y, 5, RED);
		DrawTexturePro(texture, Rectangle{ (float) width * carNumber, 0, (float)width, (float)height },
			Rectangle{ (float)x, (float)y, (float)width, (float)height },
			Vector2{ (float)width / 2.0f, (float)height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}
}

void Car::SetRoute(std::vector<RoutePoint*> r)
{
	routePoints = r;
}

int Car::RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
{
	return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
}

void Car::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	PhysBody* carPhysBody = body == bodyA ? bodyA : bodyB;
	PhysBody* otherPhysBody = body == bodyA ? bodyB : bodyA;

	switch (otherPhysBody->objectType)
	{
		case ObjectType::ROUTE_SENSOR:
			
			
			if (otherPhysBody == routePoints[nextWaypointIndex]->body)
			{
				nextWaypointIndex = (nextWaypointIndex + 1) % routePoints.size();
				currentWaypointIndex = (currentWaypointIndex + 1) % routePoints.size();
				previousWaypointIndex = (previousWaypointIndex + 1) % routePoints.size();
				if (currentWaypointIndex == 0)
				{
					ranking.lap++;
					if (!ia && ranking.lap >= game->GetLaps()) ia = true;
				}
			}
			else if (otherPhysBody == routePoints[previousWaypointIndex]->body)
			{
				nextWaypointIndex = (nextWaypointIndex - 1 + routePoints.size()) % routePoints.size();
				currentWaypointIndex = (currentWaypointIndex - 1 + routePoints.size()) % routePoints.size();
				previousWaypointIndex = (previousWaypointIndex - 1 + routePoints.size()) % routePoints.size();
				if (nextWaypointIndex == 0) ranking.lap--;
			}
			
			ranking.checkPoint = currentWaypointIndex;

			if (ia)
			{
				nextWaypointPos = routePoints[nextWaypointIndex]->position;
				float nextWayPointWidth =  routePoints[nextWaypointIndex]->body->width;
				float nextWayPointHeight =  routePoints[nextWaypointIndex]->body->height;
				if (nextWayPointWidth > nextWayPointHeight) nextWaypointPos.x += GetRandomValue(-nextWayPointWidth/2, nextWayPointWidth / 2);
				if (nextWayPointWidth < nextWayPointHeight) nextWaypointPos.y += GetRandomValue(-nextWayPointHeight / 2, nextWayPointHeight/2);
			}

			break;
	}
}

Ranking Car::GetRank()
{
	return ranking;
}

int Car::GetCarNum()
{
	return carNumber;
}

void Car::SetGame(ModuleGame* Game)
{
	game = Game;
}

void Car::SetName(std::string Name)
{
	name = Name;
}

std::string Car::GetCarName()
{
	return name;
}


void Car::CleanUp()
{
	UnloadSound(carSound);
}