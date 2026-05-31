#pragma once
#include "Display.h"
#include <SDL3/SDL.h>

namespace SDLHelper
{
	inline Display ConfigureSDLDisplaySize(SDL_Window * window, SDL_Renderer * renderer, int width, int height)
	{
		Display display;
		// Logical size
		display.windowWidth = width;
		display.windowHeight = height;
		SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_STRETCH);

		SDL_GetWindowSize(window, &display.nativeWindowWidth, &display.nativeWindowHeight);
		display.density = SDL_GetWindowPixelDensity(window);
		display.scale = SDL_GetWindowDisplayScale(window);

		SDL_LogDebug(
			SDL_LOG_CATEGORY_APPLICATION,
			"SDL Logical window size: %d x %d, Native window size: %d x %d, pixel density: %.2f, scale=%.2f",
			display.windowWidth,
			display.windowHeight,
			display.nativeWindowWidth,
			display.nativeWindowHeight,
			display.density,
			display.scale
		);

		SDL_GetWindowSizeInPixels(window, &display.fbWidth, &display.fbHeight);
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Physical pixels: %d x %d", display.fbWidth, display.fbHeight);
		return display;
	}

	inline bool IsIOS()
	{
		const char * platform = SDL_GetPlatform();
		return SDL_strcmp(platform, "iOS") == 0;
	}

	inline void LogSDLWindowFlags(SDL_Window * window)
	{
		Uint64 flags = SDL_GetWindowFlags(window);
		std::string report = "Effective Window Flags for ID " + std::to_string(SDL_GetWindowID(window)) + ":\n";
		struct FlagInfo
		{
			Uint64 bit;
			std::string name;
		};

		std::vector<FlagInfo> checkList = {
			{SDL_WINDOW_FULLSCREEN,          "FULLSCREEN"           },
			{SDL_WINDOW_OPENGL,              "OPENGL"               },
			{SDL_WINDOW_OCCLUDED,            "OCCLUDED"             },
			{SDL_WINDOW_HIDDEN,              "HIDDEN"               },
			{SDL_WINDOW_BORDERLESS,          "BORDERLESS"           },
			{SDL_WINDOW_RESIZABLE,           "RESIZABLE"            },
			{SDL_WINDOW_MINIMIZED,           "MINIMIZED"            },
			{SDL_WINDOW_MAXIMIZED,           "MAXIMIZED"            },
			{SDL_WINDOW_MOUSE_GRABBED,       "MOUSE_GRABBED"        },
			{SDL_WINDOW_INPUT_FOCUS,         "INPUT_FOCUS"          },
			{SDL_WINDOW_MOUSE_FOCUS,         "MOUSE_FOCUS"          },
			{SDL_WINDOW_HIGH_PIXEL_DENSITY,  "HIGH_PIXEL_DENSITY"   },
			{SDL_WINDOW_MOUSE_RELATIVE_MODE, "MOUSE_RELATIVE_MODE"  },
			{SDL_WINDOW_VULKAN,              "VULKAN"               },
			{SDL_WINDOW_METAL,               "METAL"                },
			{SDL_WINDOW_BORDERLESS,          "SDL_WINDOW_BORDERLESS"}
		};
		for(const auto & item : checkList)
		{
			if(flags & item.bit)
			{
				report += "  [X] " + item.name + "\n";
			}
		}
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", report.c_str());
	}
}
