#include "Physics/Body.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include <catch2/catch_test_macros.hpp>

using Catch::Matchers::WithinAbs;
using Catch::Matchers::WithinRel;

TEST_CASE("CircleCirlcle ResolveCollision", "")
{
	Body circleA(CircleShape(100), 400, 400, 1.0);
	circleA.velocity = Vec2(100, 0);

	Body circleB(CircleShape(100), 550, 400, 1.0);
	circleB.velocity = Vec2(-100, 0);

	Contact contact;
	bool actual = CollisionDetection::IsCollidingCircleCircle(&circleA, &circleB, contact);
	REQUIRE(actual == true);

	REQUIRE_THAT(contact.depth, WithinRel(50.0f));

	REQUIRE(contact.start == Vec2(450, 400));
	REQUIRE(contact.end == Vec2(500, 400));
	REQUIRE(contact.normal == Vec2(1, 0));

	contact.ResolveCollision();
	REQUIRE(circleA.position == Vec2(375, 400));
	REQUIRE(circleA.velocity == Vec2(-100, 0));

	REQUIRE(circleB.position == Vec2(575, 400));
	REQUIRE(circleB.velocity == Vec2(100, 0));
}