#include "Application.h"

#include "Graphics.h"
#include <SDL3/SDL.h>

bool Application::IsRunning()
{
	return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup()
{
	running = Graphics::OpenWindow();

	particle = new Particle(50, 100, 4);
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input()
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
void Application::Update()
{
	particle->velocity = Vec2(2.0, 0.0);
	particle->position += particle->velocity;
}

void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);
	Graphics::DrawFillCircle(particle->position.x, particle->position.y, 4, 0xFFFFFFFF);
	Graphics::RenderFrame();
}

void Application::Destroy()
{
	delete particle;
	Graphics::CloseWindow();
}