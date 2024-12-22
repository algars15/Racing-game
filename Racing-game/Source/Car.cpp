#include "Car.h"

void Car::Start()
{
	forwardKey = KEY_W;
	backKey = KEY_S;
	rightKey = KEY_D;
	leftKey = KEY_A;
	body->body->SetAngularDamping(parameters.attribute("angularDamping").as_float());
	body->body->SetLinearDamping(parameters.attribute("linearDamping").as_float());
	body->body->SetGravityScale(parameters.attribute("gravity").as_float());
	turnSpeed = parameters.attribute("turnSpeed").as_float();
	acceleration = parameters.attribute("acceleration").as_float();
	deceleration = parameters.attribute("deceleration").as_float();
	maxSpeed = parameters.attribute("maxSpeed").as_float();
	reverseMaxSpeed = parameters.attribute("reverseMaxSpeed").as_float();
	currentWaypointIndex = 0;
	waypointRadius = 20;
}

void Car::Update()
{
	input = { 0,0 };

	if (!ia) Input();
	else (Ia());

	Vector2 velocity = body->GetLinearVelocity();

	// Calcular la dirección deseada
	Vector2 forwardVector = body->GetWorldVector({ 0.0f, 1.0f }); // Dirección hacia adelante
	Vector2 rightVector = body->GetWorldVector({ 1.0f, 0.0f }); // Dirección lateral (derecha)

	float currentAcceleration = input.y > 0 ? deceleration : acceleration;
	float currentMaxSpeed = input.y > 0 ? reverseMaxSpeed : maxSpeed;

	Vector2 accelerationVector = {
		forwardVector.x * currentAcceleration * input.y,
		forwardVector.y * currentAcceleration * input.y
	};

	// Calculamos la velocidad deseada
	Vector2 targetVelocity = {
		velocity.x + accelerationVector.x,
		velocity.y + accelerationVector.y
	}; 

	// Eliminar la componente lateral de la velocidad (cancelar el derrape)
	Vector2 lateralVelocity = {
		rightVector.x * BasicOperations().DotProduct(velocity, rightVector),
		rightVector.y * BasicOperations().DotProduct(velocity, rightVector)
	};

	targetVelocity = {
		targetVelocity.x - lateralVelocity.x,
		targetVelocity.y - lateralVelocity.y
	};

	float speed = BasicOperations().MagnitudeVector(targetVelocity);

	// Limitar la velocidad máxima
	if (speed > currentMaxSpeed) {
		targetVelocity = BasicOperations().NormalizeVector(targetVelocity);
		targetVelocity = {
			targetVelocity.x * currentMaxSpeed,
			targetVelocity.y * currentMaxSpeed,
		};
	}

	//Giramos dependiendo de la velocidad
	float speedFactor = speed / (maxSpeed / 2); // Rango [0, 1]
	speedFactor = speedFactor > 1 ? 1 : speedFactor;
	speedFactor = speedFactor < 0 ? 0 : speedFactor;

	// Aplicar giro (torque)
	if (abs(speed) > 0.1f) { // Umbral mínimo de velocidad
		float dynamicTorque = turnSpeed * input.x * speedFactor;
		body->ApplyTorque(dynamicTorque);
	}

	// Establecer la nueva velocidad
	body->SetLinearVelocity(targetVelocity);
}

void Car::Input()
{
	if (IsKeyDown(forwardKey) && !IsKeyDown(backKey)) input.y = -1;
	else if (IsKeyDown(backKey) && !IsKeyDown(forwardKey)) input.y = 1;

	if (IsKeyDown(rightKey) && !IsKeyDown(leftKey)) input.x = input.y > 0 ? -1 : 1;
	else if (IsKeyDown(leftKey) && !IsKeyDown(rightKey)) input.x = input.y > 0 ? 1 : -1;
}

void Car::Ia()
{
	// Obtener posición y orientación actual del coche
	Vector2 carPosition = body->GetPosition();
	Vector2 carForward = body->GetWorldVector({ 0.0f, 1.0f }); // Dirección hacia adelante del coche

	// Punto objetivo de la ruta
	Vector2 targetPoint = routePoints[currentWaypointIndex];

	// Vector hacia el punto objetivo
	Vector2 toTarget = {
		targetPoint.x - carPosition.x,
		targetPoint.y - carPosition.y
	};

	// Normalizar el vector hacia el objetivo
	Vector2 toTargetNormalized = BasicOperations().NormalizeVector(toTarget);
	

	// Calcular inputs
	float crossProduct = BasicOperations().CrossProduct(carForward, toTargetNormalized);

	input.y = BasicOperations().DotProduct(toTargetNormalized, carForward) > 0 ? 1 : -1; // Avanzar o retroceder
	if (abs(crossProduct)>0.1) input.x = crossProduct > 0 ? -1 : 1; // Girar izquierda o derecha
}

void Car::Draw()
{
	int x, y;
	body->GetPhysicPosition(x, y);
	if (draw)
	{
		DrawCircle(routePoints[currentWaypointIndex].x, routePoints[currentWaypointIndex].y, waypointRadius, RED);
		DrawTexturePro(texture, Rectangle{ (float) width * carNumber, 0, (float)width, (float)height },
			Rectangle{ (float)x, (float)y, (float)width, (float)height },
			Vector2{ (float)width / 2.0f, (float)height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}
}

void Car::SetParameters(pugi::xml_node p)
{
	parameters = p;
}

void Car::SetRoute(std::vector<Vector2> r)
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
			currentWaypointIndex = (currentWaypointIndex + 1) % routePoints.size();
			break;
	}
}
