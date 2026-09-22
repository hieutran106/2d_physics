#include "Physics/Log.h"
#include <SDL3/SDL.h>

void Physics::LogInfoImpl(std::string_view msg)
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.data());
}

void Physics::LogDebugImpl(std::string_view msg)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.data());
}

void Physics::LogErrorImpl(std::string_view msg)
{
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.data());
}
