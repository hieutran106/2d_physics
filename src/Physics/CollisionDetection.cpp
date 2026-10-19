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
		return IsCollidingPolygonCircle(a, b, contact);
	}
	if (aIsCircle && bIsPolygon)
	{
		return IsCollidingPolygonCircle(b, a, contact);
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

bool CollisionDetection::IsCollidingPolygonCircle(Body* polygon, Body* circle, Contact& contact)
{
	const PolygonShape*      polygonShape = static_cast<PolygonShape*>(polygon->shape);
	const CircleShape*       circleShape = static_cast<CircleShape*>(circle->shape);
	const std::vector<Vec2>& vertices = polygonShape->worldVertices;
	Vec2                     center = circle->position;

	// Find the edge whose outward normal gives the greatest projection of the circle center.
	Vec2 nearestEdgeStart;
	Vec2 nearestEdgeEnd;
	// Max separation also the distance from the circle center to the edge
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


	bool isInside = maxSeparation >= 0;
	Vec2 edge = nearestEdgeEnd - nearestEdgeStart;

	if (isInside)
	{
		// The center of the circle is inside the polygon. It is definitely colliding.
		contact.a = polygon;
		contact.b = circle;
		contact.depth = circleShape->radius - maxSeparation;
		contact.normal = edge.Normal();
		contact.start = circle->position + contact.normal * circleShape->radius;
		contact.end = contact.start + contact.normal * contact.depth;
		return true;
	}

	// Circle is outside
	float d1 = (center - nearestEdgeStart).Dot(edge);
	float d2 = (center - nearestEdgeEnd).Dot(edge);
	if (d1 < 0 && d2 < 0)
	{
		// Circle center is in region A
		Graphics::DrawFillCircle(nearestEdgeStart.x, nearestEdgeStart.y, 10, GraphicHelper::ColorRed);
		Graphics::DrawFillCircle(nearestEdgeEnd.x, nearestEdgeEnd.y, 5, GraphicHelper::ColorWhite);
	}
	else if (d1 > 0 && d2 > 0)
	{
		// Circle center is in region B
		Graphics::DrawFillCircle(nearestEdgeStart.x, nearestEdgeStart.y, 5, GraphicHelper::ColorWhite);
		Graphics::DrawFillCircle(nearestEdgeEnd.x, nearestEdgeEnd.y, 10, GraphicHelper::ColorRed);
	}
	else
	{
		// Circle is in region C
		Graphics::DrawFillCircle(nearestEdgeStart.x, nearestEdgeStart.y, 10, GraphicHelper::ColorRed);
		Graphics::DrawFillCircle(nearestEdgeEnd.x, nearestEdgeEnd.y, 10, GraphicHelper::ColorRed);
	}


	return false;
}
