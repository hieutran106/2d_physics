#include "Application.h"

#define TRIGGER_BREAKPOINT() __asm__ volatile("brk #0xf000")

#include "GraphicHelper.h"
#include "Graphics.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include "Physics/Log.h"

#include <format>

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////

void Setup_TwoBox(std::vector<Body*>& bodies)
{
	Body* floor =
		new Body(BoxShape(Graphics::Width() - 50, 100), Graphics::Width() / 2.0, Graphics::Height() - 40, 0.0);
	floor->restitution = 0.2;
	bodies.push_back(floor);

	// Add a static box so other boxes can collide
	Body* bigBox = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
	bigBox->rotation = 1.4;
	bigBox->restitution = 0.5;

	bodies.push_back(bigBox);
}

void Setup_Cirlce(std::vector<Body*> bodies)
{
	Body* circleA = new Body(CircleShape(100), 800, 800, 1.0);
	Body* circleB = new Body(CircleShape(100), 800, 800, 1.0);
}
void Application::Setup()
{
	mRunning = Graphics::InitializeWindow("2d physics", 960, 720);
	Setup_TwoBox(bodies);
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update(float deltaTime)
{
	Graphics::ClearScreen(0xFF056263);
	for (auto body : bodies)
	{
		// body->AddForce(pushForce);

		// Apply drag force
		// Vec2 drag = Force::GenerateDragForce(*body, 0.002);
		// body->AddForce(drag);

		// Apply weight force
		Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
		body->AddForce(weight);
		//
		// Vec2 wind = Vec2(2.0 * PIXELS_PER_METER, 0);
		// body->AddForce(wind);

		// float torque = 400;
		// body->AddTorque(torque);
	}

	for (auto body : bodies)
	{
		body->Update(deltaTime);
	}

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
				GraphicHelper::DrawDebugCollision(contact);
				a->isColliding = true;
				b->isColliding = true;
			}
		}
	}

	// Check the boundaries of the window
	for (auto body : bodies)
	{
		if (body->shape->GetType() == CIRCLE)
		{
			CircleShape* circle = static_cast<CircleShape*>(body->shape);
			float        radius = circle->radius;
			// Nasty hardcoded flip in velocity if it touches the limits of the screen window
			if (body->position.x - radius < 0)
			{
				body->position.x = radius;
				body->velocity.x *= -0.9;
			}
			else if (body->position.x + radius > Graphics::Width())
			{
				body->position.x = Graphics::Width() - radius;
				body->velocity.x *= -0.9;
			}
			if (body->position.y - radius < 0)
			{
				body->position.y = radius;
				body->velocity.y *= -0.9;
			}
			else if (body->position.y + radius >= Graphics::Height())
			{
				body->position.y = Graphics::Height() - radius;
				body->velocity.y *= -0.9;
			}
		}
	}

	Physics::LogDebug("============================= Frame update ended =============================");
}

void Application::Render()
{
	// Graphics::ClearScreen(0xFF056263);
	if (leftMouseButtonDown)
	{
		Graphics::DrawLine(
			bodies[bodies.size() - 1]->position.x,
			bodies[bodies.size() - 1]->position.y,
			mouseCursor.x,
			mouseCursor.y,
			0xFF0000FF);
	}

	// Draw all bodies
	for (Body* body : bodies)
	{
		uint32_t strokeColor = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
		strokeColor = 0xFFFFFFFF;
		if (body->shape->GetType() == CIRCLE)
		{
			auto* circle = static_cast<CircleShape*>(body->shape);
			Graphics::DrawCircle(body->position.x, body->position.y, circle->radius, body->rotation, strokeColor);
		}
		if (body->shape->GetType() == BOX)
		{
			BoxShape* boxShape = static_cast<BoxShape*>(body->shape);
			Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, strokeColor);
		}
	}

	Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		SDL_Keycode key = event.key.key;
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			mRunning = false;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (key == SDLK_ESCAPE) mRunning = false;
			if (key == SDLK_P)
			{
				// TODO: Print world vertices of all polygon body
				for (int i = 0; i < bodies.size(); i++)
				{
					Body*     body = bodies[i];
					ShapeType shapeType = body->shape->GetType();
					if (shapeType == POLYGON || shapeType == BOX)
					{
						SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Bodies[%d]", i);
						PolygonShape* polygonShape = static_cast<PolygonShape*>(body->shape);
						for (int j = 0; j < polygonShape->worldVertices.size(); j++)
						{
							Vec2        vertex = polygonShape->worldVertices[j];
							std::string out = std::format("\tVertex[{}]: ({:.4f},{:.4f})", j, vertex.x, vertex.y);
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", out.c_str());
						}
					}
				}
				// Run collision detection and print
				Contact contact;
				bool    collide = CollisionDetection::IsColliding(bodies[0], bodies[1], contact);
				if (collide)
				{
					SDL_Log("Contact start (%.4f, %.4f)", contact.start.x, contact.start.y);
				}
			}
			if (key == SDLK_K)
			{
				TRIGGER_BREAKPOINT(); // Automatically halts execution in CLion
			}
			if (key == SDLK_UP) pushForce.y = -50 * PIXELS_PER_METER;
			if (key == SDLK_RIGHT) pushForce.x = 50 * PIXELS_PER_METER;
			if (key == SDLK_DOWN) pushForce.y = 50 * PIXELS_PER_METER;
			if (key == SDLK_LEFT) pushForce.x = -50 * PIXELS_PER_METER;
			break;
		case SDL_EVENT_KEY_UP:
			if (key == SDLK_UP) pushForce.y = 0;
			if (key == SDLK_RIGHT) pushForce.x = 0;
			if (key == SDLK_DOWN) pushForce.y = 0;
			if (key == SDLK_LEFT) pushForce.x = 0;
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			float x = event.motion.x;
			float y = event.motion.y;
			Body* box = new Body(BoxShape(100, 100), x * 2, y * 2, 1.0);
			bodies.push_back(box);
			break;
		}
			// case SDL_EVENT_MOUSE_BUTTON_DOWN:
			// {
			// 	if(event.button.button == SDL_BUTTON_LEFT && !leftMouseButtonDown)
			// 	{
			// 		float x = event.motion.x;
			// 		float y = event.motion.y;
			// 		Body * smallBall = new Body(CircleShape(20), x * 2, y * 2, 1.0);
			// 		bodies.push_back(smallBall);
			// 	}
			// 	break;
			// }
			// case SDL_EVENT_MOUSE_MOTION:
			// {
			// 	Vec2 mousePosition{event.motion.x, event.motion.y};
			// 	mousePosition = mousePosition * Graphics::windowInfo.density;
			// 	bodies[0]->position = mousePosition;
			// 	break;
			// }
			// case SDL_EVENT_MOUSE_BUTTON_UP:
			// {
			// 	if(leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
			// 	{
			// 		leftMouseButtonDown = false;
			// 		Body * last = bodies[bodies.size() - 1];
			// 		Vec2 impulseDirection = (last->position - mouseCursor).UnitVector();
			// 		float impulseMagnitude = (last->position - mouseCursor).Magnitude() * 5.0;
			// 		last->velocity = impulseDirection * impulseMagnitude;
			// 	}
			// 	break;
			// }
		}
	}
}

void Application::Destroy()
{
	for (auto p : bodies)
	{
		delete p;
	}
	Graphics::CloseWindow();
}
