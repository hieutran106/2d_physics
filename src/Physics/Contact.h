#pragma once
#include "Body.h"

/*
 * Collision contact information
 * 	- Start and end contact points (from "a" to "b")
 * 	- Normal direction is always from "a" to "b"
 * 	- Depth is the penetration length of the collision
 */
struct Contact
{
	Body * a;
	Body * b;

	Vec2 start;
	Vec2 end;

	Vec2 normal;
	float depth;

	Contact() = default;
	~Contact() = default;

	void ResolvePenetration();
	void ResolveCollision();
};