#include "CollisionDetection.h"

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

bool CollisionDetection::IsCollidingPolygonPolygon(Body * a, Body * b, Contact & contact)
{
	const auto * shapeA = static_cast<PolygonShape *>(a->shape);
	const auto * shapeB = static_cast<PolygonShape *>(b->shape);

	// SAT: find the axis of least penetration from each polygon's edges
	Vec2 edgeNormalA, penetrationPointA;
	// testing A's edges against B's vertices. The returned point is the vertex of B that penetrates deepest into A
	float separationAB = shapeA->FindMinSeparation(*shapeB, edgeNormalA, penetrationPointA);
	if(separationAB >= 0)
		return false;

	Vec2 edgeNormalB, penetrationPointB;
	float separationBA = shapeB->FindMinSeparation(*shapeA, edgeNormalB, penetrationPointB);
	if(separationBA >= 0)
		return false;

	// Both separations are negative — pick the axis with least penetration	(closest to 0)
	contact.a = a;
	contact.b = b;

	if(separationAB > separationBA)
	{
		// A's edge is the reference face
		contact.normal = edgeNormalA.Normal();
		contact.depth = -separationAB;
		contact.start = penetrationPointA;
		contact.end = penetrationPointA + contact.normal * contact.depth;
	}
	else
	{
		// B's edge is the reference face — flip normal to maintain A→B convention
		contact.normal = -edgeNormalB.Normal();
		contact.depth = -separationBA;
		contact.end = penetrationPointB;
		contact.start = penetrationPointB - contact.normal * contact.depth;
	}

	return true;
}
