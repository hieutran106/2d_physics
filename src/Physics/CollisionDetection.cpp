#include "CollisionDetection.h"
#include <limits>

bool CollisionDetection::IsColliding(Body * a, Body * b, Contact & contact)
{
	ShapeType aShapeType = a->shape->GetType();
	ShapeType bShapeType = b->shape->GetType();

	bool aIsCircle = aShapeType == CIRCLE;
	bool bIsCircle = bShapeType == CIRCLE;

	bool aIsPolygon = aShapeType == POLYGON || aShapeType == BOX;
	bool bIsPolygon = bShapeType == POLYGON || bShapeType == BOX;
	if(aIsCircle && bIsCircle)
	{
		return IsCollidingCircleCircle(a, b, contact);
	}
	if(aIsPolygon && bIsPolygon)
	{
		return IsCollidingPolygonPolygon(a, b, contact);
	}

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

float FindMinSeparation(const PolygonShape & a, const PolygonShape & b)
{
	float separation = std::numeric_limits<float>::lowest();
	for(int i = 0; i < a.worldVertices.size(); i++)
	{
		Vec2 va = a.worldVertices[i];
		Vec2 normal = a.EdgeAt(i).Normal();

		float minProjection = std::numeric_limits<float>::max();
		for(int j = 0; j < b.worldVertices.size(); j++)
		{
			Vec2 vb = b.worldVertices[j];
			// Project the vertex b (vb) onto the normal axis
			auto projection = (vb - va).Dot(normal);
			minProjection = std::min(minProjection, projection);
		}
		separation = std::max(separation, minProjection);
	}
	return separation;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body * a, Body * b, Contact & contact)
{
	PolygonShape * aPolygonShape = static_cast<PolygonShape *>(a->shape);
	PolygonShape * bPolygonShape = static_cast<PolygonShape *>(b->shape);

	if(aPolygonShape->FindMinSeparation(*bPolygonShape) >= 0)
	{
		return false;
	}
	if(bPolygonShape->FindMinSeparation(*aPolygonShape) >= 0)
	{
		return false;
	}

	return true;
}