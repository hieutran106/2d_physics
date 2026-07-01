#pragma once
#include "Shape.h"
#include "Vec2.h"

struct Body
{
	// Linear motion
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;

	// TODO: Angular motion

	// Forces
	Vec2 sumForces;

	// Mass
	float mass;
	float invMass;
	float radius;

	// A pointer to the geometry
	Shape * shape = nullptr;

	Body(const Shape & shape, float x, float y, float m);
	~Body();

	void Integrate(float dt);
	void AddForce(const Vec2 & force);
	void ClearForces();
};
