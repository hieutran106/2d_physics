#pragma once

struct Display
{
	// Window size
	int windowWidth = 0;
	int windowHeight = 0;

	// Support HiDPI Retina
	int fbWidth = 0;
	int fbHeight = 0;

	int renderWidth = 0;
	int renderHeight = 0;

	float density = 0;
	float scale = 0;
};