#include "vakraft/blocks/models/block_factory.hpp"

std::vector<vec3<float>> BlockFactory::get_vertices(const Block::Model Model) {
	switch (Model) {
		case Block::Model::CUBE:
			return {
				// Front face
				vec3<float>(-0.5f, -0.5f, 0.5f), // 0
				vec3<float>( 0.5f, -0.5f, 0.5f),
				vec3<float>( 0.5f,  0.5f, 0.5f),
				vec3<float>(-0.5f,  0.5f, 0.5f), // 3
				// Back face
				vec3<float>(-0.5f, -0.5f, -0.5f), // 4
				vec3<float>(-0.5f,  0.5f, -0.5f),
				vec3<float>( 0.5f,  0.5f, -0.5f),
				vec3<float>( 0.5f, -0.5f, -0.5f), // 7
				// Top face
				vec3<float>(-0.5f, 0.5f, -0.5f), // 8
				vec3<float>(-0.5f, 0.5f,  0.5f),
				vec3<float>( 0.5f, 0.5f,  0.5f),
				vec3<float>( 0.5f, 0.5f, -0.5f), // 11
				// Bottom face
				vec3<float>(-0.5f, -0.5f, -0.5f), // 12
				vec3<float>( 0.5f, -0.5f, -0.5f),
				vec3<float>( 0.5f, -0.5f,  0.5f),
				vec3<float>(-0.5f, -0.5f,  0.5f), // 15
				// Left face
				vec3<float>(-0.5f, -0.5f, -0.5f), // 16
				vec3<float>(-0.5f, -0.5f,  0.5f),
				vec3<float>(-0.5f,  0.5f,  0.5f),
				vec3<float>(-0.5f,  0.5f, -0.5f), // 19
				// Right face
				vec3<float>(0.5f, -0.5f, -0.5f), // 20
				vec3<float>(0.5f,  0.5f, -0.5f),
				vec3<float>(0.5f,  0.5f,  0.5f),
				vec3<float>(0.5f, -0.5f,  0.5f)  // 23
			};

		case Block::Model::FLOWER:
			return {
				vec3<float>(-0.3536f,  0.5f,  0.3536f),
				vec3<float>(-0.3536f, -0.5f,  0.3536f),
				vec3<float>( 0.3536f, -0.5f, -0.3536f),
				vec3<float>( 0.3536f,  0.5f, -0.3536f),

				vec3<float>(-0.3536f,  0.5f, -0.3536f),
				vec3<float>(-0.3536f, -0.5f, -0.3536f),
				vec3<float>( 0.3536f, -0.5f,  0.3536f),
				vec3<float>( 0.3536f,  0.5f,  0.3536f),

				vec3<float>( 0.3536f,  0.5f, -0.3536f),
				vec3<float>( 0.3536f, -0.5f, -0.3536f),
				vec3<float>(-0.3536f, -0.5f,  0.3536f),
				vec3<float>(-0.3536f,  0.5f,  0.3536f),

				vec3<float>( 0.3536f,  0.5f,  0.3536f),
				vec3<float>( 0.3536f, -0.5f,  0.3536f),
				vec3<float>(-0.3536f, -0.5f, -0.3536f),
				vec3<float>(-0.3536f,  0.5f, -0.3536f)
			};

		default:
			return {};
	}
};

std::vector<vec3<float>> BlockFactory::get_texuv(const Block::Model Model) {
	switch (Model) {
		case Block::Model::CUBE:
			return {
				// Front face
				vec3<float>(0.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 1.0f, 0.0f),
				// Back face
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 0.0f, 0.0f),
				// Top face
				vec3<float>(0.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),
				// Bottom face
				vec3<float>(0.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 1.0f, 0.0f),
				// Left face
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 0.0f, 0.0f),
				// Right face
				vec3<float>(0.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 1.0f, 0.0f)
			};

		case Block::Model::FLOWER:
			return {
				vec3<float>(0.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),

				vec3<float>(0.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),

				vec3<float>(0.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f),

				vec3<float>(0.0f, 1.0f, 0.0f),
				vec3<float>(0.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 0.0f, 0.0f),
				vec3<float>(1.0f, 1.0f, 0.0f)
			};

		default:
			return {};
	}
};

std::vector<vec4<float>> BlockFactory::get_shading(const Block::Model Model) {
	switch (Model) {
		case Block::Model::CUBE:
			return {
				vec4<float>(0.6f, 0.6f, 0.6f, 0.6f), // Front
				vec4<float>(0.6f, 0.6f, 0.6f, 0.6f), // Back
				vec4<float>(1.0f, 1.0f, 1.0f, 1.0f), // Top
				vec4<float>(0.4f, 0.4f, 0.4f, 0.4f), // Bottom
				vec4<float>(0.8f, 0.8f, 0.8f, 0.8f), // Left
				vec4<float>(0.8f, 0.8f, 0.8f, 0.8f)  // Right
			};

		case Block::Model::FLOWER:
			return {
				vec4<float>(1.0f, 1.0f, 1.0f, 1.0f),
				vec4<float>(1.0f, 1.0f, 1.0f, 1.0f),
				vec4<float>(1.0f, 1.0f, 1.0f, 1.0f),
				vec4<float>(1.0f, 1.0f, 1.0f, 1.0f),
			};

		default:
			return {};
	}
}
