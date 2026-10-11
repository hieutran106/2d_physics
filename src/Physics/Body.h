#pragma once
#include "Shape.h"
#include "Vec2.h"

struct Body
{
	// Linear motion
	Vec2 position;
	// Linear velocity
	Vec2 velocity;
	Vec2 acceleration;

	// Angular motion
	float rotation{};
	float angularVelocity{};
	float angularAcceleration{};

	// Forces and Torque
	Vec2  sumForces;
	float sumTorque = 0;

	// Mass & Moment of Inertia
	float mass;
	float invMass;
	float I;
	float invI;

	// Coefficient of restitution
	float restitution{ 1.0 };

	// Coefficient of friction
	float friction{ 0.7 };

	// Collision
	bool isColliding = false;

	// A pointer to the geometry
	Shape* shape = nullptr;

	Body(const Shape& shape, float x, float y, float m);
	~Body();

	bool IsStatic() const;
	void Update(float dt);

	void IntegrateLinear(float dt);
	void IntegrateAngular(float dt);
	void AddForce(const Vec2& force);
	void ClearForces();

	void AddTorque(float torque);
	void ClearTorque();

	/*
	 * Impulse is an instantaneous change in velocity
	 * inversely proportional to the mass of the object
	 * Impulse J is the change in momentum: J = ∆P = m*∆v
	 * Therefore, the change in velocity is: ∆v = J/m
	 */
	// void ApplyImpulse(const Vec2& J);

	// Change velocity and angular velocity
	void ApplyImpulse(const Vec2& J, const Vec2& r);
};

// Formatter for Body
template <>
struct std::formatter<Body> : std::formatter<std::string>
{
	auto format(const Body& v, std::format_context& ctx) const
	{
		std::string shapeStr;
		auto        shapeType = v.shape->GetType();
		if (shapeType == BOX)
		{
			shapeStr = "BOX";
		}
		else if (shapeType == CIRCLE)
		{
			shapeStr = "CIRCLE";
		}
		else if (shapeType == POLYGON)
		{
			shapeStr = "POLYGON";
		}
		std::string staticStr = v.IsStatic() ? "STATIC" : "";
		return std::format_to(ctx.out(), "({} {} pos={})", shapeStr, staticStr, v.position);
	}
};
