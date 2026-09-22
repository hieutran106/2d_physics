#include "Contact.h"

#include "Log.h"

void Contact::ResolvePenetration()
{
	if (a->IsStatic() && b->IsStatic())
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

	Vec2 ra = end - a->position;
	Vec2 rb = start - b->position;

	Vec2       va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
	Vec2       vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
	const Vec2 vrel = va - vb;

	// Calculate the relative velocity along the normal collision vector
	float vrelDotNormal = vrel.Dot(normal);

	// Calculate the collision impulse
	const Vec2 impulseDirection = normal;
	float      impulseMagnitude =
		-(1 + e) * vrelDotNormal
		/ (a->invMass + b->invMass + std::pow(ra.Cross(normal), 2) * a->invI + std::pow(rb.Cross(normal), 2) * b->invI);
	Vec2 Jn = impulseDirection * impulseMagnitude;

	// Calculate velocity after collision
	a->ApplyImpulse(Jn, ra);
	b->ApplyImpulse(-Jn, rb);

	Physics::LogDebug("Applied implulse J={} to body a={} b={}", Jn, *a, *b);
}
