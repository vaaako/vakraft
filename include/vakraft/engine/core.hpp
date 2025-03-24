#pragma once

#include "scarablib/input/keyboard.hpp"
#include "scarablib/input/mouse.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/window/window.hpp"
#include "vakraft/engine/raycast.hpp"
#include "vakraft/world/world.hpp"

class Core {
	public:
		Core(const Window& window) noexcept;
		~Core() noexcept;

		void update(Window& window) noexcept;
		void draw() const noexcept;

		inline Camera& get_camera() noexcept {
			return this->camera;
		}

	private:
		Raycast* ray = nullptr; // Just a pointer to initialize after world
		World* world = nullptr;
		Camera camera;
		bool mouse_captured = false;

		Shader shader = Shader(
			R"glsl(
				#version 330 core

				layout (location = 0) in vec3 aPos;
				layout (location = 1) in vec3 aTex;
				layout (location = 2) in float aShading;

				out vec3 texCoord;
				out float shading;

				uniform mat4 mvp;

				void main() {
					gl_Position = mvp * vec4(aPos, 1.0);
					texCoord    = aTex;
					shading     = aShading;
				}
			)glsl",

			R"glsl(
				#version 330 core

				in vec3 texCoord;
				in float shading;
				out vec4 FragColor;

				uniform sampler2DArray texSampler;

				void main() {
					vec4 tex = texture(texSampler, texCoord);
					FragColor = tex * vec4(vec3(shading), 1.0); // Apply shading as brightness
					if(tex.a == 0.0) {
						discard;
					}
				}
			)glsl"
		);

		void hit_callback(MouseHandler& mouse, const vec3<int32>& block, const vec3<int32>& prev_block) const noexcept;
		void update_keyboard(Window& window, KeyboardHandler& keyboard, const float dt) noexcept;
		void update_mouse(const Window& window, MouseHandler& mouse, const float dt) noexcept;

};
