#pragma once
#include "Physics/Contact.h"
#include <SDL3/SDL.h>

namespace GraphicHelper
{
constexpr Uint32 ColorGreen = 0xFF00FF00; // green
constexpr Uint32 ColorMagenta = 0xFFFF00FF;
constexpr Uint32 ColorRed = 0xFFFF0000;
constexpr Uint32 ColorCyan = 0xFF00FFFF;
constexpr Uint32 ColorYellow = 0xFFFFFF00;

void DrawDebugCollision(const Contact& contact);
void DrawFillCircle(const Vec2& pos, int radius, Uint32 color);
void DrawCircle(const Vec2& pos, int radius, float angle, Uint32 color);
void DrawLine(const Vec2& p1, const Vec2& p2, Uint32 color);
} // namespace GraphicHelper
