#include "Constraint.h"

Constraint::Constraint(Body* a, Body* b)
	: a(a)
	, b(b)
{
}
MatMN Constraint::GetInvM() const
{
	MatMN invM(6, 6);
	// TODO: Why do we need to zero
	// invM.Zero();

	invM.rows[0][0] = a->invMass;
	invM.rows[1][1] = a->invMass;
	invM.rows[2][2] = a->invI;

	invM.rows[3][3] = b->invMass;
	invM.rows[4][4] = b->invMass;
	invM.rows[5][5] = b->invI;
	return invM;
}
VecN Constraint::GetVelocities() const
{
	VecN V(6);
	V[0] = a->velocity.x;
	V[1] = a->velocity.y;
	V[2] = a->angularVelocity;

	V[3] = b->velocity.x;
	V[4] = b->velocity.y;
	V[5] = b->angularVelocity;
	return V;
}
JointConstraint::JointConstraint(Body* a, Body* b, const Vec2& anchorPosition)
	: Constraint(a, b)
	, jacobian(1, 6)
	, aPoint(a->WorldSpaceToLocalSpace(anchorPosition))
	, bPoint(b->WorldSpaceToLocalSpace(anchorPosition))

{
	// With the body assignment, aPoint and bPoint are default-constructed and then assigned, so each is set twice.
	// The init list constructs each one once with its final value. For a Vec2 the cost difference is negligible,
	// so this is mostly about style and consistency.

	// this->aPoint = a->WorldSpaceToLocalSpace(anchorPosition); this->bPoint =
	// b->WorldSpaceToLocalSpace(anchorPosition);
}
void JointConstraint::Solve(float dt) { }
