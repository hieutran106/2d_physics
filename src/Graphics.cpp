#include "Graphics.h"

#include "SDL3_gfxPrimitives.h"
#include "SDLHelper.h"
#include <iostream>

SDL_Window * Graphics::window = nullptr;
SDL_Renderer * Graphics::renderer = nullptr;
Display Graphics::display;

int Graphics::Width()
{
	return display.windowWidth;
}

int Graphics::Height()
{
	return display.windowHeight;
}

bool Graphics::InitializeWindow(const char * title, int width, int height)
{
	if(!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Fail to init SDL");
		return false;
	}
	SDL_WindowFlags windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
	int nativeWindowWidth = 0;
	int nativeWindowHeight = 0;
	if(SDLHelper::IsIOS())
	{
		windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
		const SDL_DisplayMode * mode = SDL_GetCurrentDisplayMode(primaryDisplay);
		SDL_Log("Screen resolution: %d x %d", mode->w, mode->h);
		nativeWindowHeight = 4 * mode->h / 5;
		// keep aspect ratio
		nativeWindowWidth = nativeWindowHeight * width / height;
	}

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d12,metal");

	window = SDL_CreateWindow(title, nativeWindowWidth, nativeWindowHeight, windowFlags);

	if(!window)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating SDL Window");
		return false;
	}
	SDLHelper::LogSDLWindowFlags(window);

	renderer = SDL_CreateRenderer(window, nullptr);
	if(!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating SDL Renderer");
		return false;
	}
	display = SDLHelper::ConfigureSDLDisplaySize(window);
	return true;
}

void Graphics::ClearScreen(Uint32 color)
{
	SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
	SDL_RenderClear(renderer);
}

void Graphics::RenderFrame()
{
	SDL_RenderPresent(renderer);
}

void Graphics::DrawLine(int x0, int y0, int x1, int y1, Uint32 color)
{
	lineColor(renderer, x0, y0, x1, y1, color);
}

void Graphics::DrawCircle(int x, int y, int radius, float angle, Uint32 color)
{
	circleColor(renderer, x, y, radius, color);
	lineColor(renderer, x, y, x + cos(angle) * radius, y + sin(angle) * radius, color);
}

void Graphics::DrawFillCircle(int x, int y, int radius, Uint32 color)
{
	filledCircleColor(renderer, x, y, radius, color);
}

void Graphics::DrawRect(int x, int y, int width, int height, Uint32 color)
{
	lineColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y - height / 2.0, color);
	lineColor(renderer, x + width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
	lineColor(renderer, x + width / 2.0, y + height / 2.0, x - width / 2.0, y + height / 2.0, color);
	lineColor(renderer, x - width / 2.0, y + height / 2.0, x - width / 2.0, y - height / 2.0, color);
}

void Graphics::DrawFillRect(int x, int y, int width, int height, Uint32 color)
{
	boxColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
}

void Graphics::DrawPolygon(int x, int y, const std::vector<Vec2> & vertices, Uint32 color)
{
	for(int i = 0; i < vertices.size(); i++)
	{
		int currIndex = i;
		int nextIndex = (i + 1) % vertices.size();
		lineColor(
			renderer, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color
		);
	}
	filledCircleColor(renderer, x, y, 1, color);
}

void Graphics::DrawFillPolygon(int x, int y, const std::vector<Vec2> & vertices, Uint32 color)
{
	std::vector<float> vx;
	std::vector<float> vy;
	for(int i = 0; i < vertices.size(); i++)
	{
		vx.push_back(static_cast<int>(vertices[i].x));
	}
	for(int i = 0; i < vertices.size(); i++)
	{
		vy.push_back(static_cast<int>(vertices[i].y));
	}
	filledPolygonColor(renderer, &vx[0], &vy[0], vertices.size(), color);
	filledCircleColor(renderer, x, y, 1, 0xFF000000);
}

void Graphics::DrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture * texture)
{
	SDL_FRect dstRect = {
		x - (width / 2.0f), y - (height / 2.0f), static_cast<float>(width), static_cast<float>(height)
	};
	float rotationDeg = rotation * 57.2958;
	SDL_RenderTextureRotated(renderer, texture, nullptr, &dstRect, rotationDeg, nullptr, SDL_FLIP_NONE);
}

void Graphics::CloseWindow()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}