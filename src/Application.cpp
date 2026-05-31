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
	particle = new Particle(8, 8, 1);
	particle->radius = 8;
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
				break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update(float deltaTime)
{
	particle->acceleration = Vec2(2.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);
	particle->velocity += particle->acceleration * deltaTime;

	particle->position += particle->velocity * deltaTime;

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

void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);
	Graphics::DrawRect(1280 - 5, 720 - 5, 5, 5, 0xFFFFFFFF);
	Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
	Graphics::RenderFrame();
}

void Application::Destroy()
{
	delete particle;
	Graphics::CloseWindow();
}