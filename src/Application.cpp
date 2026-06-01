#include "Application.h"
#include "Graphics.h"
#include "Physics/Constants.h"

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
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update(float deltaTime)
{
	// Apply a "wind" force to my particles
	for(auto particle : particles)
	{
		Vec2 wind = Vec2(0.2 * PIXELS_PER_METER, 0.0);
		particle->AddForce(wind);
	}

	// Apply a "weight" force to my particles
	for(auto particle : particles)
	{
		Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
		particle->AddForce(weight);
	}
	// Apply a "push" force to my particles
	for(auto particle : particles)
	{
		particle->AddForce(pushForce);
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
	Graphics::DrawRect(1280 - 5, 720 - 5, 5, 5, 0xFFFFFFFF);
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