#include "CollisionDetection.h"

bool CollisionDetection::IsColliding(Body * a, Body * b, Contact & contact)
{
	bool aIsCircle = a->shape->GetType() == CIRCLE;
	bool bIsCircle = b->shape->GetType() == CIRCLE;
	if(aIsCircle && bIsCircle)
	{
		return IsCollidingCircleCircle(a, b, contact);
	}
	// TODO: Detect collision between other shapes
	return false;
}
bool CollisionDetection::IsCollidingCircleCircle(Body * a, Body * b, Contact & contact)
{
	CircleShape * aCircleShape = static_cast<CircleShape *>(a->shape);
	CircleShape * bCircleShape = static_cast<CircleShape *>(b->shape);

	Vec2 ab = b->position - a->position;
	const float radiusSum = aCircleShape->radius + bCircleShape->radius;
	bool isColliding = ab.MagnitudeSquared() <= radiusSum * radiusSum;
	if(!isColliding)
	{
		return false;
	}
	// We have a collision
	contact.a = a;
	contact.b = b;

	contact.normal = ab;
	contact.normal.Normalize();

	contact.start = b->position - contact.normal * bCircleShape->radius;
	contact.end = a->position + contact.normal * aCircleShape->radius;

	contact.depth = (contact.end - contact.start).Magnitude();

	return true;
}