#include "Driver/Platform.hpp"

#include "Core/Twist.hpp"

#include <iostream>

namespace SuperHaxagon {
	std::unique_ptr<Twist> Platform::getTwister() {
		std::random_device source;
		std::mt19937::result_type data[std::mt19937::state_size];
		generate(std::begin(data), std::end(data), ref(source));
		return std::make_unique<Twist>(
				std::make_unique<std::seed_seq>(std::begin(data), std::end(data))
		);
	}
}
