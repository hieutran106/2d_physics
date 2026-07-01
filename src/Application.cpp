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

	Particle * a = new Particle(100, 100, 1.0);
	Particle * b = new Particle(300, 100, 1.0);
	Particle * c = new Particle(300, 300, 1.0);
	Particle * d = new Particle(100, 300, 1.0);

	a->radius = 6;
	b->radius = 6;
	c->radius = 6;
	d->radius = 6;

	particles.push_back(a);
	particles.push_back(b);
	particles.push_back(c);
	particles.push_back(d);
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
		Vec2 drag = Force::GenerateDragForce(*particle, 0.002);
		particle->AddForce(drag);

		// // Apply weight force
		Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
		particle->AddForce(weight);
	}

	// Connect
	for(int i = 0; i < particles.size(); i++)
	{
		int curr = i;
		int next = (i + 1) % particles.size();

		Vec2 springForce = Force::GenerateSpringForce(*particles[curr], particles[next]->position, restLength, k);
		particles[curr]->AddForce(springForce);
		particles[next]->AddForce(-springForce);
	}

	Vec2 springForce = Force::GenerateSpringForce(*particles[0], particles[2]->position, restLength, k);
	particles[0]->AddForce(springForce);
	particles[2]->AddForce(-springForce);

	springForce = Force::GenerateSpringForce(*particles[1], particles[3]->position, restLength, k);
	particles[1]->AddForce(springForce);
	particles[3]->AddForce(-springForce);

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
			particles[particles.size() - 1]->position.x,
			particles[particles.size() - 1]->position.y,
			mouseCursor.x,
			mouseCursor.y,
			0xFF0000FF
		);
	}

	// Draw bob
	for(int i = 0; i < particles.size(); i++)
	{
		Graphics::DrawFillCircle(particles[i]->position.x, particles[i]->position.y, particles[0]->radius, 0xFFFFFFFF);
		// Draw spring between particles
		int next = (i + 1) % NUM_PARTICLES;

		Graphics::DrawLine(
			particles[i]->position.x,
			particles[i]->position.y,
			particles[next]->position.x,
			particles[next]->position.y,
			0xFF313131
		);
	}

	Graphics::DrawLine(
		particles[0]->position.x,
		particles[0]->position.y,
		particles[2]->position.x,
		particles[2]->position.y,
		0xFF313131
	);

	Graphics::DrawLine(
		particles[1]->position.x,
		particles[1]->position.y,
		particles[3]->position.x,
		particles[3]->position.y,
		0xFF313131
	);

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
					Particle * last = particles[particles.size() - 1];
					Vec2 impulseDirection = (last->position - mouseCursor).UnitVector();
					float impulseMagnitude = (last->position - mouseCursor).Magnitude() * 5.0;
					last->velocity = impulseDirection * impulseMagnitude;
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