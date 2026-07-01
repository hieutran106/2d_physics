#include "Shape.h"
#include <SDL3/SDL.h>

CircleShape::CircleShape(float radius) : radius(radius) {}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

float CircleShape::GetMomentOfInertia() const
{
	return 0;
}

Shape * CircleShape::Clone() const
{
	return new CircleShape(radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> & vertices) : vertices(vertices) {}
ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}
float PolygonShape::GetMomentOfInertia() const
{
	return 0;
}

Shape * PolygonShape::Clone() const
{
	return new PolygonShape(vertices);
}

BoxShape::BoxShape(float width, float height) : width(width), height(height) {}

ShapeType BoxShape::GetType() const
{
	return BOX;
}
float BoxShape::GetMomentOfInertia() const
{
	return 0;
}

Shape * BoxShape::Clone() const
{
	return new BoxShape(width, height);
}