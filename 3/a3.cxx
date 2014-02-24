#include <mrr/graphics/common.hxx>
#include <mrr/graphics/models/cube.hxx>
using namespace mrr::graphics;

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <list>

int main()
{
	glfw::init();
	glfw::window_handle window(1920, 1080, "03-60-352 Assignment 3");
	glew::init();
	gl::init(0.0f, 0.0f, 0.0f, 0.2f);
	gl::vertex_array vao;

	glm::mat4 projection = glm::perspective(
		(float)M_PI/4.0f,  // Horizontal field of view (angle of view).
		16.0f/9.0f,        // Aspect ratio.
		0.1f,              // Near clipping plane.
		200.0f             // Far clipping plane.
	);

	glm::vec3 camera_location(0, 0, 30);

	glm::mat4 view = glm::lookAt(
		camera_location,      // Camera location.
		glm::vec3(0, 0, 0),   // Looks at the origin.
		glm::vec3(0, 1, 0)    // Head is facing up.
	);

	auto colour_shader = gl::colour_shader();
	auto texture_shader = gl::texture_shader();

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Arena
	gl::component arena;
	arena.set_shader(texture_shader);
	arena.load_wavefront("/home/matt/arena.obj");
	arena.load_texture("diffuse.DDS");
	arena.set_model(glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(1,0,0)));

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Prey.
	gl::component prey;
	prey.set_shader(colour_shader);
	prey.load_wavefront("/home/matt/prey.obj");
	prey.set_colour(glm::vec3(0.4, 0.4, 0.0));

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Predator
	gl::component predator;
	predator.set_shader(texture_shader);
	predator.load_wavefront("suzanne.obj");
	predator.load_texture("uvmap.DDS");

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Floor.
	gl::component floor;
	floor.set_shader(texture_shader);
	floor.load_wavefront("/home/matt/plane.obj");
	floor.load_texture("brick.DDS");
	floor.set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(8.0, 8.0, 1.0)) *
		glm::rotate(glm::mat4(1.0f), (float)M_PI/2, glm::vec3(1, 0, 0))
	);

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Scene.
	gl::model scene;

	scene.add_component(floor);
	scene.add_component(arena);

	// Lighting.
	scene.set_ambient_light_colour(glm::vec3(0.1, 0.1, 0.1));

	scene.add_point_source(
		glm::vec3(15, 0, 15),
		glm::vec3(1, 1, 1),
		200.0f
	);

	scene.add_point_source(
		glm::vec3(-15, 0, 15),
		glm::vec3(1, 1, 1),
		200.0f
	);

	scene.add_point_source(
		glm::vec3(5, 5, -5),
		glm::vec3(1, 0.8, 0.8),
		30.0f
	);

	glfw::add_key_callback(glfw::key_callbacks::quit_on_escape);

	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Key callbacks.
	glm::mat4 camera_translate = glm::mat4(1.0f);
	glm::mat4 camera_strafe = glm::mat4(1.0f);
	glm::mat4 camera_pan = glm::mat4(1.0f);

	auto zoom_in   = [&]() { camera_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.2)); };
	auto zoom_out  = [&]() { camera_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.2)); };
	auto zoom_stop = [&]() { camera_translate = glm::mat4(1.0f); };

	auto pan_down = [&]() { camera_pan = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.2, 0)); };
	auto pan_up   = [&]() { camera_pan = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.2, 0)); };
	auto pan_stop = [&]() { camera_pan = glm::mat4(1.0f); };

	auto strafe_left  = [&]() { camera_strafe = glm::translate(glm::mat4(1.0f), glm::vec3(0.2, 0, 0)); };
	auto strafe_right = [&]() { camera_strafe = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2, 0, 0)); };
	auto strafe_stop  = [&]() { camera_strafe = glm::mat4(1.0f); };

	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_W,     pan_up);
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_S,     pan_down);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_W,     pan_stop);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_S,     pan_stop);

	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_A,     strafe_left);
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_D,     strafe_right);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_A,     strafe_stop);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_D,     strafe_stop);

	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_UP,    zoom_in);
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_DOWN,  zoom_out);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_UP,    zoom_stop);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_DOWN,  zoom_stop);


	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	float default_translation_speed = 0.05f;

	glm::vec4 prey_heading(0, default_translation_speed, 0, 1);
	glm::mat4 prey_movement = glm::translate(glm::mat4(1.0f), glm::vec3(prey_heading));
	glm::mat4 prey_rotation;
	int prey_rotation_timer = 0;
	int prey_exit_timer = 0;

	glm::vec4 predator_heading(0, -2 * default_translation_speed, 0, 1);
	glm::mat4 predator_movement = glm::translate(glm::mat4(1.0f), glm::vec3(predator_heading));
	glm::mat4 predator_rotation;
	int predator_rotation_timer = 0;

	bool prey_present = false;
	bool predator_present = false;

	std::list<gl::component> added_components;

	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Mouse callbacks.
	glfw::add_mouse_callback(
		GLFW_PRESS, GLFW_MOUSE_BUTTON_LEFT,
		[&]()
		{
			int width = window.get_width();
			int height = window.get_height();

			double zx = 1308.5 * (width/1920.0);
			double zy = 1308.5 * (height/1080.0);
			double Z = camera_location.z - 0.5;
			double x, y;
			std::tie(x, y) = window.get_cursor_position();

			x = x - (width/2.0);
			y = (height/2.0) - y;

			std::clog << "Width: " << width << "\n";
			std::clog << "Height: " << height << "\n";

			std::clog << "(" << x << ", " << y << ")\n";

			int predator_rotation;
			double hx, hy;
			bool possibly_invalid = false;
			bool position_valid = true;
			float X = (x * Z)/zx;
			float Y = (y * Z)/zy;

			std::clog << "(" << X << ", " << Y << ")\n";

			if (abs(X - 6.2) < 0.2)
			{
				predator_rotation = 2;
				X = 6.2;
				hx = 0;
				hy = 1;
			}
			else if (abs(X + 6.2) < 0.2)
			{
				predator_rotation = 0;
				X = -6.2;
				hx = 0;
				hy = -1;
			}
			else
				possibly_invalid = true;

			if (abs(Y - 6.2) < 0.2)
			{
				predator_rotation = -1;
				Y = 6.2;
				hx = -1;
				hy = 0;
			}
			else if (abs(Y + 6.1) < 0.2)
			{
				predator_rotation = 1;
				Y = -6.1;
				hx = 1;
				hy = 0;
			}
			else if (possibly_invalid)
				position_valid = false;

			if (position_valid)
			{
				if (!prey_present)
				{
					prey_present = true;
					prey.set_model(
						glm::translate(glm::mat4(1.0f), glm::vec3(X, Y, 0.5f)) *
						glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5))
					);

					prey_heading = glm::vec4(hx * default_translation_speed, hy * default_translation_speed, 0, 1);
					prey_movement = glm::translate(glm::mat4(1.0f), glm::vec3(prey_heading));
					scene.add_component(prey);
				}
				else if (!predator_present)
				{
					predator_present = true;
					predator.set_model(
						glm::translate(glm::mat4(1.0f), glm::vec3(X, Y, 0.5f)) *
						glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5))  *
						glm::rotate(glm::mat4(1.0f), (float)M_PI/2, glm::vec3(1, 0, 0)) *
						glm::rotate(glm::mat4(1.0f), predator_rotation * (float)M_PI/2, glm::vec3(0, 1, 0))
					);

					predator_heading = glm::vec4(2 * hx * default_translation_speed, 2 * hy * default_translation_speed, 0, 1);
					predator_movement = glm::translate(glm::mat4(1.0f), glm::vec3(predator_heading));
					scene.add_component(predator);
				}
			}
		}
	);


	auto prey_turn_left = [&] (gl::component& c) {
		prey_heading = glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(0, 0, 1)) * prey_heading;
		prey_movement = glm::translate(glm::mat4(1.0f), glm::vec3(prey_heading));
		prey_rotation_timer = 16;
	};

	auto prey_turn_right = [&] (gl::component& c) {
		prey_heading = glm::rotate(glm::mat4(1.0f), -(float)M_PI/2.0f, glm::vec3(0, 0, 1)) * prey_heading;
		prey_movement = glm::translate(glm::mat4(1.0f), glm::vec3(prey_heading));
		prey_exit_timer = 32;
	};

	auto predator_turn_left = [&] (gl::component& c) {
		predator_heading = glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(0, 0, 1)) * predator_heading;
		predator_movement = glm::translate(glm::mat4(1.0f), glm::vec3(predator_heading));
		predator_rotation_timer = 16;
	};

	// Waypoints.
	std::vector<gl::waypoint> corners = {
		{ glm::vec3(6.2f, 6.2f, 0.5f),   0.05 },
		{ glm::vec3(-6.2f, 6.2f, 0.5f),  0.05 },
		{ glm::vec3(-6.2f, -6.1f, 0.5f), 0.05 },
		{ glm::vec3(6.2f, -6.1f, 0.5f),  0.05 }
	};

	for (gl::waypoint& corner : corners)
	{
		corner.add_action(prey, prey_turn_left);
		corner.add_action(predator, predator_turn_left);
	}

	gl::waypoint exit(glm::vec3(0.0f, 6.2f, 0.5f), 0.1);

	exit.add_action(
		prey,
		[&] (gl::component& c)
		{
			if (predator_present && !prey_exit_timer)
				prey_turn_right(prey);
		}
	);

	window.add_waypoint(exit);
	window.add_waypoints(corners.begin(), corners.end());

	window.main_loop(
		[&]()
		{
			view = camera_pan * camera_strafe * camera_translate * view;

			if (prey_present && predator_present
			    && glm::length(prey.get_location() - predator.get_location()) < 0.1)
			{
				predator.apply_fp_transformation(glm::scale(glm::mat4(1.0f), glm::vec3(1.3, 1.3, 1.3)));
				prey_present = false;
				scene.remove_component(prey);
				predator_movement = glm::mat4(1.0f);
			}

			if (prey_rotation_timer)
			{
				--prey_rotation_timer;
				prey_rotation = glm::rotate(glm::mat4(1.0f), (float)M_PI/32.0f, glm::vec3(0, 0, 1));
			}
			else
			{
				prey_rotation = glm::mat4(1.0f);
			}

			if (predator_rotation_timer)
			{
				--predator_rotation_timer;
				predator_rotation = glm::rotate(glm::mat4(1.0f), (float)M_PI/32.0f, glm::vec3(0, 1, 0));
			}
			else
			{
				predator_rotation = glm::mat4(1.0f);
			}

			// Prey leaving maze.
			if (prey_exit_timer)
			{
				--prey_exit_timer;

				if (prey_exit_timer == 0)
				{
					prey_movement = glm::mat4(1.0f);
					predator_movement = glm::mat4(1.0f);
					predator.apply_fp_transformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.8, 0.8, 0.8)));
				}
			}

			// Update models.
			if (prey_present)
			{
				prey.update_model(prey_movement);
				prey.apply_fp_transformation(prey_rotation);
			}

			if (predator_present)
			{
				predator.update_model(predator_movement);
				predator.apply_fp_transformation(predator_rotation);
			}

			scene.render(view, projection);
			// std::clog << window.get_ms_per_frame() << " ms/frame\n";
		}
	);
}
