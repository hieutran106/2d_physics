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
	Particle * smallBall = new Particle(50, 100, 1.0);
	smallBall->radius = 4;
	particles.push_back(smallBall);

	// Particle * bigBall = new Particle(200, 100, 3.0);
	// bigBall->radius = 12;
	// particles.push_back(bigBall);
	liquid.x = 0;
	liquid.y = Graphics::Height() / 2;
	liquid.w = Graphics::Width();
	liquid.h = Graphics::Height() / 2;
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

					// float x, y;
					// SDL_GetMouseState(&x, &y);
					// Particle * particle = new Particle(x, y, 1.0);
					// particle->radius = 5;
					// particles.push_back(particle);
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
					Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
					float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
					particles[0]->velocity = impulseDirection * impulseMagnitude;
				}
				break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update(float deltaTime)
{

	for(auto particle : particles)
	{
		// Apply a "weight" force to my particles
		// Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
		// particle->AddForce(weight);
		// Apply a "push" force to my particles
		particle->AddForce(pushForce);

		Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
		particle->AddForce(friction);
		// Apply a drag force if we are inside the liquid
		// if(particle->position.y > liquid.y)
		// {
		// 	Vec2 drag = Force::GenerateDragForce(*particle, 0.03);
		// 	particle->AddForce(drag);
		// }
	}

	for(auto particle : particles)
	{
		particle->Integrate(deltaTime);
	}

	for(auto particle : particles)
	{
		if(particle->position.x - particle->radius < 0)
		{
			particle->position.x = particle->radius;
			particle->velocity.x *= -0.9;
		}
		else if(particle->position.x + particle->radius > Graphics::Width())
		{
			particle->position.x = Graphics::Width() - particle->radius;
			particle->velocity.x *= -0.9;
		}
		if(particle->position.y - particle->radius < 0)
		{
			particle->position.y = particle->radius;
			particle->velocity.y *= -0.9;
		}
		else if(particle->position.y + particle->radius >= Graphics::Height())
		{
			particle->position.y = Graphics::Height() - particle->radius;
			particle->velocity.y *= -0.9;
		}
	}
}

void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);
	// Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF6E3713);
	if(leftMouseButtonDown)
	{
		Graphics::DrawLine(
			particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF
		);
	}
	for(auto particle : particles)
	{
		Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
	}

	Graphics::RenderFrame();
}

void Application::Destroy()
{
	for(auto p : particles)
	{
		delete p;
	}
	Graphics::CloseWindow();
}