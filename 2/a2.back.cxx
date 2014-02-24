#include <mrr/graphics/common.hxx>
#include <mrr/graphics/models/cube.hxx>
#include <mrr/graphics/models/square.hxx>
//#include <mrr/graphics/models/tire.hxx>
using namespace mrr::graphics;

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

int main()
{
	glfw::init();

	glfw::window_handle window(1920, 1080, "03-60-352 Assignment 2");
	window.make_current();
	glew::init();

	glClearColor(0.0f, 0.2f, 0.0f, 0.2f);

	gl::init();

	gl::vertex_array vao;
	vao.bind();

	glm::mat4 projection = glm::perspective(
		(float)M_PI/4.0f,  // Horizontal field of view (angle of view).
		16.0f/9.0f,        // Aspect ratio.
		0.1f,              // Near clipping plane.
		100.0f             // Far clipping plane.
	);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 4, 12),  // Camera location.
		glm::vec3(0, 0, 0),   // Looks at the origin.
		glm::vec3(0, 1, 0)    // Head is facing up.
	);

	gl::shader_handle colour_shader(
		"shaders/single-colour-vertex.glsl",
		"shaders/single-colour-fragment-shader.glsl"
	);

	// Create road.
	gl::component road;
	//	road.set_name("road");
	road.set_shader(colour_shader);
	road.set_colour(glm::vec3(0.2f, 0.2f, 0.2f));
	road.set_vertex_data(square_vertex_data, sizeof(square_vertex_data));
	glm::mat4 road_scale = glm::scale(glm::mat4(1.0f), glm::vec3(20.f, 0.f, 2.0f));
	glm::mat4 road_translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.8f, 0.0f));
	road.set_model(road_translation * road_scale);

	// Create model.
	gl::component cab;
	//	cab.set_name("cab");
	cab.set_shader(colour_shader);
	cab.set_colour(glm::vec3(1.0f, 0.0f, 0.0f));
	cab.set_vertex_data(cube_vertex_data, sizeof(cube_vertex_data));
	glm::mat4 cab_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.8f, 0.8f));
	glm::mat4 cab_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-2, -0.2, 0));
	cab.set_model(cab_translation * cab_scale);

	gl::component trailer;
	//	trailer.set_name("trailer");
	trailer.set_shader(colour_shader);
	trailer.set_colour(glm::vec3(0.0f, 0.0f, 0.4f));
	trailer.set_vertex_data(cube_vertex_data, sizeof(cube_vertex_data));
	glm::mat4 trailer_scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	glm::mat4 trailer_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5, 0, 0));
	trailer.set_model(trailer_translation * trailer_scale);

	gl::component hitch;
	//	hitch.set_name("hitch");
	hitch.set_shader(colour_shader);
	hitch.set_colour(glm::vec3(0.0f, 0.0f, 0.0f));
	hitch.set_vertex_data(cube_vertex_data, sizeof(cube_vertex_data));
	glm::mat4 hitch_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.1f, 0.1f));
	glm::mat4 hitch_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8, -0.9, 0));
	hitch.set_model(hitch_translation * hitch_scale);

	// gl::component tire;
	// //	tire.set_name("tire");
	// tire.set_shader(colour_shader);
	// tire.set_colour(glm::vec3(0.0f, 0.0f, 0.0f));
	// tire.set_vertex_data(tire_vertex_data, sizeof(tire_vertex_data));
	// glm::mat4 tire_scale = glm::mat4(3.0f);
	// glm::mat4 tire_translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 1.0, 0.0));
	// tire.set_model(tire_translation* tire_scale);


	gl::model truck;
	truck.add_component(cab);
	truck.add_component(trailer);
	truck.add_component(hitch);
	// truck.add_component(tire);
	truck.save();

	gl::model scene;
	scene.add_component(truck);
	scene.add_component(road);

	float default_translate = 0.01f;
	float translate_by = default_translate;
	bool auto_move = false;

	glfw::add_key_callback(glfw::key_callbacks::quit_on_escape);

	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Key callbacks.
	auto reset_truck = [&]() { truck.reset(); };
	auto toggle_move = [&]() { auto_move = !auto_move; };
	auto change_direction = [&]() { translate_by = -translate_by; };
	auto move_left = [&]() { translate_by = translate_by < 0 ? translate_by : -translate_by; };
	auto move_right = [&]() { translate_by = translate_by > 0 ? translate_by : -translate_by; };
	auto increase_speed = [&]() { translate_by += translate_by < 0 ? -default_translate : default_translate; };
	auto decrease_speed = [&]() { translate_by += translate_by < 0 ? default_translate : -default_translate; };
	auto zoom_in = [&]() { view = glm::translate(view, glm::vec3(0, 0, 0.2)); };
	auto zoom_out = [&]() { view = glm::translate(view, glm::vec3(0, 0, -0.2)); };

	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_R,     reset_truck);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_ENTER, toggle_move);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_SPACE, change_direction);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_LEFT,  move_left);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_RIGHT, move_right);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_UP,    increase_speed);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_DOWN,  decrease_speed);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_W,     zoom_in);
	glfw::add_key_callback(GLFW_REPEAT, GLFW_KEY_W,     zoom_in);
	glfw::add_key_callback(GLFW_PRESS,  GLFW_KEY_S,     zoom_out);
	glfw::add_key_callback(GLFW_REPEAT, GLFW_KEY_S,     zoom_out);

	window.set_framebuffer_size_callback(glfw::framebuffer_size_callback);

	window.main_loop(
		[&]()
		{
			if (auto_move)
				truck.update_model(glm::translate(glm::mat4(1.0f), glm::vec3(translate_by, 0, 0)));

			scene.render(projection * view);
		}
	);
}
