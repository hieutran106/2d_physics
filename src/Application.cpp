#include "Application.h"
#include "Graphics.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup()
{
	mRunning = Graphics::InitializeWindow("2d physics", 960, 720);
	Body * bigBall = new Body(CircleShape(100), 100, 100, 1.0);
	Body * smallBall = new Body(CircleShape(50), 500, 100, 1.0);

	bodies.push_back(bigBall);
	bodies.push_back(smallBall);
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update(float deltaTime)
{
	Graphics::ClearScreen(0xFF056263);
	for(auto body : bodies)
	{
		// body->AddForce(pushForce);

		// Apply drag force
		// Vec2 drag = Force::GenerateDragForce(*body, 0.002);
		// body->AddForce(drag);

		// // Apply weight force
		// Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
		// body->AddForce(weight);
		//
		// Vec2 wind = Vec2(20.0 * PIXELS_PER_METER, 0);
		// body->AddForce(wind);

		// float torque = 400;
		// body->AddTorque(torque);
	}

	for(auto body : bodies)
	{
		body->Update(deltaTime);
	}

	for(const auto body : bodies)
	{
		body->isColliding = false;
	}
	// Check all the bodies for collision
	for(int i = 0; i < bodies.size() - 1; i++)
	{
		for(int j = i + 1; j < bodies.size(); j++)
		{
			Body * a = bodies[i];
			Body * b = bodies[j];
			Contact contact;
			bool isColliding = CollisionDetection::IsColliding(a, b, contact);
			if(isColliding)
			{
				Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
				Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
				Graphics::DrawLine(
					contact.start.x,
					contact.start.y,
					contact.start.x + contact.normal.x * 15,
					contact.start.y + contact.normal.y * 15,
					0xFFFF00FF
				);
				a->isColliding = true;
				b->isColliding = true;
			}
		}
	}

	// Check the boundaries of the window
	for(auto body : bodies)
	{
		if(body->shape->GetType() == CIRCLE)
		{
			CircleShape * circle = (CircleShape *)body->shape;
			float radius = circle->radius;
			// Nasty hardcoded flip in velocity if it touches the limits of the screen window
			if(body->position.x - radius < 0)
			{
				body->position.x = radius;
				body->velocity.x *= -0.9;
			}
			else if(body->position.x + radius > Graphics::Width())
			{
				body->position.x = Graphics::Width() - radius;
				body->velocity.x *= -0.9;
			}
			if(body->position.y - radius < 0)
			{
				body->position.y = radius;
				body->velocity.y *= -0.9;
			}
			else if(body->position.y + radius >= Graphics::Height())
			{
				body->position.y = Graphics::Height() - radius;
				body->velocity.y *= -0.9;
			}
		}
	}
}

void Application::Render()
{
	// Graphics::ClearScreen(0xFF056263);
	if(leftMouseButtonDown)
	{
		Graphics::DrawLine(
			bodies[bodies.size() - 1]->position.x,
			bodies[bodies.size() - 1]->position.y,
			mouseCursor.x,
			mouseCursor.y,
			0xFF0000FF
		);
	}

	// Draw all bodies
	for(Body * body : bodies)
	{
		uint32_t color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
		if(body->shape->GetType() == CIRCLE)
		{
			auto * circle = static_cast<CircleShape *>(body->shape);
			Graphics::DrawCircle(body->position.x, body->position.y, circle->radius, body->rotation, color);
		}
		if(body->shape->GetType() == BOX)
		{
			BoxShape * boxShape = static_cast<BoxShape *>(body->shape);
			Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFFFFFFFF);
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
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_EVENT_QUIT:
				mRunning = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				if(event.key.key == SDLK_ESCAPE)
					mRunning = false;
				if(event.key.key == SDLK_UP)
					pushForce.y = -50 * PIXELS_PER_METER;
				if(event.key.key == SDLK_RIGHT)
					pushForce.x = 50 * PIXELS_PER_METER;
				if(event.key.key == SDLK_DOWN)
					pushForce.y = 50 * PIXELS_PER_METER;
				if(event.key.key == SDLK_LEFT)
					pushForce.x = -50 * PIXELS_PER_METER;
				break;
			case SDL_EVENT_KEY_UP:
				if(event.key.key == SDLK_UP)
					pushForce.y = 0;
				if(event.key.key == SDLK_RIGHT)
					pushForce.x = 0;
				if(event.key.key == SDLK_DOWN)
					pushForce.y = 0;
				if(event.key.key == SDLK_LEFT)
					pushForce.x = 0;
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				if(event.button.button == SDL_BUTTON_LEFT && !leftMouseButtonDown)
				{
					leftMouseButtonDown = true;
					float x, y;
					SDL_GetMouseState(&x, &y);
					mouseCursor.x = x;
					mouseCursor.y = y;
				}
				break;
			case SDL_EVENT_MOUSE_MOTION:
			{
				// mouseCursor.x = event.motion.x;
				// mouseCursor.y = event.motion.y;
				Vec2 mousePosition{event.motion.x, event.motion.y};
				mousePosition = mousePosition * Graphics::windowInfo.density;
				bodies[0]->position = mousePosition;
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_UP:
				if(leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
				{
					leftMouseButtonDown = false;
					Body * last = bodies[bodies.size() - 1];
					Vec2 impulseDirection = (last->position - mouseCursor).UnitVector();
					float impulseMagnitude = (last->position - mouseCursor).Magnitude() * 5.0;
					last->velocity = impulseDirection * impulseMagnitude;
				}
				break;
		}
	}
}

void Application::Destroy()
{
	for(auto p : bodies)
	{
		delete p;
	}
	Graphics::CloseWindow();
}