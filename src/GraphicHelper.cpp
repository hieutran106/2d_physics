#include "GraphicHelper.h"

#include "Graphics.h"

void GraphicHelper::DrawDebugCollision(const Contact& contact)
{
	Graphics::DrawThickArrow(contact.start.x, contact.start.y, contact.end.x, contact.end.y, 5, ColorRed);
	DrawFillCircle(contact.start, 5, ColorGreen);
	DrawFillCircle(contact.end, 5, ColorGreen);
}

void GraphicHelper::DrawFillCircle(const Vec2& pos, int radius, Uint32 color)
{
	Graphics::DrawFillCircle(pos.x, pos.y, radius, color);
}
void GraphicHelper::DrawCircle(const Vec2& pos, int radius, float angle, Uint32 color)
{
	Graphics::DrawCircle(pos.x, pos.y, radius, angle, color);
}
void GraphicHelper::DrawLine(const Vec2& p1, const Vec2& p2, Uint32 color)
{
	Graphics::DrawLine(p1.x, p1.y, p2.x, p2.y, color);
}
