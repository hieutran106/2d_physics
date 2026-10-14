#include "GraphicHelper.h"

#include "Graphics.h"

void GraphicHelper::DrawDebugCollision(const Contact& contact)
{
	DrawFillCircle(contact.start, 10, ColorYellow);
	DrawFillCircle(contact.end, 10, ColorYellow);
	// Line from start
	Vec2 lineEnd(contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15);
	DrawLine(contact.start, contact.end, ColorRed);

	// Line between center
	// Graphics::DrawLine(contact.a->position.x, contact.a->position.y, contact.b->position.x, contact.b->position.y,
	// ColorMagenta);
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
