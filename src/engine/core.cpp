#include "vakraft/engine/core.hpp"
#include "scarablib/proper/log.hpp"
#include "vakraft/engine/raycast.hpp"

Core::Core(Window& window) noexcept
	: window(window), world(new World()), camera(Camera(window)) {

	// OpenGL
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Set world
	this->world->generate_chunks();

	// Set Camera
	this->camera.set_fov(90.0f);
	this->camera.set_speed(100.0f);

	// this->camera.set_sensitivity(0.002f);

	this->camera.set_posiion(this->world->get_world_center());

	// Set ray
	this->ray = new Raycast(*world);

	// TODO: Rename voxel to voxel (everywhere)
}

Core::~Core() noexcept {
	delete this->ray;
	delete this->world;
}

void Core::update() noexcept {
	const float dt = this->window.dt();

	this->update_keyboard(this->window.keyboard(), dt);
	this->update_mouse(this->window.mouse(), dt);
}

void Core::update_keyboard(KeyboardHandler& keyboard, const float dt) noexcept {
	vec3<float> direction = vec3<float>(0.0f);

	if(keyboard.isdown(Keycode::W)) {
		direction.x = 1.0f;
	} else if (keyboard.isdown(Keycode::S)) {
		direction.x = -1.0f;
	}

	if(keyboard.isdown(Keycode::A)) {
		direction.z = 1.0f;
	} else if (keyboard.isdown(Keycode::D)) {
		direction.z = -1.0f;
	}

	if(keyboard.isdown(Keycode::SPACE)) {
		direction.y = 1.0f;
	} else if(keyboard.isdown(Keycode::LSHIFT)) {
		direction.y = -1.0f;
	}

	camera.move(direction, dt);


	// TESTING voxel PLACEMENT
	// vec3<int32> pos = camera.get_position();
	// this->world->set_voxel((int)pos.x, (int)pos.y, (int)pos.z, 1);
	// LOG_DEBUG("voxel (%i, %i, %i): %i", pos.x, pos.y, pos.z, this->world->get_voxel_at(pos.x, pos.y, pos.z));
	// TESTING voxel PLACEMENT


	// ROTATION LOGIC //
	if(this->mouse_captured && keyboard.ispressed(Keycode::ESCAPE)) {
		this->mouse_captured = false;
		window.grab_cursor(false);
	} else if(!this->mouse_captured && keyboard.ispressed(Keycode::ESCAPE)) {
		window.close();
	}
}

void Core::hit_callback(MouseHandler& mouse, const vec3<int32>& voxel, const vec3<int32>& last) const noexcept {
	LOG_DEBUG("Hit voxel (%i, %i, %i)", voxel.x, voxel.y, voxel.z);

	if(mouse.isclick(MouseHandler::Button::LMB)) {
		this->world->set_voxel(voxel.x, voxel.y, voxel.z, 0);

		mouse.set_keystate(MouseHandler::Button::LMB, MouseHandler::State::RELEASED);
	}

	if(mouse.isclick(MouseHandler::Button::RMB)) {
		LOG_DEBUG("try to place");
		this->world->set_voxel(last.x, last.y, last.z, 1);
	}
}

void Core::update_mouse(MouseHandler& mouse, const float dt) noexcept {
	// Only rotate when click on screen
	if(!this->mouse_captured && mouse.isclick(MouseHandler::Button::LMB)) {
		LOG_INFO("Clicked");
		this->mouse_captured = true;
		this->window.grab_cursor(true);
	}

	// Rotate camera
	if(this->mouse_captured) {
		camera.rotate(mouse);
	}

	if(this->window.has_event(MOUSEBUTTONDOWN)) {
		ray->check(camera.get_position(), camera.get_forward(),
			[&](const vec3<int32>& voxel, const vec3<int32>& next_voxel) {
				this->hit_callback(mouse, voxel, next_voxel);
			});
	}
}


void Core::draw() const noexcept {
	this->shader.use();
	this->shader.set_matrix4f("mvp", camera.get_proj_matrix() * camera.get_view_matrix());

	this->world->draw();
}
