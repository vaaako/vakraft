#pragma once

#include <scarablib/typedef.hpp>
#include <vector>

struct BlockModel {
	std::vector<vec3<float>> vertices;
	std::vector<vec3<float>> texuv;
};
