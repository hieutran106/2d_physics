#include "Contact.h"

void Contact::ResolvePenetration()
{
	if(a->IsStatic() && b->IsStatic())
	{
		return;
	}
	float da = depth / (a->invMass + b->invMass) * a->invMass;
	float db = depth / (a->invMass + b->invMass) * b->invMass;

	a->position -= normal * da;
	b->position += normal * db;
}

void Contact::ResolveCollision()
{
	ResolvePenetration();
	float e = std::min(a->restitution, b->restitution);

	const Vec2 vrel = a->velocity - b->velocity;
	float vrelDotNormal = vrel.Dot(normal);

	// Calculate the collision impulse
	const Vec2 impulseDirection = normal;
	float impulseMagnitude = -(1 + e) * vrelDotNormal / (a->invMass + b->invMass);
	Vec2 J = impulseDirection * impulseMagnitude;

	a->ApplyImpulse(J);
	b->ApplyImpulse(-J);
}