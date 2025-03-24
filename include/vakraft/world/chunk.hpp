#pragma once

#include <scarablib/opengl/bufferbundle.hpp>
#include <scarablib/opengl/ebo.hpp>
#include <scarablib/typedef.hpp>

class World;

class Chunk {
	friend class World;

	public:
		static constexpr int32 WIDTH  = 16;
		static constexpr int32 HEIGHT = 16;
		static constexpr int32 LENGTH = 16;
		static constexpr int32 AREA   = Chunk::WIDTH * Chunk::HEIGHT;

		Chunk(const vec3<int32> chunk_position) noexcept;
		Chunk(const int32 x, const int32 y, const int32 z) noexcept;

		inline vec3<int32> get_chunk_position() const noexcept {
			return this->chunk_position;
		}

		// Check if the coordinates are valid
		inline bool is_valid(const int32 x, const int32 y, const int32 z) const noexcept {
			return (x >= 0 && x < WIDTH) &&
				   (y >= 0 && y < HEIGHT) &&
				   (z >= 0 && z < LENGTH);
		}

		// Coordinates must be in chunk local position
		// If coordinates are outside the chunk, return 0.
		inline uint8 get_voxel(const int32 x, const int32 y, const int32 z) const noexcept {
			if(!this->is_valid(x, y, z)) {
				return 0;
			}
			return this->voxels.at(static_cast<size_t>(x + (y * WIDTH) + (z * WIDTH * HEIGHT)));
		}

		// Add a voxel to the chunk.
		// Return 0 if coordinates are outside the chunk
		inline uint8 set_voxel(const int32 x, const int32 y, const int32 z, const uint8 voxel) noexcept {
			if(!this->is_valid(x, y, z)) {
				return 0;
			}
			return this->voxels[static_cast<size_t>(x + (y * WIDTH) + (z * WIDTH * HEIGHT))] = voxel;
		}

		// Draw current chunk
		inline void draw() const noexcept {
			glBindVertexArray(this->vao_id);
			glDrawElements(GL_TRIANGLES, this->indices_count, GL_UNSIGNED_INT, 0);
		}

		// Update chunk voxels.
		// This method should be called after all chunks are generated
		void update_mesh(const World& world) noexcept;

	private:
		vec3<int32> chunk_position; // Chunk position in world-space
		vec3<int32> position;       // Position of chunk order generated

		BufferBundle bundle;
		GLsizei indices_count = 0;
		uint32 vao_id = 0; // For easy access

		// X, Y, Z voxels
		// Here voxels are represented as numbers
		// Fill all with air (0)
		// 1D vector and uint8 for save space
		std::vector<uint8> voxels = std::vector<uint8>(WIDTH * HEIGHT * LENGTH, 0);

		// Check if the voxel can be rendered
		bool can_render_face(const World& world, const int32 x, const int32 y, const int32 z) const noexcept;
};
