#include "Driver/Platform.hpp"

#include <SDL.h>

#include <iostream>

namespace SuperHaxagon {
	SDL_Window* getWindow(const Platform& platform) {
		constexpr int dsWidth = 400;
		constexpr int dsHeight = 240;
		int displayWidth = 1280;
		int displayHeight = 720;
		int scale = 1;

		SDL_DisplayMode mode;
		if (!SDL_GetCurrentDisplayMode(0, &mode)) {
			auto scaleW = (mode.w - 1) / dsWidth;
			auto scaleH = (mode.h - 1) / dsHeight;
			scale = std::max(1, std::min(scaleW, scaleH));
		} else {
			platform.message(Dbg::WARN, "window", "failed to get desktop resolution");
		}

		displayWidth = dsWidth * scale;
		displayHeight = dsHeight * scale;

		return SDL_CreateWindow("Super Haxagon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayWidth, displayHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	}

	void Platform::message(const Dbg dbg, const std::string& where, const std::string& message) const {
		if (dbg == Dbg::INFO) {
			std::cout << "[linuxSDL2:info] " + where + ": " + message << std::endl;
		} else if (dbg == Dbg::WARN) {
			std::cout << "[linuxSDL2:warn] " + where + ": " + message << std::endl;
		} else if (dbg == Dbg::FATAL) {
			std::cerr << "[linuxSDL2:fatal] " + where + ": " + message << std::endl;
		}
	}

	Supports Platform::supports() {
		return Supports::SHADOWS;
	}
}
