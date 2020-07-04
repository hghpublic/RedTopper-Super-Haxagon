#include "Factories/Level.hpp"
#include "States/Quit.hpp"
#include "States/Play.hpp"
#include "States/Load.hpp"
#include "States/Menu.hpp"

#include "Game.hpp"
#include "Structs.hpp"

namespace SuperHaxagon {
	Menu::Menu(Game& game, bool showLoadLevels) :
		game(game),
		platform(game.getPlatform()),
		showLoadLevels(showLoadLevels)
	{}

	Menu::~Menu() = default;

	void Menu::enter() {
		platform.stopBGM();
		platform.playBGM(game.getBgmMenu());
		platform.playSFX(game.getSfxHexagon());
	}

	void Menu::exit() {
		platform.stopBGM();
	}

	std::unique_ptr<State> Menu::update() {
		platform.pollButtons();
		Buttons press = platform.getDown();

		if (press.quit) return std::make_unique<Quit>();

		if(!transitionDirection) {
			if (press.back && showLoadLevels) return std::make_unique<Load>(game);
			if (press.select) return std::make_unique<Play>(game, *game.getLevels()[level]);
			if (press.right) {
				transitionDirection = 1;
				lastLevel = level;
				level++;
				platform.playSFX(game.getSfxSelect());
			}
			if (press.left) {
				transitionDirection = -1;
				lastLevel = level;
				level--;
				platform.playSFX(game.getSfxSelect());
			}
		}

		if(level >=  game.getLevels().size()) level = 0;
		if(level < 0) level = game.getLevels().size() - 1;
		if(transitionDirection) transitionFrame++;
		if(transitionFrame >= FRAMES_PER_TRANSITION) {
			transitionFrame = 0;
			transitionDirection = 0;
		}

		return nullptr;
	}

	void Menu::draw() {
		platform.setScreen(Screen::TOP);

		double percentRotated = (double)(transitionFrame) / (double)FRAMES_PER_TRANSITION;
		double rotation = percentRotated * TAU/6.0;

		// If the user is going to the left, flip the radians so the animation plays backwards.
		if(transitionDirection == -1) {
			rotation *= -1.0;
		}

		// Colors
		Color FG{};
		Color BG1{};
		Color BG2{};
		Color BG3{};

		auto& levelPrev = *game.getLevels()[lastLevel];
		auto& levelCur = *game.getLevels()[level];

		if(transitionDirection) {
			FG = interpolateColor(levelPrev.getColorsFG()[0], levelCur.getColorsFG()[0], percentRotated);
			BG1 = interpolateColor(levelPrev.getColorsBG1()[0], levelCur.getColorsBG2()[0], percentRotated);
			BG2 = interpolateColor(levelPrev.getColorsBG2()[0], levelCur.getColorsBG1()[0], percentRotated);
			BG3 = interpolateColor(levelPrev.getColorsBG2()[0], levelCur.getColorsBG2()[0], percentRotated); //Real BG2 transition
		} else {
			FG = levelCur.getColorsFG()[0];
			BG1 = levelCur.getColorsBG1()[0];
			BG2 = levelCur.getColorsBG2()[0];
			BG3 = levelCur.getColorsBG2()[0]; //same as BG2
		}

		Point screen = platform.getScreenDim();
		Point shadow = game.getShadowOffset();

		Point focus = {screen.x/2, screen.y/4};
		Point offsetFocus = {focus.x + shadow.x, focus.y + shadow.y};

		// Home screen always has 6 sides.
		game.drawBackground(BG1, BG2, focus, game.getRenderDistance(), rotation, 6.0);

		// Shadows
		game.drawRegular(COLOR_SHADOW, offsetFocus, game.getHexLength(), rotation, 6.0);
		game.drawHumanCursor(COLOR_SHADOW, offsetFocus, TAU/4.0, 0);

		// Geometry
		game.drawRegular(FG, focus, game.getHexLength(), rotation, 6.0);
		game.drawRegular(BG3, focus, game.getHexLength() - game.getHexLengthBorder(), rotation, 6.0);
		game.drawHumanCursor(FG, focus, TAU/4.0, 0); //Draw cursor fixed quarter circle, no movement.

		// Top rectangle and triangle
		int TRIANGLE_WIDTH = 70;
		int GAP_FROM_RIGHT_SIDE = 20;

		// Text positions
		Point posTitle = {4, 2};
		Point posDifficulty = {4, posTitle.y + 32 + 1};
		Point posMode = {4, posDifficulty.y + 16 + 1};
		Point posCreator = {4, posMode.y + 16 + 1};
		Point posTime = {4, screen.y - 18};

		Point infoPos = {0, 0};
		Point infoSize = {screen.x - TRIANGLE_WIDTH - GAP_FROM_RIGHT_SIDE, posCreator.y + 16 + 3};
		std::array<Point, 3> infoTriangle = {
			Point{infoSize.x, platform.getScreenDim().y - 1 - infoSize.y},
			Point{infoSize.x, platform.getScreenDim().y - 1},
			Point{infoSize.x + TRIANGLE_WIDTH,  platform.getScreenDim().y - 1}
		};

		platform.drawRect(COLOR_TRANSPARENT, infoPos, infoSize);
		platform.drawTriangle(COLOR_TRANSPARENT, infoTriangle);

		// Score block with triangle
		Point timePos = {0, posTime.y - 3};
		Point timeSize = {10/*chars?*/ * 16 + 4, 16 + 7};
		std::array<Point, 3> timeTriangle = {
			Point{timeSize.x, timeSize.y - 3},
			Point{timeSize.x, - 1},    //why does this have to be -1?
			Point{timeSize.x + 18, -1} //I mean, it works...
		};

		platform.drawRect(COLOR_TRANSPARENT, timePos, timeSize);
		platform.drawTriangle(COLOR_TRANSPARENT, timeTriangle);

		// Actual text
		auto scoreTime  = getBestTime(levelCur.getHighScore());
		writeFont(COLOR_WHITE, posTitle, levelCur.getName(), FONT32, ALIGN_LEFT_C);
		writeFont(COLOR_GREY, posDifficulty, levelCur.getDifficulty(), FONT16, ALIGN_LEFT_C);
		writeFont(COLOR_GREY, posMode, levelCur.getMode(), FONT16, ALIGN_LEFT_C);
		writeFont(COLOR_GREY, posCreator, levelCur.getCreator(), FONT16, ALIGN_LEFT_C);
		writeFont(COLOR_WHITE, posTime, scoreTime, FONT16, ALIGN_LEFT_C);

		// Don't want to clear the screen if there is no bottom screen.
		if (platform.hasScreen(Screen::BOTTOM)) {
			platform.setScreen(Screen::BOTTOM);
			Point position = {0,0};
			Point size = platform.getScreenDim();
			platform.drawRect(COLOR_WHITE, position, size);
		}


		Point posButton = {4, 4};
		Point posLocation = {210, 4};
		Point posLevels = {4, posLocation.y + 16 + 1};
		if(showLoadLevels){
			writeFont(WHITE, posButton, "PRESS B TO LOAD", FONT16, ALIGN_LEFT_C);
			if(loaded == ROMFS) writeFont(WHITE, posLocation, "EXTERNAL", FONT16, ALIGN_LEFT_C);
			if(loaded == SDMC) writeFont(WHITE, posLocation, "INTERNAL", FONT16, ALIGN_LEFT_C);
			writeFont(WHITE, posLevels, "LEVELS", FONT16, ALIGN_LEFT_C);
		}

		drawFramerate(fps);
	}
}