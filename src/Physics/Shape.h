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

	/**
	 * @brief Finds the minimum separation between this polygon and another using the Separating Axis Theorem (SAT).
	 *
	 * Iterates over each edge of this polygon, computes its outward normal, and projects all vertices
	 * of the other polygon onto that normal to find the minimum separation distance. Tracks the edge
	 * axis and vertex that produce the least negative (or most positive) separation.
	 *
	 * @param[in] other The other polygon shape to test separation against.
	 * @param[out] axis The edge of this polygon corresponding to the axis of minimum separation.
	 * @param[out] point The vertex on the other polygon that is closest along the axis of minimum separation.
	 * @return The minimum separation distance. A negative value indicates overlap; a positive value
	 *         indicates the polygons are separated.
	 */
	float FindMinSeparation(const PolygonShape & other, Vec2 & axis, Vec2 & point) const;
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