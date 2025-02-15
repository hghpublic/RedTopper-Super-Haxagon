#include "Driver/Music.hpp"

#include <SFML/Audio.hpp>

namespace SuperHaxagon {
	struct Music::MusicImpl {
		explicit MusicImpl(std::unique_ptr<sf::Music> music) : music(std::move(music)) {}
		std::unique_ptr<sf::Music> music;
	};

	Music::Music(std::unique_ptr<MusicImpl> data) : _impl(std::move(data)) {}

	Music::~Music() {
		_impl->music->stop();
	}

	// SFML: Nothing to do on update.
	void Music::update() const {}

	void Music::setLoop(const bool loop) const {
		_impl->music->setLoop(loop);
	}

	void Music::play() const {
		_impl->music->play();
	}

	void Music::pause() const {
		_impl->music->pause();
	}

	bool Music::isDone() const {
		return _impl->music->getStatus() == sf::SoundSource::Stopped;
	}

	float Music::getTime() const {
		return _impl->music->getPlayingOffset().asSeconds();
	}

	std::unique_ptr<Music> createMusic(std::unique_ptr<sf::Music> music) {
		return std::make_unique<Music>(std::make_unique<Music::MusicImpl>(std::move(music)));
	}
}
