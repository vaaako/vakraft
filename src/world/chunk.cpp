#include "vakraft/world/chunk.hpp"
#include "scarablib/proper/log.hpp"
#include "vakraft/blocks/block.hpp"
#include "vakraft/blocks/models/block_factory.hpp"
#include "vakraft/world/world.hpp"
#include <vector>

Chunk::Chunk(const vec3<int32> chunk_position) noexcept
	: chunk_position(chunk_position), position(chunk_position.x * WIDTH, chunk_position.y * HEIGHT, chunk_position.z * LENGTH) {}

Chunk::Chunk(const int32 x, const int32 y, const int32 z) noexcept
	: chunk_position(x, y, z), position(x * WIDTH, y * HEIGHT, z * LENGTH) {}

void Chunk::update_mesh(const World& world) noexcept {
	std::vector<Vertex> vertices_data;
	std::vector<uint32> indices;

	// Offset of one mesh to another
	uint32 mesh_index_offset = 0;

	// Anonymous function for rendering a specific face
	auto add_face = [&](const Block& voxel, const uint32 face, const vec3<int32>& world_pos) -> void {
		std::vector<vec3<float>> model_vertices = BlockFactory::get_vertices(voxel.model_type);
		std::vector<vec4<float>> model_shading = BlockFactory::get_shading(voxel.model_type);

		// Add Vertex data for this face
		for(size_t v = 0; v < 4; v++) { // Number of vertices per face
			// f * 4 = Get first index of the desired face
			// v = Vertex index
			vertices_data.push_back(Vertex {
				.position = vec3<float>(model_vertices[face * 4 + v] + vec3<float>(world_pos)),
				.texuv    = voxel.texuv[face * 4 + v],
				.shading  = model_shading[face][static_cast<int32>(v)] // Assign shading per vertex
			});
		}

		uint32 indices_index[6] = { 0, 1, 2, 0, 2, 3 };
		for(size_t i = 0; i < 6; i++) {
			indices.push_back(indices_index[i] + mesh_index_offset);
		}

		// Amount of vertices in a face
		mesh_index_offset += 4;
	};

	for(int x = 0; x < this->WIDTH; x++) {
		for(int y = 0; y < this->HEIGHT; y++) {
			for(int z = 0; z < this->LENGTH; z++) {
				// Make sure x, y and z is integer, because get_voxel may convert uint32 to int (which gives a wrong number)
				uint8 cur_voxel = this->get_voxel(x, y, z);

				// Skip air voxel
				if(cur_voxel == 0) {
					continue;
				}

				// voxel position in world space
				vec3<int32> world_pos = {
					this->position.x + x,
					this->position.y + y,
					this->position.z + z
				};

				// Translate vertices to the relative position inside the chunk
				Block& voxel = World::get_voxel_by_id(cur_voxel);

				// If not cube, just render all faces
				if(!voxel.is_cube) {
					for(uint32 f = 0; f < BlockFactory::get_vertices(voxel.model_type).size() / 4; f++) {
						add_face(voxel, f, world_pos);
					}

					// Skip to next voxel
					continue;
				}

				// Not happy with this but works
				if(this->can_render_face(world, x, y, z + 1)) {
					add_face(voxel, 0, world_pos);
				}
				if(this->can_render_face(world, x, y, z - 1)) {
					add_face(voxel, 1, world_pos);
				}
				if(this->can_render_face(world, x, y + 1, z)) {
					add_face(voxel, 2, world_pos);
				}
				if(this->can_render_face(world, x, y - 1, z)) {
					add_face(voxel, 3, world_pos);
				}
				if(this->can_render_face(world, x - 1, y, z)) {
					add_face(voxel, 4, world_pos);
				}
				if(this->can_render_face(world, x + 1, y, z)) {
					add_face(voxel, 5, world_pos);
				}
			}
		}
	}

	// No data
	if(mesh_index_offset == 0) {
		LOG_DEBUG("CHUNK VERTICES IS EMPTY");
		return;
	}

	this->indices_count = static_cast<GLsizei>(indices.size());

	this->bundle.make_vao_with_manager(vertices_data, indices);
	this->vao_id = this->bundle.get_vao_id();
}

bool Chunk::can_render_face(const World& world, const int32 x, const int32 y, const int32 z) const noexcept {
	// Outside current chunk

	// voxel position in world-space
	int32 world_x = this->position.x + x;
	int32 world_y = this->position.y + y;
	int32 world_z = this->position.z + z;

	// Check voxel in the chunk
	uint8 voxel_id = world.get_voxel_at(world_x, world_y, world_z);
	return (voxel_id == 0) ? true
		: (World::get_voxel_by_id(voxel_id)).is_transparent;
}
