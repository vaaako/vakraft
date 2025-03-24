#include "scarablib/gfx/2d/sprite.hpp"
#include "vakraft/engine/core.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <linux/limits.h>
#include <scarablib/scenes/scene3d.hpp>
#include <scarablib/scenes/scene2d.hpp>
#include <scarablib/scenes/camera.hpp>
#include <scarablib/scenes/camera2d.hpp>
#include <scarablib/gfx/3d/model_factory.hpp>
#include <scarablib/gfx/2d/sprite_factory.hpp>
#include <scarablib/types/color.hpp>
#include <scarablib/types/font.hpp>
#include <scarablib/window/window.hpp>

#include <cstdio>
#include <string>
#include <format>

int main() {
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = "Vakraft",
		.vsync = false,
		.resizable = true
	});
	window.set_clear_color(Colors::MAGENTA);

	Camera2D camera2d = Camera2D(window);
	Font font = Font(camera2d, "resources/fonts/dos.ttf", 20);

	Core core = Core(window);
	Camera& camera = core.get_camera();

	uint32 timer = window.timenow();

	Scene2D scene2d = Scene2D(camera2d);
	Rectangle* aim = SpriteFactory::create_rectangle();
	aim->set_size({ 5.0f, 5.0f });
	aim->set_position({ window.get_half_width() - 5.0f, window.get_half_height() - 5.0f });
	scene2d.add_to_scene("aim", aim);

	while(window.is_open()) {
		window.clear();

		window.process_events();

		if(window.has_event(Event::WINDOW_QUIT)) {
			window.close();
		}

		window.on_event(Event::WINDOW_RESIZED, [&](){
			core.get_camera().update_viewport(window);
			camera2d.update_viewport(window);

			aim->set_position({ window.get_half_width() - 5.0f, window.get_half_height() - 5.0f });
		});

		core.update(window);
		core.draw();

		// WARNING: i need to improve font object and logic 
		glDisable(GL_CULL_FACE);
		vec3<int32> pos = camera.get_position();
		font.draw_text(std::format("X: {}, Y: {}, Z: {}", pos.x, pos.y, pos.z), { 0.0f, 10.0f });

		scene2d.draw_all();
		glEnable(GL_CULL_FACE);

		// Elapsed 1 second
		uint32 current = window.timenow();
		if(current - timer >= 1000) {
			timer = current; // Reset timer
			window.set_title("FPS: " + std::to_string(window.fps()) + " / Delta: " + std::to_string(window.dt()));
		}

		window.swap_buffers();
	}
}
