#include "CollisionDetection.h"

#include "../GraphicHelper.h"
#include "../Graphics.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
	ShapeType aShapeType = a->shape->GetType();
	ShapeType bShapeType = b->shape->GetType();

	bool aIsCircle = aShapeType == CIRCLE;
	bool bIsCircle = bShapeType == CIRCLE;

	bool aIsPolygon = aShapeType == POLYGON || aShapeType == BOX;
	bool bIsPolygon = bShapeType == POLYGON || bShapeType == BOX;
	if (aIsCircle && bIsCircle)
	{
		return IsCollidingCircleCircle(a, b, contact);
	}
	if (aIsPolygon && bIsPolygon)
	{
		return IsCollidingPolygonPolygon(a, b, contact);
	}
	if (aIsPolygon && bIsCircle)
	{
		return IsCollodingPolygonCircle(a, b, contact);
	}
	if (aIsCircle && bIsPolygon)
	{
		return IsCollodingPolygonCircle(b, a, contact);
	}

	return false;
}
bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact)
{
	CircleShape* aCircleShape = static_cast<CircleShape*>(a->shape);
	CircleShape* bCircleShape = static_cast<CircleShape*>(b->shape);

	Vec2        ab = b->position - a->position;
	const float radiusSum = aCircleShape->radius + bCircleShape->radius;
	bool        isColliding = ab.MagnitudeSquared() <= radiusSum * radiusSum;
	if (!isColliding)
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

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
	const auto* shapeA = static_cast<PolygonShape*>(a->shape);
	const auto* shapeB = static_cast<PolygonShape*>(b->shape);

	// SAT: find the axis of least penetration from each polygon's edges
	Vec2 edgeNormalA, penetrationPointA;
	// testing A's edges against B's vertices. The returned point is the vertex of B that penetrates deepest into A
	float separationAB = shapeA->FindMinSeparation(*shapeB, edgeNormalA, penetrationPointA);
	if (separationAB >= 0) return false;

	Vec2  edgeNormalB, penetrationPointB;
	float separationBA = shapeB->FindMinSeparation(*shapeA, edgeNormalB, penetrationPointB);
	if (separationBA >= 0) return false;

	// Both separations are negative — pick the axis with least penetration	(closest to 0)
	contact.a = a;
	contact.b = b;

	if (separationAB > separationBA)
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

bool CollisionDetection::IsCollodingPolygonCircle(Body* polygon, Body* circle, Contact& contact)
{
	const PolygonShape*      polygonShape = static_cast<PolygonShape*>(polygon->shape);
	const std::vector<Vec2>& vertices = polygonShape->worldVertices;
	Vec2                     center = circle->position;

	// Find the edge whose outward normal gives the greatest projection of the circle center.
	// maxSeparation >= 0 => center is outside that edge's plane; < 0 => inside polygon.
	Vec2  nearestEdgeStart;
	Vec2  nearestEdgeEnd;
	float maxSeparation = std::numeric_limits<float>::lowest();

	// Find the nearest edge in the polygon to the circle center
	for (int i = 0; i < vertices.size(); i++)
	{
		Vec2 edge = polygonShape->EdgeAt(i);
		Vec2 normal = edge.Normal();

		float projection = (center - vertices[i]).Dot(normal);
		if (projection > 0 && projection > maxSeparation)
		{
			maxSeparation = projection;
			nearestEdgeStart = vertices[i];
			nearestEdgeEnd = vertices[(i + 1) % vertices.size()];
		}
	}
	Graphics::DrawFillCircle(nearestEdgeStart.x, nearestEdgeStart.y, 5, GraphicHelper::ColorYellow);
	Graphics::DrawFillCircle(nearestEdgeEnd.x, nearestEdgeEnd.y, 5, GraphicHelper::ColorYellow);
	return false;
}
