#pragma once
#include "Physics/Particle.h"

class Application
{
private:
	bool running = false;
	Particle * particle;

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
