#include "Application.h"
#include "Graphics.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"

bool Application::IsRunning()
{
	return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup()
{
	running = Graphics::InitializeWindow("2d physics", 960, 720);

	Body * a = new Body(100, 100, 1.0);
	Body * b = new Body(300, 100, 1.0);
	Body * c = new Body(300, 300, 1.0);
	Body * d = new Body(100, 300, 1.0);

	a->radius = 6;
	b->radius = 6;
	c->radius = 6;
	d->radius = 6;

	bodies.push_back(a);
	bodies.push_back(b);
	bodies.push_back(c);
	bodies.push_back(d);
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update(float deltaTime)
{

	for(auto body : bodies)
	{
		body->AddForce(pushForce);

		// Apply drag force
		Vec2 drag = Force::GenerateDragForce(*body, 0.002);
		body->AddForce(drag);

		// // Apply weight force
		Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
		body->AddForce(weight);
	}

	// Connect
	for(int i = 0; i < bodies.size(); i++)
	{
		int curr = i;
		int next = (i + 1) % bodies.size();

		Vec2 springForce = Force::GenerateSpringForce(*bodies[curr], bodies[next]->position, restLength, k);
		bodies[curr]->AddForce(springForce);
		bodies[next]->AddForce(-springForce);
	}

	Vec2 springForce = Force::GenerateSpringForce(*bodies[0], bodies[2]->position, restLength, k);
	bodies[0]->AddForce(springForce);
	bodies[2]->AddForce(-springForce);

	springForce = Force::GenerateSpringForce(*bodies[1], bodies[3]->position, restLength, k);
	bodies[1]->AddForce(springForce);
	bodies[3]->AddForce(-springForce);

	for(auto particle : bodies)
	{
		particle->Integrate(deltaTime);
	}

	// Check the boundaries of the window
	for(auto body : bodies)
	{
		// Nasty hardcoded flip in velocity if it touches the limits of the screen window
		if(body->position.x - body->radius < 0)
		{
			body->position.x = body->radius;
			body->velocity.x *= -0.9;
		}
		else if(body->position.x + body->radius > Graphics::Width())
		{
			body->position.x = Graphics::Width() - body->radius;
			body->velocity.x *= -0.9;
		}
		if(body->position.y - body->radius < 0)
		{
			body->position.y = body->radius;
			body->velocity.y *= -0.9;
		}
		else if(body->position.y + body->radius >= Graphics::Height())
		{
			body->position.y = Graphics::Height() - body->radius;
			body->velocity.y *= -0.9;
		}
	}
}

void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);
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

	// Draw bob
	for(int i = 0; i < bodies.size(); i++)
	{
		Graphics::DrawFillCircle(bodies[i]->position.x, bodies[i]->position.y, bodies[0]->radius, 0xFFFFFFFF);
		// Draw spring between particles
		int next = (i + 1) % NUM_PARTICLES;

		Graphics::DrawLine(
			bodies[i]->position.x, bodies[i]->position.y, bodies[next]->position.x, bodies[next]->position.y, 0xFF313131
		);
	}

	Graphics::DrawLine(
		bodies[0]->position.x, bodies[0]->position.y, bodies[2]->position.x, bodies[2]->position.y, 0xFF313131
	);

	Graphics::DrawLine(
		bodies[1]->position.x, bodies[1]->position.y, bodies[3]->position.x, bodies[3]->position.y, 0xFF313131
	);

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
				running = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				if(event.key.key == SDLK_ESCAPE)
					running = false;
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
				mouseCursor.x = event.motion.x;
				mouseCursor.y = event.motion.y;
				break;
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