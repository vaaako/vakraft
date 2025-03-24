#include "vakraft/world/world.hpp"
#include "scarablib/proper/log.hpp"
#include "vakraft/blocks/block.hpp"
#include "vakraft/world/chunk.hpp"
#include "vakraft/world/random.hpp"
#include <cstdio>
#include <glm/fwd.hpp>
#include <glm/gtc/noise.hpp>
#include <vector>

std::vector<Block*> World::all_voxels;

World::World() noexcept : texture_array(new TextureArray(16, 16, 256)) {
	this->register_all_voxels();
	this->texture_array->generate_mipmap();
}

World::~World() noexcept {
	for(Block* voxel : World::all_voxels) {
		delete voxel;
	}
	World::all_voxels.clear();

	for(Chunk* chunk : this->chunks) {
		delete chunk;
	}

	delete this->texture_array;
}

void World::generate_chunks() noexcept {
	// const int32 NUM_CHUNKS_XZ = 2;
	// const int32 NUM_CHUNKS_2 = NUM_CHUNKS_XZ / 2;

	for(int32 wx = 0; wx < World::WIDTH; wx++) {
		for(int32 wy = 0; wy < World::HEIGHT; wy++) {
			for(int32 wz = 0; wz < World::DEPTH; wz++) {

				// Start from center around (0, -1, 0)
				// Chunk* cur_chunk = new Chunk(wx - NUM_CHUNKS_2, -1, wz - NUM_CHUNKS_2);
				Chunk* cur_chunk = new Chunk(wx, wy, wz);
				int32 chunk_index = wx + World::WIDTH * wz + World::AREA * wy;

				// TODO:Put this somewhere for terrain generation
				for(int32 x = 0; x < Chunk::WIDTH; x++) {
					for(int32 y = 0; y < Chunk::HEIGHT; y++) {
						for(int32 z = 0; z < Chunk::LENGTH; z++) {
							// Compute the absolute Y coordinate in world space
							int32 world_y = wy * Chunk::HEIGHT + y;

							// Determine the highest world terrain level
							int32 highest_terrain = (World::HEIGHT * Chunk::HEIGHT) - 1;

							// Fill voxels
							if(world_y == highest_terrain) {
								if(cur_chunk->get_voxel(x, y - 1, z) == Block::GRASS) {
									cur_chunk->set_voxel(x, y, z,
										Random::choose_weighted<uint8>({
											0, Block::ROSE, Block::DAISY, Block::AHIRO_FLOWER
										}, { 90, 5, 5, 1 }));
								}
							} else if(world_y > highest_terrain - 3) {
								cur_chunk->set_voxel(x, y, z,
									Random::choose_from<uint8>({ 0, Block::GRASS }));
							} else {
								cur_chunk->set_voxel(x, y, z, 1);
							}
						}
					}
				}

				LOG_DEBUG("Chunk index: %i", chunk_index);
				this->chunks.at((size_t)chunk_index) = cur_chunk;
			}
		}
	}

	LOG_DEBUG("Generating %d chunks", this->chunks.size());

	for(Chunk* chunk : this->chunks) {
		chunk->update_mesh(*this);
	}
}

int32 World::get_chunk_id(const int32 world_x, const int32 world_y, const int32 world_z) const noexcept {
	const vec3<int32> chunk_pos = this->get_chunk_pos(world_x, world_y, world_z);

	if(chunk_pos.x < 0 || chunk_pos.x >= World::WIDTH ||
		chunk_pos.y < 0 || chunk_pos.y >= World::HEIGHT ||
		chunk_pos.z < 0 || chunk_pos.z >= World::DEPTH) {
		return -1;
	}

	return chunk_pos.x + World::WIDTH * chunk_pos.z + World::AREA * chunk_pos.y;
}

Chunk* World::get_chunk_at(const int32 world_x, const int32 world_y, const int32 world_z) const noexcept {
	const int32 chunk_index = this->get_chunk_id(world_x, world_y, world_z);
	if(chunk_index == -1) {
		return nullptr;
	}
	return this->chunks.at((size_t)chunk_index);
}

uint8 World::get_voxel_at(const int32 world_x, const int32 world_y, const int32 world_z) const noexcept {
	const Chunk* chunk = this->get_chunk_at(world_x, world_y, world_z);
	if(chunk == nullptr) {
		return 0;
	}

	// Calculate local coordinates within the chunk
	const int32 local_x = world_x - chunk->position.x;
	const int32 local_y = world_y - chunk->position.y;
	const int32 local_z = world_z - chunk->position.z;

	// get_voxel checks if voxel is in bounds
	return chunk->get_voxel(local_x, local_y, local_z);
}



void World::set_voxel(const int32 world_x, const int32 world_y, const int32 world_z, const uint8 voxel) noexcept {
	Chunk* chunk = this->get_chunk_at(world_x, world_y, world_z);
	if(chunk == nullptr) {
		// Dont make a new chunk if voxel is air
		if(voxel == 0) {
			return;
		}

		// Calculate chunk index
		const int32 chunk_index = this->get_chunk_id(world_x, world_y, world_z);
		// Not in the world bounds
		if(chunk_index == -1) {
			return;
		}
		const vec3<int32> chunk_pos = this->get_chunk_pos(world_x, world_y, world_z);
		this->chunks[(size_t)chunk_index] = new Chunk(chunk_pos.x * Chunk::WIDTH, chunk_pos.y * Chunk::HEIGHT, chunk_pos.z * Chunk::LENGTH);
	}

	// Same voxel dont update
	if(this->get_voxel_at(world_x, world_y, world_z) == voxel) {
		return;
	}

	// This will always be valid, since it makes a new chunk if not
	chunk->set_voxel(world_x % Chunk::WIDTH, world_y % Chunk::HEIGHT, world_z % Chunk::LENGTH, voxel);
	chunk->update_mesh(*this);
}


void World::register_all_voxels() noexcept {
	// Order matters here
	World::all_voxels.emplace_back(new Block());
	this->register_voxel(new Block({ .id = 1, .name = "stone", .model_type = Block::Model::CUBE }));
	this->register_voxel(new Block({ .id = 2, .name = "grass", .model_type = Block::Model::CUBE }));

	// Flowers
	this->register_voxel(new Block({ .id = 3, .name = "rose",  .model_type = Block::Model::FLOWER,
				.is_transparent = true, .is_cube = false }));
	this->register_voxel(new Block({ .id = 4, .name = "daisy", .model_type = Block::Model::FLOWER,
				.is_transparent = true, .is_cube = false }));
	this->register_voxel(new Block({ .id = 5, .name = "ahiro_flower", .model_type = Block::Model::FLOWER,
				.is_transparent = true, .is_cube = false }));
}

void World::register_voxel(Block* voxel) noexcept {
	uint32 tex_index = texture_array->get_current_index(); // Index 0 for first texture

	// Change voxel face texture
	for(vec3<float>& tex : voxel->texuv) {
		tex.z = static_cast<float>(tex_index);
	}

	LOG_DEBUG("Added voxel %d with texture index %d", voxel->id, tex_index);

	// Load texture after modifing the texuv (because then the index is incremented)
	std::string texture_path = "resources/textures/" + std::string(voxel->name) + ".png";
	texture_array->add_texture({ .path = texture_path.c_str() });

	World::all_voxels.emplace_back(voxel);
}

void World::draw() const noexcept {
	this->texture_array->bind();
	for(const Chunk* chunk : this->chunks) {
		chunk->draw();
	}
}
