#pragma once
#include "Physics/Body.h"
#include "Physics/Contact.h"
#include "Physics/World.h"

#include <SDL3/SDL.h>
#include <vector>

class Application
{
	bool   debug = false;
	bool   mRunning = false;
	World* world;
	// std::vector<Body*> bodies;
	// Vec2               pushForce;

	// Mouse for impulse
	Vec2 mouseCursor = Vec2(0, 0);
	bool leftMouseButtonDown = false;

	// For soft body
	float     k = 1500;
	float     restLength = 200;
	const int NUM_PARTICLES = 4;

public:
	Application() = default;
	~Application() = default;

	// Public read-only access point
	const bool& running = mRunning;

	void Setup();
	void ProcessInput();
	void Update(float deltaTime);
	void Render();
	void Destroy();
};
