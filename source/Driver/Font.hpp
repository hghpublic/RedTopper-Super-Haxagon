#ifndef SUPER_HAXAGON_FONT_HPP
#define SUPER_HAXAGON_FONT_HPP

#include "Core/Vector.hpp"

#include <memory>
#include <string>

namespace SuperHaxagon {
	enum class Alignment {
		LEFT,
		CENTER,
		RIGHT
	};

	struct Color;

	class Platform;

	class Font {
	public:
		struct FontData;

		explicit Font(std::unique_ptr<FontData> data);
		Font(Font&) = delete;
		~Font();

		void setScale(float scale);
		float getHeight() const;
		float getWidth(const std::string& str) const;
		void draw(const Color& color, const Vec2f& position, Alignment alignment, const std::string& text) const;

	private:
		std::unique_ptr<FontData> _data;
	};
}

#endif //SUPER_HAXAGON_FONT_HPP
