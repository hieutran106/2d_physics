#include "World.h"

#include "CollisionDetection.h"
#include "Constants.h"
#include "Contact.h"

World::World(float gravity)
	: G(-gravity)
{
}
World::~World()
{
	for (const Body* body : bodies)
	{
		delete body;
	}
}

void World::AddBody(Body* body)
{
	bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies()
{
	return bodies;
}

void World::AddContraint(Constraint* constraint)
{
	constraints.push_back(constraint);
}

std::vector<Constraint*>& World::GetConstraints()
{
	return constraints;
}

void World::AddForce(const Vec2& force)
{
	forces.push_back(force);
}

void World::AddTorque(float torque)
{
	torques.push_back(torque);
}
void World::Update(float dt)
{
	// Loop all bodies of the world and apply forces
	for (Body* body : bodies)
	{
		// Apply the weight force
		Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
		body->AddForce(weight);

		// Apply forces to all bodies
		for (const Vec2& force : forces)
		{
			body->AddForce(force);
		}
		// Apply torque to all bodies
		for (float torque : torques)
		{
			body->AddTorque(torque);
		}
	}

	for (Body* body : bodies)
	{
		body->Update(dt);
	}

	CheckCollisions();
}
void World::CheckCollisions()
{
	for (const auto body : bodies)
	{
		body->isColliding = false;
	}
	// Check all the bodies for collision
	for (int i = 0; i < bodies.size() - 1; i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body*   a = bodies[i];
			Body*   b = bodies[j];
			Contact contact;
			bool    isColliding = CollisionDetection::IsColliding(a, b, contact);
			if (isColliding)
			{
				contact.ResolveCollision();
				// if (debug)
				// {
				// 	GraphicHelper::DrawDebugCollision(contact);
				// }
				a->isColliding = true;
				b->isColliding = true;
			}
		}
	}
}
