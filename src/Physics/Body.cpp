#include "Body.h"

#include <SDL3/SDL.h>

#include <cmath> // Required for std::fabs

Body::Body(const Shape & shape, float x, float y, float m) : position(x, y), mass(m)
{
	this->shape = shape.Clone();
	if(mass != 0)
	{
		invMass = 1 / mass;
	}
	else
	{
		// simulate object that has infinite mass
		invMass = 0.0;
	}

	this->I = shape.GetMomentOfInertia() * mass;
	if(I != 0)
	{
		this->invI = 1 / this->I;
	}
	else
	{
		this->invI = 0.0;
	}
}
Body::~Body()
{
	delete shape;
}
bool Body::IsStatic() const
{
	const float epsilon = 0.005f;
	return std::fabs(invMass - 0.0) < epsilon;
}
void Body::Update(float deltaTime)
{
	IntegrateLinear(deltaTime);
	IntegrateAngular(deltaTime);
	auto shapeType = shape->GetType();
	bool isPolygon = shapeType == POLYGON || shapeType == BOX;
	if(isPolygon)
	{
		PolygonShape * polygonShape = static_cast<PolygonShape *>(shape);
		polygonShape->UpdateVertices(rotation, position);
	}
}

void Body::IntegrateLinear(float dt)
{
	if(IsStatic())
	{
		return;
	}
	// The acceleration is calculated from the net-force divided by the mass
	acceleration = sumForces * invMass;
	// Integrate the acceleration to find the new velocity for the next step
	velocity += acceleration * dt;
	// Prevent moving slightly due to friction
	if(velocity.MagnitudeSquared() < 5)
	{
		velocity.x = 0.0;
		velocity.y = 0.0;
	}
	// Integrate the velocity to find the new position for the next step
	position += velocity * dt;
	ClearForces();
}
void Body::IntegrateAngular(float dt)
{
	if(IsStatic())
	{
		return;
	}

	angularAcceleration = sumTorque * invI;
	angularVelocity += angularAcceleration * dt;
	rotation += angularVelocity * dt;
	// SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Body angularVelocity=%.4f, rotation=%.4f", angularVelocity, rotation);
	ClearTorque();
}
void Body::AddForce(const Vec2 & force)
{
	sumForces += force;
}
void Body::ClearForces()
{
	sumForces = Vec2(0, 0);
}
////////////////////////////////////////
// Torque
void Body::AddTorque(float torque)
{
	sumTorque += torque;
}

void Body::ClearTorque()
{
	sumTorque = 0;
}

void Body::ApplyImpulse(const Vec2 & J)
{
	if(IsStatic())
	{
		return;
	}
	velocity += J * invMass;
}