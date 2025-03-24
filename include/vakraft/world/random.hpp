#pragma once

#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <vector>
#include <random>

namespace Random {
	// Randomly choose an element from the given vector
	template <typename T>
	static T choose_from(const std::vector<T>& choices) {
		// Initialize random number generator
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, choices.size() - 1);

		// Select a random index and return the corresponding choice
		return choices[dis(gen)];
	}

	// Choices are the elements to choose from, and weights are the corresponding weights.
	// Example: choose_weighted({ 0, 1, 2 }, { 8, 1, 1 }).
	// 0 has a weight of 8, 1 has a weight of 1, and 2 also has a weight of 1.
	// This means 0 is more likely to be chosen than 1 or 2.
	template <typename T>
	static T choose_weighted(const std::vector<T>& choices, const std::vector<uint8>& weights) {
		// Ensure choices and weights have the same size
		if(choices.size() != weights.size() || choices.empty()) {
			throw ScarabError("Choices and weights must have the same non-zero size");
		}

		// Initialize random number generator
		std::random_device rd;
		std::mt19937 gen(rd());
		std::discrete_distribution<> dis(weights.begin(), weights.end());

		// Select a weighted random index and return the corresponding choice
		return choices[dis(gen)];
	}
};
