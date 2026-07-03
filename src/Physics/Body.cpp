#include "Body.h"

#include <SDL3/SDL.h>

#include <stdexcept>

Body::Body(const Shape & shape, float x, float y, float m) : position(x, y), mass(m)
{
	this->shape = shape.Clone();
	if(mass == 0)
	{
		throw std::invalid_argument("Mass cannot be zero.");
	}
	invMass = 1 / mass;
}
Body::~Body()
{
	delete shape;
}

void Body::Integrate(float dt)
{
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
void Body::AddForce(const Vec2 & force)
{
	sumForces += force;
}
void Body::ClearForces()
{
	sumForces = Vec2(0, 0);
}