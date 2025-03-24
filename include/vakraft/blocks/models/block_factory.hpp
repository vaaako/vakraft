#pragma once

#include "vakraft/blocks/block.hpp"
#include <scarablib/typedef.hpp>
#include <vector>

struct BlockFactory {
	static std::vector<vec3<float>> get_vertices(const Block::Model Model);
	static std::vector<vec3<float>> get_texuv(const Block::Model Model);
	static std::vector<vec4<float>> get_shading(const Block::Model Model);
};
