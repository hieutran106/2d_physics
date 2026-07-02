#include "Application.h"
#include <SDL3/SDL.h>

const Uint64 FPS = 60;
const Uint64 MILLISECS_PER_FRAME = 1000 / FPS;

int main(int argc, char * args[])
{
	SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
	Application app;
	app.Setup();
	Uint64 millisecsPreviousFrame = SDL_GetTicks();
	while(app.running)
	{
		// If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
		Uint64 elapsed = SDL_GetTicks() - millisecsPreviousFrame;
		if(elapsed < MILLISECS_PER_FRAME)
		{
			SDL_Delay(static_cast<Uint32>(MILLISECS_PER_FRAME - elapsed));
		}
		// delta time
		Uint64 deltaTimeMs = SDL_GetTicks() - millisecsPreviousFrame;
		// Cap delta time
		if(deltaTimeMs > MILLISECS_PER_FRAME)
		{
			deltaTimeMs = MILLISECS_PER_FRAME;
		}

		double deltaTime = deltaTimeMs / 1000.0;
		app.ProcessInput();
		app.Update(deltaTime);
		app.Render();

		millisecsPreviousFrame = SDL_GetTicks();
	}

	app.Destroy();

	return 0;
}