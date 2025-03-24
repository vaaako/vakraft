#pragma once

#include <scarablib/types/texture_array.hpp>
#include "vakraft/blocks/block.hpp"
#include "vakraft/world/chunk.hpp"

class World {
	public:
		static constexpr int32 WIDTH  = 2;
		static constexpr int32 HEIGHT = 2;
		static constexpr int32 DEPTH  = 2;
		static constexpr int32 AREA   = WIDTH * HEIGHT; // X-Z plane
		static constexpr int32 VOLUME = World::AREA * World::HEIGHT;

		World() noexcept;
		~World() noexcept;

		inline vec3<int32> get_world_center() {
			// The highest Y level in the world
			const int32 top_y = World::HEIGHT * Chunk::HEIGHT;
			const int32 center_x = World::WIDTH * (Chunk::WIDTH / 2);
			const int32 center_z = World::WIDTH * (Chunk::LENGTH / 2);
			return {
				center_x,
				top_y,
				center_z
			};
		}

		// Get voxel reference by its ID
		inline static Block& get_voxel_by_id(const uint32 id) noexcept {
			return *World::all_voxels.at(id);
		}

		// Uses world-space coordinates to get the chunk index the coordinate is in.
		// If chunk is not in the world, return -1
		int32 get_chunk_id(const int32 world_x, const int32 world_y, const int32 world_z) const noexcept;

		// Calculates the chunk position in the world
		inline vec3<int32> get_chunk_pos(const int32 world_x, const int32 world_y, const int32 world_z) const noexcept {
			return {
				world_x / Chunk::WIDTH,
				world_y / Chunk::HEIGHT,
				world_z / Chunk::LENGTH
			};
		}

		// Coordinates must be in world-space
		// Try to return a chunk if position is inside of it.
		// Return nullptr if no chunk is found
		Chunk* get_chunk_at(const int32 world_x, const int32 world_y, const int32 world_z) const noexcept;

		// Coordinates must be in world-space.
		// If chunk is not found, return 0
		uint8 get_voxel_at(const int32 world_x, const int32 world_y, const int32 world_z) const noexcept;

		// Set a voxel in a position in the world.
		// If the position does not have a chunk, create one
		void set_voxel(const int32 x, const int32 y, const int32 z, const uint8 voxel) noexcept;

		// Generate all chunks
		void generate_chunks() noexcept;

		// Draw all chunks
		void draw() const noexcept;

	private:
		std::vector<Chunk*> chunks = std::vector<Chunk*>(World::VOLUME, nullptr);
		TextureArray* texture_array;
		static std::vector<Block*> all_voxels;

		void register_all_voxels() noexcept;
		void register_voxel(Block* voxel) noexcept;
};
