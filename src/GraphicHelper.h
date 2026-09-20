#pragma once
#include "Physics/Contact.h"
#include <SDL3/SDL.h>

namespace GraphicHelper
{

constexpr Uint32 ColorSetARGB(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
	return (static_cast<Uint32>(alpha) << 24) | (static_cast<Uint32>(red) << 16) | (static_cast<Uint32>(green) << 8) | (static_cast<Uint32>(blue));
}

constexpr Uint32 ColorSetRGB(uint8_t red, uint8_t green, uint8_t blue)
{
	return ColorSetARGB(255, red, green, blue);
}

constexpr Uint32 ColorRed = ColorSetRGB(255, 0, 0);
constexpr Uint32 ColorGreen = ColorSetRGB(0, 255, 0);
constexpr Uint32 ColorBlue = ColorSetRGB(0, 0, 255);
constexpr Uint32 ColorWhite = ColorSetRGB(255, 255, 255);

constexpr Uint32 ColorMagenta = ColorSetRGB(255, 0, 255);
constexpr Uint32 ColorCyan = ColorSetRGB(0, 255, 255);
constexpr Uint32 ColorYellow = ColorSetRGB(255, 255, 0);

void DrawDebugCollision(const Contact& contact);
void DrawFillCircle(const Vec2& pos, int radius, Uint32 color);
void DrawCircle(const Vec2& pos, int radius, float angle, Uint32 color);
void DrawLine(const Vec2& p1, const Vec2& p2, Uint32 color);
} // namespace GraphicHelper
