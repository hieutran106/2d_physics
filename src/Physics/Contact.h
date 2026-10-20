#pragma once
#include "Body.h"

/*
 * Collision contact information
 * 	- Start and end contact points. If body A collides with body B, start point is on B, and end point is on A
 * 	- Normal direction is always from start->end
 * 	- Depth is the penetration length of the collision
 */
struct Contact
{
	Body* a;
	Body* b;

	// Start point is on B
	Vec2 start;
	// End point is on A
	Vec2 end;

	// Normal direction is always from start->end, aka from body B -> body A
	Vec2  normal;
	float depth;

	Contact() = default;
	~Contact() = default;

	void ResolvePenetration();
	void ResolveCollision();
};
