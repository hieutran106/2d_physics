#pragma once
#include "Physics/Particle.h"
#include <vector>

class Application
{
private:
	bool running = false;
	std::vector<Particle *> particles;
	Vec2 pushForce;

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
