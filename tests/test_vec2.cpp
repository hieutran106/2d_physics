#include "Physics/Body.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"
#include "Physics/Shape.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("Collision Detection - IsCollidingPolygonPolygon", "[Vec2]")
{
	Body * boxA = new Body(BoxShape(200, 200), 0, 0, 1.0);
	BoxShape * shapeA = static_cast<BoxShape *>(boxA->shape);
	shapeA->worldVertices.clear();
	shapeA->worldVertices.emplace_back(630, 340);
	shapeA->worldVertices.emplace_back(829, 360);
	shapeA->worldVertices.emplace_back(809, 559);
	shapeA->worldVertices.emplace_back(610, 539);

	Body * boxC = new Body(BoxShape(200, 200), 0, 0, 1.0);
	BoxShape * shapeC = static_cast<BoxShape *>(boxC->shape);
	shapeC->worldVertices.clear();
	shapeC->worldVertices.emplace_back(676, 511);
	shapeC->worldVertices.emplace_back(771, 541);
	shapeC->worldVertices.emplace_back(741, 636);
	shapeC->worldVertices.emplace_back(646, 606);

	Contact contact;
	bool actual = CollisionDetection::IsColliding(boxA, boxC, contact);
	REQUIRE(actual == true);
}

TEST_CASE("Example 1", "[Vec2]")
{
	REQUIRE(1 == 2);
}
