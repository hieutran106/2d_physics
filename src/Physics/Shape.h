#pragma once
#include "Vec2.h"
#include <vector>

enum ShapeType
{
	CIRCLE,
	POLYGON,
	BOX
};

struct Shape
{
	virtual ~Shape() = default;
	virtual ShapeType GetType() const = 0;
	virtual float GetMomentOfInertia() const = 0;
	virtual Shape * Clone() const = 0;
};

struct CircleShape : public Shape
{
	float radius;
	explicit CircleShape(float radius);
	~CircleShape() override = default;
	ShapeType GetType() const override;
	float GetMomentOfInertia() const override;
	Shape * Clone() const override;
};

struct PolygonShape : public Shape
{
	std::vector<Vec2> localVertices;
	std::vector<Vec2> worldVertices;

	PolygonShape() = default;
	explicit PolygonShape(const std::vector<Vec2> & vertices);
	~PolygonShape() override = default;
	ShapeType GetType() const override;
	float GetMomentOfInertia() const override;
	Vec2 EdgeAt(int index) const;
	float FindMinSeparation(const PolygonShape & other) const;
	Shape * Clone() const override;
	// Function to transform the polygon vertices from "local space" to "world space"
	void UpdateVertices(float rotation, const Vec2 & position);
};

struct BoxShape : public PolygonShape
{
	float width, height;
	BoxShape(float width, float height);
	~BoxShape() override = default;
	ShapeType GetType() const override;
	float GetMomentOfInertia() const override;
	Shape * Clone() const override;
};