#include <mrr/graphics/common.hxx>
#include <mrr/graphics/models/cube.hxx>
#include <mrr/graphics/models/pointy_tire.hxx>
using namespace mrr::graphics;

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

int main()
{
	glfw::init();
	glfw::window_handle window(1920, 1080, "03-60-352 Assignment 4");
	glew::init();
	gl::init(0.0f, 0.0f, 0.0f, 0.2f);
	gl::vertex_array vao;


	glm::mat4 main_projection = glm::perspective(
		(float)M_PI/4.0f,  // Horizontal field of view (angle of view).
		16.0f/9.0f,        // Aspect ratio.
		0.1f,              // Near clipping plane.
		200.0f             // Far clipping plane.
	);

	glm::vec3 camera_location(0, -20, 30);

	glm::mat4 main_view = glm::lookAt(
		camera_location,      // Camera location.
		glm::vec3(0, 0, 0),   // Looks at the origin.
		glm::vec3(0, 1, 0)    // Head is facing up.
	);

	auto colour_shader = gl::colour_shader();
	auto texture_shader = gl::texture_shader();

	gl::component road;
	road.set_shader(texture_shader);
	road.load_wavefront("/home/matt/plane.obj");
	road.load_texture("diffuse.DDS");
	road.set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(8.0, 8.0, 1.0)) *
		glm::rotate(glm::mat4(1.0f), (float)M_PI/2, glm::vec3(1, 0, 0))
	);


	// Create model.
	gl::component cab;
	cab.set_shader(colour_shader);
	cab.set_colour(glm::vec3(0.0f, 1.0f, 0.0f));
	cab.load_wavefront("/home/matt/cab.obj");
	glm::mat4 cab_rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(1, 0, 0));
	glm::mat4 cab_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.2f));
	glm::mat4 cab_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-2, -0.0, 0));
	cab.set_model(cab_translation * cab_scale * cab_rotate);

	gl::component trailer;
	trailer.set_shader(colour_shader);
	trailer.load_wavefront("/home/matt/trailer.obj");
	trailer.set_colour(glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 trailer_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-8.5, -0.0, 0));
	trailer.set_model(trailer_translation);


	// Tires.
	glm::mat4 tire_scale = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
	// glm::mat4 tire_rotation = glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 tire_rotation = glm::rotate(glm::mat4(1.0f), 0 * (float)M_PI/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	std::vector<gl::component> tires(10);
	gl::model truck_tires;

	float tire_translation_value[3] = { 0, 0, 0};

	std::vector<glm::mat4> tire_translations(10, glm::mat4(1.0));

	tire_translations[0] = glm::translate(glm::mat4(1.0), glm::vec3(-2.0, 0.6 , -2.0)); // Front right.
	tire_translations[1] = glm::translate(glm::mat4(1.0), glm::vec3(-2.0, -0.6, -2.0)); // Front left.

	tire_translations[2] = glm::translate(glm::mat4(1.0), glm::vec3(-4.5, 0.6 , -2.0)); // Front right.
	tire_translations[3] = glm::translate(glm::mat4(1.0), glm::vec3(-4.5, -0.6, -2.0)); // Front left.

	tire_translations[4] = glm::translate(glm::mat4(1.0), glm::vec3(-7, 1.0 , -2.0));
	tire_translations[5] = glm::translate(glm::mat4(1.0), glm::vec3(-7, -1.0, -2.0));

	tire_translations[6] = glm::translate(glm::mat4(1.0), glm::vec3(-11.5, 1.0 , -2.0));
	tire_translations[7] = glm::translate(glm::mat4(1.0), glm::vec3(-11.5, -1.0, -2.0));

	tire_translations[8] = glm::translate(glm::mat4(1.0), glm::vec3(-13.0, 1.0 , -2.0));
	tire_translations[9] = glm::translate(glm::mat4(1.0), glm::vec3(-13.0, -1.0, -2.0));

	for (int i = 0; i < 10; ++i)
	{
		tires[i].set_shader(colour_shader);
		tires[i].set_colour(glm::vec3(0.0f, 1.0f, 1.0f));
		tires[i].load_wavefront("/home/matt/pointy_tire.obj");
		tires[i].set_model(tire_translations[i] * tire_rotation * tire_scale);
		truck_tires.add_component(tires[i]);
	}

	gl::model truck;
	truck.add_component(cab);
	truck.add_component(trailer);
	truck.add_component(truck_tires);

	truck.save();

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Scene.
	gl::model scene;
	scene.add_component(road);
	scene.add_component(truck);


	float default_translate = 0.01f;
	float translate_by = default_translate;
	bool auto_move = false;

	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Key callbacks.
	glm::mat4 camera_translate = glm::mat4(1.0f);
	glm::mat4 camera_strafe = glm::mat4(1.0f);
	glm::mat4 camera_pan = glm::mat4(1.0f);

	auto reset_truck = [&]() { truck.reset(); };
	auto toggle_move = [&]() { auto_move = !auto_move; };
	auto change_direction = [&]() { translate_by = -translate_by; };
	auto move_left = [&]() { translate_by = translate_by < 0 ? translate_by : -translate_by; };
	auto move_right = [&]() { translate_by = translate_by > 0 ? translate_by : -translate_by; };
	auto increase_speed = [&]() { translate_by += translate_by < 0 ? -default_translate : default_translate; };
	auto decrease_speed = [&]() { translate_by += translate_by < 0 ? default_translate : -default_translate; };

	auto zoom_in   = [&]() { camera_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.2)); };
	auto zoom_out  = [&]() { camera_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.2)); };
	auto zoom_stop = [&]() { camera_translate = glm::mat4(1.0f); };

	auto pan_down = [&]() { camera_pan = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.2, 0)); };
	auto pan_up   = [&]() { camera_pan = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.2, 0)); };
	auto pan_stop = [&]() { camera_pan = glm::mat4(1.0f); };

	auto strafe_left  = [&]() { camera_strafe = glm::translate(glm::mat4(1.0f), glm::vec3(0.2, 0, 0)); };
	auto strafe_right = [&]() { camera_strafe = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2, 0, 0)); };
	auto strafe_stop  = [&]() { camera_strafe = glm::mat4(1.0f); };


	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_R,     reset_truck);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_ENTER, toggle_move);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_SPACE, change_direction);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_LEFT,  move_left);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_RIGHT, move_right);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_UP,    increase_speed);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_DOWN,  decrease_speed);

	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_W,     pan_up);
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_S,     pan_down);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_W,     pan_stop);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_S,     pan_stop);

	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_A,     strafe_left);
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_D,     strafe_right);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_A,     strafe_stop);
	glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_D,     strafe_stop);

	// glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_UP,    zoom_in);
	// glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_DOWN,  zoom_out);
	// glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_UP,    zoom_stop);
	// glfw::add_key_callback(GLFW_RELEASE, GLFW_KEY_DOWN,  zoom_stop);

	glfw::add_key_callback(GLFW_PRESS, GLFW_KEY_UP,   increase_speed);
	glfw::add_key_callback(GLFW_PRESS, GLFW_KEY_DOWN, decrease_speed);


	// Lighting.
	scene.set_ambient_light_colour(glm::vec3(0.1, 0.1, 0.1));

	scene.add_point_source(
		glm::vec3(0, -10, 300),
		glm::vec3(1, 1, 1),
		70000.0f
	);

	scene.add_point_source(
		glm::vec3(0, -20, -10),
		glm::vec3(1, 1, 1),
		80.0f
	);

	glfw::add_key_callback(glfw::key_callbacks::quit_on_escape);

	gl::viewport main_vp(0, 0, 1920, 1080);

	window.main_loop(
		[&]()
		{

			main_view = camera_pan * camera_strafe * camera_translate * main_view;

			if (auto_move)
			{
				truck_tires.apply_fp_transformation(glm::rotate(glm::mat4(1.0f), translate_by, glm::vec3(0.0f, 1.0f, 0.0f)));
				truck.update_model(glm::translate(glm::mat4(1.0f), glm::vec3(translate_by, 0, 0)));
			}

			main_vp.render(scene, main_view, main_projection);
		}
	);
}
