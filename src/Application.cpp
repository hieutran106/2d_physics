#include "Application.h"

#define TRIGGER_BREAKPOINT() __asm__ volatile("brk #0xf000")

#include "GraphicHelper.h"
#include "Graphics.h"
#include "SDLHelper.h"
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
	BoxShape floorShape = BoxShape(Graphics::Width() - 50, 80);
	Body* floor = new Body(floorShape, Graphics::Width() / 2.0, Graphics::Height() - floorShape.height - 50 / 2, 0.0);

	BoxShape wallShape = BoxShape(50, Graphics::Height() - 100);
	Body*    leftWall = new Body(wallShape, 50, Graphics::Height() / 2 - 25, 0);
	Body*    rightWall = new Body(wallShape, Graphics::Width() - 50, Graphics::Height() / 2 - 25, 0);

	floor->restitution = 0.5;
	leftWall->restitution = 0.2;
	rightWall->restitution = 0.2;

	bodies.push_back(floor);
	bodies.push_back(leftWall);
	bodies.push_back(rightWall);

	Body* bigBox = new Body(BoxShape(400, 400), Graphics::Width() / 2, Graphics::Height() / 2, 0.0);
	bigBox->SetTexture("assets/crate.png");
	bigBox->restitution = 0.7;
	bigBox->rotation = 1.4;
	bodies.push_back(bigBox);

	// Body* ball = new Body(CircleShape(200), Graphics::Width() / 2, Graphics::Height() / 2, 1.0);
	// ball->restitution = 0.1;
	// bodies.push_back(ball);
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
				if (debug)
				{
					GraphicHelper::DrawDebugCollision(contact);
				}
				a->isColliding = true;
				b->isColliding = true;
			}
		}
	}

	Physics::LogVerbose("============================= Frame update ended =============================");
}

void Application::Render()
{
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
		uint32_t strokeColor = GraphicHelper::ColorWhite;
		if (debug)
		{
			strokeColor = body->isColliding ? GraphicHelper::ColorRed : GraphicHelper::ColorWhite;
		}

		if (body->shape->GetType() == CIRCLE)
		{
			auto* circle = static_cast<CircleShape*>(body->shape);
			Graphics::DrawCircle(body->position.x, body->position.y, circle->radius, body->rotation, strokeColor);
		}
		if (body->shape->GetType() == BOX)
		{
			BoxShape* boxShape = static_cast<BoxShape*>(body->shape);
			if (!debug && body->texture)
			{
				Graphics::DrawTexture(
					body->position.x,
					body->position.y,
					boxShape->width,
					boxShape->height,
					body->rotation,
					body->texture);
			}
			else
			{
				Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, strokeColor, debug);
			}
		}
		if (body->shape->GetType() == POLYGON)
		{
			PolygonShape* polygonShape = static_cast<PolygonShape*>(body->shape);
			Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, strokeColor, debug);
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
		case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
		{
			SDL_DisplayID          display = (SDL_DisplayID)event.window.data1;
			const char*            name = SDL_GetDisplayName(display);
			const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
			SDL_Log("Window moved to display %u: %s, resolution: %d x %d", display, name, mode->w, mode->h);
			break;
		}
		case SDL_EVENT_QUIT:
			mRunning = false;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (key == SDLK_ESCAPE) mRunning = false;
			if (key == SDLK_D) debug = !debug;
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
			// case SDL_EVENT_MOUSE_BUTTON_DOWN:
			// {
			// 	float x = event.motion.x * Graphics::PixelDensity();
			// 	float y = event.motion.y * Graphics::PixelDensity();
			// 	Body* ball = new Body(CircleShape(50), x, y, 1.0);
			// 	bodies.push_back(ball);
			// 	break;
			// }
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT && !leftMouseButtonDown)
			{
				float x = event.motion.x * Graphics::PixelDensity();
				float y = event.motion.y * Graphics::PixelDensity();
				// Body* smallBall = new Body(CircleShape(50), x * 2, y * 2, 1.0);
				// bodies.push_back(smallBall);

				std::vector<Vec2> vertices = {
					Vec2(20, 60), Vec2(-40, 20), Vec2(-20, -60), Vec2(20, -60), Vec2(40, 20)
				};
				Body* poly = new Body(PolygonShape(vertices), x, y, 2.0);
				poly->restitution = 0.1;
				poly->friction = 0.7;
				bodies.push_back(poly);
			}
			break;
		}
			// case SDL_EVENT_MOUSE_MOTION:
			// {
			// 	Vec2 mousePosition{ event.motion.x, event.motion.y };
			// 	mousePosition = mousePosition * Graphics::windowInfo.density;
			// 	bodies[1]->position = mousePosition;
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
