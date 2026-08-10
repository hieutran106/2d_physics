#include "Shape.h"
// #include <SDL3/SDL.h>

CircleShape::CircleShape(float radius) : radius(radius) {}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

float CircleShape::GetMomentOfInertia() const
{
	// 1/2 * r ^2, but still needs to be multiplied by the rigidbody's mass
	return radius * radius / 2;
}

Shape * CircleShape::Clone() const
{
	return new CircleShape(radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> & vertices) : localVertices(vertices) {}
ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}
float PolygonShape::GetMomentOfInertia() const
{
	return 0;
}
Vec2 PolygonShape::EdgeAt(int index) const
{
	int nextVertex = (index + 1) % worldVertices.size();
	return worldVertices[nextVertex] - worldVertices[index];
}
float PolygonShape::FindMinSeparation(const PolygonShape & other, Vec2 & axis, Vec2 & point) const
{
	float separation = std::numeric_limits<float>::lowest();
	for(int i = 0; i < worldVertices.size(); i++)
	{
		Vec2 va = worldVertices[i];
		Vec2 normal = EdgeAt(i).Normal();

		float minSep = std::numeric_limits<float>::max();
		Vec2 minVertex;
		for(int j = 0; j < other.worldVertices.size(); j++)
		{
			Vec2 vb = other.worldVertices[j];
			// Project the vertex b (vb) onto the normal axis
			float projection = (vb - va).Dot(normal);
			// minProjection = std::min(minProjection, projection);
			if(projection < minSep)
			{
				minSep = projection;
				// update point
				minVertex = vb;
			}
		}
		// separation = std::max(separation, minProjection);
		if(minSep > separation)
		{
			separation = minSep;
			axis = this->EdgeAt(i);
			point = minVertex;
		}
	}
	return separation;
}

Shape * PolygonShape::Clone() const
{
	return new PolygonShape(localVertices);
}
void PolygonShape::UpdateVertices(float rotation, const Vec2 & position)
{
	for(int i = 0; i < localVertices.size(); i++)
	{
		// First, rotate
		worldVertices[i] = localVertices[i].Rotate(rotation);
		// Then translate
		worldVertices[i] += position;
	}
}

BoxShape::BoxShape(float width, float height) : width(width), height(height)
{
	localVertices.push_back(Vec2(-width / 2, -height / 2));
	localVertices.push_back(Vec2(width / 2, -height / 2));
	localVertices.push_back(Vec2(width / 2, height / 2));
	localVertices.push_back(Vec2(-width / 2, height / 2));

	// Initialize world vertices, to prepare for in-place update
	worldVertices.push_back(Vec2(-width / 2, -height / 2));
	worldVertices.push_back(Vec2(width / 2, -height / 2));
	worldVertices.push_back(Vec2(width / 2, height / 2));
	worldVertices.push_back(Vec2(-width / 2, height / 2));
}

ShapeType BoxShape::GetType() const
{
	return BOX;
}
float BoxShape::GetMomentOfInertia() const
{
	return (width * width + height * height) / 12;
}

Shape * BoxShape::Clone() const
{
	return new BoxShape(width, height);
}