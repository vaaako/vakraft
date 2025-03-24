#include "vakraft/engine/raycast.hpp"
#include "scarablib/proper/log.hpp"
#include <cmath>
#include <glm/common.hpp>

Raycast::Raycast(World& world) noexcept : world(world) {}

bool Raycast::check(const vec3<float>& origin, const vec3<float>& forward, const std::function<void(const vec3<int32>&, const vec3<int32>&)>& callback) noexcept {
	float step = 1.0f;

	// Last empty voxel
	vec3<int32> last_voxel = vec3<int32>(origin.x, origin.y, origin.z);
	for(float t = 0; t < Raycast::HIT_RANGE; t += step) {
		const vec3<float> point = origin + forward * t;

		// Round is very important here
		// small floating-point inaccuracies can cause the ray to "miss" the intended block or hit neighboring blocks
		const vec3<int32> voxel = {
			std::round(point.x),
			std::round(point.y),
			std::round(point.z)
		};


		LOG_DEBUG("Checking voxel: (%i, %i, %i)", voxel.x, voxel.y, voxel.z);
		if (world.get_voxel_at(voxel.x, voxel.y, voxel.z) != 0) {
			callback(voxel, last_voxel);
			return true;
		}

		last_voxel = voxel;
	}

	return false;
}
