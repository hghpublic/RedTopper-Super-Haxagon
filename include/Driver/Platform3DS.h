#ifndef SUPER_HAXAGON_PLATFORM_3DS_H
#define SUPER_HAXAGON_PLATFORM_3DS_H

#include "Structs.h"
#include "Platform.h"
#include "Audio.h"
#include "Player.h"

static const int MAX_TRACKS = 4;

namespace SuperHaxagon {
	class Platform3DS : public Platform {
	public:
		Platform3DS();
		~Platform3DS();

		bool hasScreen(Screen test) override;

		std::string getPath(const std::string& partial) override;
		std::string getPathRom(const std::string& partial) override;
		std::unique_ptr<Audio> loadAudio(const std::string& path) override;

		void playSFX(Audio& audio) override;
		void playBGM(Audio& audio) override;
		void stopBGM() override;

		void pollButtons() override;
		Buttons getDown() override;
		Buttons getPressed() override;

		Point getScreenDim() const override;
		Point getShadowOffset() const override;

		void drawRect(const Color& color, const Point& point, const Point& size) const override;
		void drawTriangle(const Color& color, const std::array<Point, 3>& points) const override;

	private:
		static Buttons toButtons(u32);
		std::unique_ptr<Player> sfx[MAX_TRACKS];
		std::unique_ptr<Player> bgm;
	};
}

#endif //SUPER_HAXAGON_PLATFORM_3DS_H
