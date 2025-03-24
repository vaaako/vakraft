#pragma once

#include "scarablib/typedef.hpp"
#include "vakraft/world/world.hpp"
#include <functional>

// DDA (Digital Differential Algorithm) algorithm
class Raycast {
	public:
		// Maximum distance before the ray stops
		static constexpr uint32 HIT_RANGE = 5;

		// Default constructor.
		// Use HitRay::init_ray to set values each check
		Raycast(World& world) noexcept;

		// Checks if the ray hit some block.
		// position: camera's position.
		// direction: is the unit vector of camera yaw and pitch (Camera::get_unit_direction gives this vector).
		// Returns true if the ray hit something
		bool check(const vec3<float>& origin, const vec3<float>& forward, const std::function<void(const vec3<int32>&, const vec3<int32>&)>& callback) noexcept;

	private:
		World& world;
};
