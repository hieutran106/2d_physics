#pragma once

#include "Physics/Vec2.h"
#include "WindowInfo.h"
#include <SDL3/SDL.h>
#include <vector>

class Graphics
{

public:
	static SDL_Window * window;
	static SDL_Renderer * renderer;

	static WindowInfo windowInfo;
	static int Width();
	static int Height();
	static bool InitializeWindow(const char * title, int width, int height);
	static void CloseWindow();
	static void ClearScreen(Uint32 color);
	static void RenderFrame();
	static void DrawLine(int x0, int y0, int x1, int y1, Uint32 color);
	static void DrawCircle(int x, int y, int radius, float angle, Uint32 color);
	static void DrawFillCircle(int x, int y, int radius, Uint32 color);
	static void DrawRect(int x, int y, int width, int height, Uint32 color);
	static void DrawFillRect(int x, int y, int width, int height, Uint32 color);
	static void DrawPolygon(int x, int y, const std::vector<Vec2> & vertices, Uint32 color);
	static void DrawFillPolygon(int x, int y, const std::vector<Vec2> & vertices, Uint32 color);
	static void DrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture * texture);
};
