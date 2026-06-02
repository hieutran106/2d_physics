#pragma once
#include "Particle.h"
#include "Vec2.h"

namespace Force
{
	inline Vec2 GenerateDragForce(const Particle & particle, float k)
	{
		Vec2 dragForce = Vec2(0, 0);
		if(particle.velocity.MagnitudeSquared() > 0)
		{
			// Calculate the drag direciont (inverse of velocity unit vector)
			Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;
			float dragMagnitute = k * particle.velocity.MagnitudeSquared();
			dragForce = dragDirection * dragMagnitute;
		}
		return dragForce;
	}

	inline Vec2 GenerateFrictionForce(const Particle & particle, float k)
	{
		Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;
		return frictionDirection * k;
	}
}