#include "Application.h"
#include "Graphics.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"

bool Application::IsRunning()
{
	return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup()
{
	running = Graphics::InitializeWindow("2d physics", 960, 720);

	anchor = Vec2(Graphics::Width() / 2.0, 30);
	Particle * bob = new Particle(Graphics::Width() / 2.0, Graphics::Height() / 2.0, 2.0);
	bob->radius = 10.0;
	particles.push_back(bob);
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update(float deltaTime)
{

	for(auto particle : particles)
	{
		particle->AddForce(pushForce);

		// Apply drag force
		Vec2 drag = Force::GenerateDragForce(*particle, 0.01);
		particle->AddForce(drag);

		// // Apply weight force
		Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
		particle->AddForce(weight);
	}

	// Apply spring force to the particle connected to the anchor
	Vec2 springForce = Force::GenerateSpringForce(*particles[0], anchor, restLength, k);
	particles[0]->AddForce(springForce);

	for(auto particle : particles)
	{
		particle->Integrate(deltaTime);
	}

	// Check the boundaries of the window
	for(auto particle : particles)
	{
		// Nasty hardcoded flip in velocity if it touches the limits of the screen window
		if(particle->position.x - particle->radius < 0)
		{
			particle->position.x = particle->radius;
			particle->velocity.x *= -0.9;
		}
		else if(particle->position.x + particle->radius > Graphics::Width())
		{
			particle->position.x = Graphics::Width() - particle->radius;
			particle->velocity.x *= -0.9;
		}
		if(particle->position.y - particle->radius < 0)
		{
			particle->position.y = particle->radius;
			particle->velocity.y *= -0.9;
		}
		else if(particle->position.y + particle->radius >= Graphics::Height())
		{
			particle->position.y = Graphics::Height() - particle->radius;
			particle->velocity.y *= -0.9;
		}
	}
}

void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);
	if(leftMouseButtonDown)
	{
		Graphics::DrawLine(
			particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF
		);
	}
	// Draw spring
	Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);

	// Draw anchor
	Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
	// Draw bob
	Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFFFFFFF);

	Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::ProcessInput()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_EVENT_QUIT:
				running = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				if(event.key.key == SDLK_ESCAPE)
					running = false;
				if(event.key.key == SDLK_UP)
					pushForce.y = -50 * PIXELS_PER_METER;
				if(event.key.key == SDLK_RIGHT)
					pushForce.x = 50 * PIXELS_PER_METER;
				if(event.key.key == SDLK_DOWN)
					pushForce.y = 50 * PIXELS_PER_METER;
				if(event.key.key == SDLK_LEFT)
					pushForce.x = -50 * PIXELS_PER_METER;
				break;
			case SDL_EVENT_KEY_UP:
				if(event.key.key == SDLK_UP)
					pushForce.y = 0;
				if(event.key.key == SDLK_RIGHT)
					pushForce.x = 0;
				if(event.key.key == SDLK_DOWN)
					pushForce.y = 0;
				if(event.key.key == SDLK_LEFT)
					pushForce.x = 0;
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				if(event.button.button == SDL_BUTTON_LEFT && !leftMouseButtonDown)
				{
					leftMouseButtonDown = true;
					float x, y;
					SDL_GetMouseState(&x, &y);
					mouseCursor.x = x;
					mouseCursor.y = y;
				}
				break;
			case SDL_EVENT_MOUSE_MOTION:
				mouseCursor.x = event.motion.x;
				mouseCursor.y = event.motion.y;
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				if(leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
				{
					leftMouseButtonDown = false;
					Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
					float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
					particles[0]->velocity = impulseDirection * impulseMagnitude;
				}
				break;
		}
	}
}

void Application::Destroy()
{
	for(auto p : particles)
	{
		delete p;
	}
	Graphics::CloseWindow();
}