#pragma once
#include "Physics/Particle.h"
#include <SDL3/SDL.h>
#include <vector>

class Application
{
private:
	bool running = false;
	std::vector<Particle *> particles;
	Vec2 pushForce;

	// Mouse for impulse
	Vec2 mouseCursor = Vec2(0, 0);
	bool leftMouseButtonDown = false;
	// For spring
	Vec2 anchor = Vec2(0, 0);
	float k = 100;
	float restLength = 400;

public:
	Application() = default;
	~Application() = default;
	bool IsRunning();
	void Setup();
	void ProcessInput();
	void Update(float deltaTime);
	void Render();
	void Destroy();
};
