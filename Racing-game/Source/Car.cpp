#include "Car.h"

void Car::Start()
{
	body->body->SetGravityScale(0);
	forwardKey = KEY_W;
	backKey = KEY_S;
	rightKey = KEY_D;
	leftKey = KEY_A;
	body->body->SetAngularDamping(20);
	body->body->SetLinearDamping(3);
}

void Car::Update()
{
	Vector2 input = { 0,0 };
	float turnSpeed = 15.0f;
	float acceleration = 1.0f;
	float deceleration = 0.3f;
	float maxSpeed = 10.0f;

	if (IsKeyDown(forwardKey) && !IsKeyDown(backKey)) input.y = -1;
	else if(IsKeyDown(backKey) && !IsKeyDown(forwardKey)) input.y = 1;

	if (IsKeyDown(rightKey) && !IsKeyDown(leftKey)) input.x = 1;
	else if (IsKeyDown(leftKey) && !IsKeyDown(rightKey)) input.x = -1;

	if (input.y > 0)
	{
		input.x = -input.x;
		acceleration = deceleration;
	}

	Vector2 velocity = body->GetLinearVelocity();

	// Calcular la dirección deseada
	Vector2 forwardVector = body->GetWorldVector({ 0.0f, 1.0f }); // Dirección hacia adelante
	Vector2 rightVector = body->GetWorldVector({ 1.0f, 0.0f }); // Dirección lateral (derecha)

	Vector2 accelerationVector = {
		forwardVector.x * acceleration * input.y,
		forwardVector.y * acceleration * input.y
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
	if (speed > maxSpeed) {
		targetVelocity = BasicOperations().NormalizeVector(targetVelocity);
		targetVelocity = {
			targetVelocity.x * maxSpeed,
			targetVelocity.y * maxSpeed,
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

void Car::Draw()
{
	int x, y;
	body->GetPhysicPosition(x, y);
	if (draw)
	{
		DrawTexturePro(texture, Rectangle{ (float) width * carNumber, 0, (float)width, (float)height },
			Rectangle{ (float)x, (float)y, (float)width, (float)height },
			Vector2{ (float)width / 2.0f, (float)height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}
}

int Car::RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
{
	return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
}