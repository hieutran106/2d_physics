#pragma once
#include "Body.h"
#include "MatMN.h"

class Constraint
{
public:
	Body* a = nullptr;
	Body* b = nullptr;
	Constraint(Body* a, Body* b);

	virtual ~Constraint() = default;

	MatMN        GetInvM() const;
	VecN         GetVelocities() const;
	virtual void Solve(float dt) = 0;
};


class JointConstraint : public Constraint
{
private:
	MatMN jacobian;
	// the anchor point the A's local space
	Vec2 aPoint;
	// The anchor point in B's local space
	Vec2 bPoint;

public:
	// JointConstraint() = default;
	JointConstraint(Body* a, Body* b, const Vec2& anchorPosition);

	void Solve(float dt) override;
};

// class PenetrationConstraint : public Constraint
// {
// };
