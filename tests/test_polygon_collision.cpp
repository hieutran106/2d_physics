#include "Physics/Body.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"
#include "Physics/Shape.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;
using Catch::Matchers::WithinRel;

TEST_CASE("Collision Detection - IsCollidingPolygonPolygon", "")
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

TEST_CASE("CollisionDetection 1", "[Vec2]")
{
	Body * boxA = new Body(BoxShape(200, 200), 0, 0, 1.0);
	BoxShape * shapeA = static_cast<BoxShape *>(boxA->shape);
	shapeA->worldVertices = {
		{1437.7372, 957.1821 },
        {1304.4819, 1106.3231},
        {1155.3409, 973.0679 },
        {1288.5962, 823.9269 }
	};

	Body * boxB = new Body(BoxShape(200, 200), 0, 0, 1.0);
	BoxShape * shapeB = static_cast<BoxShape *>(boxB->shape);
	shapeB->worldVertices = {
		{1052, 764},
        {1252, 764},
        {1252, 964},
        {1052, 964}
	};

	Contact contact;
	bool actual = CollisionDetection::IsColliding(boxA, boxB, contact);
	Vec2 start = contact.start;
	REQUIRE(actual == true);
	REQUIRE_THAT(start.x, WithinRel(1252.0f));
	REQUIRE_THAT(start.y, WithinRel(964.0f));
}

TEST_CASE("CollisionDetection 2", "[Vec2]")
{
	Body * boxA = new Body(BoxShape(200, 200), 0, 0, 1.0);
	BoxShape * shapeA = static_cast<BoxShape *>(boxA->shape);
	shapeA->worldVertices = {
		{836.2188,  781.2734},
        {1036.2188, 781.2734},
        {1036.2188, 981.2734},
        {836.2188,  981.2734}
	};

	Body * boxB = new Body(BoxShape(200, 200), 0, 0, 1.0);
	BoxShape * shapeB = static_cast<BoxShape *>(boxB->shape);
	shapeB->worldVertices = {
		{952,  664 },
        {1352, 664 },
        {1352, 1064},
        {952,  1064}
	};

	Contact contact;
	bool actual = CollisionDetection::IsColliding(boxA, boxB, contact);
	Vec2 start = contact.start;
	REQUIRE(actual == true);
	REQUIRE_THAT(start.x, WithinRel(952.0f));
	REQUIRE_THAT(start.y, WithinRel(781.2734f));
}
