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
	std::vector<Vec2> vertices;

	PolygonShape() = default;
	explicit PolygonShape(const std::vector<Vec2> & vertices);
	~PolygonShape() override = default;
	ShapeType GetType() const override;
	float GetMomentOfInertia() const override;
	Shape * Clone() const override;
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