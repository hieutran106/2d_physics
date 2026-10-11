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
	float f = std::min(a->friction, b->friction);

	Vec2 ra = end - a->position;
	Vec2 rb = start - b->position;

	Vec2       va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
	Vec2       vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
	const Vec2 vrel = va - vb;

	// Calculate the relative velocity along the normal collision vector
	float vrelDotNormal = vrel.Dot(normal);

	// Calculate the collision impulse along the normal
	const Vec2 impulseDirectionN = normal;
	float      impulseMagnitudeN =
		-(1 + e) * vrelDotNormal
		/ (a->invMass + b->invMass + std::pow(ra.Cross(normal), 2) * a->invI + std::pow(rb.Cross(normal), 2) * b->invI);
	Vec2 Jn = impulseDirectionN * impulseMagnitudeN;

	// Calculate the collision impulse along the tangent
	Vec2       tangent = normal.Normal();
	float      vrelDotTangent = vrel.Dot(tangent);
	const Vec2 impulseDirectionT = tangent;
	float      impulseMagnitudeT = -(1 + f) * vrelDotTangent
							/ (a->invMass + b->invMass + std::pow(ra.Cross(tangent), 2) * a->invI
							   + std::pow(rb.Cross(tangent), 2) * b->invI);
	Vec2 Jt = impulseDirectionT * impulseMagnitudeT;

	Vec2 J = Jn + Jt;
	// Calculate velocity after collision
	a->ApplyImpulse(J, ra);
	b->ApplyImpulse(-J, rb);

	Physics::LogDebug("Applied implulse J={} to body a={} b={}", Jn, *a, *b);
}
