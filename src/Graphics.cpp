#include "Graphics.h"

#include "SDL3_gfxPrimitives.h"
#include "SDLHelper.h"
#include <iostream>

// Convert SDL_Renderer Color (ARGB) to SDL3_gfx (ABGR)
static Uint32 ToGfxColor(Uint32 argb)
{
	return (argb & 0xFF00FF00) | ((argb & 0x00FF0000) >> 16) | ((argb & 0x000000FF) << 16);
}

SDL_Window*   Graphics::window = nullptr;
SDL_Renderer* Graphics::renderer = nullptr;
WindowInfo    Graphics::windowInfo;

int Graphics::Width()
{
	return windowInfo.fbWidth;
}

int Graphics::Height()
{
	return windowInfo.fbHeight;
}
float Graphics::PixelDensity()
{
	return windowInfo.density;
}

bool Graphics::InitializeWindow(const char* title, int aspectW, int aspectH)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Fail to init SDL");
		return false;
	}
	SDL_WindowFlags windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_METAL;
	if (SDLHelper::IsIOS())
	{
		windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		// Desktop environment, create hidden first; we don't yet know which display we'll land on.
		windowFlags |= SDL_WINDOW_HIDDEN;
	}

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d12,metal");

	// Create at a placeholder size; real size decided after we know the display
	window = SDL_CreateWindow(title, 100, 100, windowFlags);

	if (!window)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating SDL Window");
		return false;
	}

	if (!SDLHelper::IsIOS())
	{
		// Desktop environment
		SDL_DisplayID          display = SDL_GetDisplayForWindow(window);
		const char*            name = SDL_GetDisplayName(display);
		const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
		SDL_Log("Window opened on display %u: %s, resolution: %d x %d", display, name, mode->w, mode->h);
		int windowHeight = 4 * mode->h / 5;
		int windowWidth = windowHeight * aspectW / aspectH; // Keep aspect ratio
		SDL_SetWindowSize(window, windowWidth, windowHeight);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(display), SDL_WINDOWPOS_CENTERED_DISPLAY(display));
		SDL_ShowWindow(window);
	}

	SDLHelper::LogSDLWindowFlags(window);

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating SDL Renderer: %s", SDL_GetError());
		return false;
	}
	SDL_PropertiesID props = SDL_GetRendererProperties(renderer);
	SDL_PixelFormat* format = static_cast<SDL_PixelFormat*>(
		SDL_GetPointerProperty(props, SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER, nullptr));
	if (format)
	{
		SDL_Log("SDL_Renderer PixelFormat: %s", SDL_GetPixelFormatName(*format));
	}
	windowInfo = SDLHelper::ConfigureSDLWindowInfo(window);
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
	lineColor(renderer, x0, y0, x1, y1, ToGfxColor(color));
}

void Graphics::DrawCircle(int x, int y, int radius, float angle, Uint32 color)
{
	circleColor(renderer, x, y, radius, ToGfxColor(color));
	lineColor(renderer, x, y, x + cos(angle) * radius, y + sin(angle) * radius, ToGfxColor(color));
}

void Graphics::DrawFillCircle(int x, int y, int radius, Uint32 color)
{
	filledCircleColor(renderer, x, y, radius, ToGfxColor(color));
}

void Graphics::DrawRect(int x, int y, int width, int height, Uint32 color)
{
	Uint32 gfxColor = ToGfxColor(color);
	lineColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y - height / 2.0, gfxColor);
	lineColor(renderer, x + width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, gfxColor);
	lineColor(renderer, x + width / 2.0, y + height / 2.0, x - width / 2.0, y + height / 2.0, gfxColor);
	lineColor(renderer, x - width / 2.0, y + height / 2.0, x - width / 2.0, y - height / 2.0, gfxColor);
}

void Graphics::DrawFillRect(int x, int y, int width, int height, Uint32 color)
{
	boxColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, ToGfxColor(color));
}

void Graphics::DrawPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color)
{
	Uint32 gfxColor = ToGfxColor(color);
	for (int i = 0; i < vertices.size(); i++)
	{
		int currIndex = i;
		int nextIndex = (i + 1) % vertices.size();
		lineColor(
			renderer,
			vertices[currIndex].x,
			vertices[currIndex].y,
			vertices[nextIndex].x,
			vertices[nextIndex].y,
			gfxColor);
	}
	filledCircleColor(renderer, x, y, 1, gfxColor);
}

void Graphics::DrawFillPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color)
{
	std::vector<float> vx;
	std::vector<float> vy;
	for (int i = 0; i < vertices.size(); i++)
	{
		vx.push_back(static_cast<int>(vertices[i].x));
	}
	for (int i = 0; i < vertices.size(); i++)
	{
		vy.push_back(static_cast<int>(vertices[i].y));
	}
	filledPolygonColor(renderer, &vx[0], &vy[0], vertices.size(), ToGfxColor(color));
	filledCircleColor(renderer, x, y, 1, ToGfxColor(0xFF000000));
}

void Graphics::DrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture* texture)
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
