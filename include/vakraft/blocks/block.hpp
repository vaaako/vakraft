#pragma once

#include <scarablib/types/vertex.hpp>

struct Block {
	enum BlockID : uint8 {
		AIR,
		STONE,
		GRASS,
		ROSE,
		DAISY,
		AHIRO_FLOWER
	};

	enum class Model : uint8 {
		NONE = 0,
		CUBE,
		FLOWER
	};

	struct Config {
		const uint8 id;
		const char* name;
		const Block::Model model_type;
		const bool is_transparent = false;
		const bool is_cube = true;
	};

	const uint8 id;
	const char* name;
	const Block::Model model_type = Block::Model::NONE;
	const bool is_transparent;
	const bool is_cube;
	std::vector<vec3<float>> texuv;

	Block() noexcept;
	Block(const Block::Config& config) noexcept;
};

