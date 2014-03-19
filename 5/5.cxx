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
	glfw::window_handle window(1920, 1080, "03-60-352 Assignment 5");
	glew::init();
	gl::init(0.0f, 0.0f, 0.0f, 0.2f);
	gl::vertex_array vao;

	glm::mat4 projection = glm::perspective(
		(float)M_PI/4.0f,  // Horizontal field of view (angle of view).
		16.0f/9.0f,        // Aspect ratio.
		0.1f,              // Near clipping plane.
		200.0f             // Far clipping plane.
	);

	glm::mat4 projections = glm::perspective(
		(float)M_PI/4.0f,
		16.0f/9.0f,
		0.1f,
		1000.0f
	);

	std::vector<glm::mat4> camera_views = {
		glm::lookAt(glm::vec3(6.5, 10, 4.5),  glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
		glm::lookAt(glm::vec3(6.5, 10, -4.5),  glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
		glm::lookAt(glm::vec3(-6.5, 10, 4.5),  glm::vec3(0, 0, 0), glm::vec3(0, 14, 0)),
		glm::lookAt(glm::vec3(-6.5, 10, -4.5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))
	};

	glm::mat4 view = camera_views[0];

	auto colour_shader = gl::colour_shader();
	auto texture_shader = gl::texture_shader();

	std::vector<gl::component> walls(4);
	walls[0].set_shader(colour_shader);
	walls[0].load_wavefront("/home/matt/block.obj");
	walls[0].set_colour(glm::vec3(0.9, 0.2, 0.2));
	walls[0].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(7.0, 0.0, 0.0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 4.0, 5.0))
		* glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(1,0,0))
	);

	walls[1].set_shader(colour_shader);
	walls[1].load_wavefront("/home/matt/block.obj");
	walls[1].set_colour(glm::vec3(0.9, 0.2, 0.2));
	walls[1].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(-7.0, 0.0, 0.0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 4.0, 5.0))
		* glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(1,0,0))
	);

	walls[2].set_shader(colour_shader);
	walls[2].load_wavefront("/home/matt/block.obj");
	walls[2].set_colour(glm::vec3(0.9, 0.2, 0.2));
	walls[2].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(7.0, 1.0, -5.0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(7.0, 4.0, 1.0))
		* glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(0,1,0))
	);

	walls[3].set_shader(colour_shader);
	walls[3].load_wavefront("/home/matt/block.obj");
	walls[3].set_colour(glm::vec3(0.9, 0.2, 0.2));
	walls[3].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(7.0, 1.0, 5.0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(7.0, 4.0, 1.0))
		* glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(0,1,0))
	);

	gl::component floor;
	floor.set_shader(colour_shader);
	floor.load_wavefront("/home/matt/floor2.obj");
	floor.set_colour(glm::vec3(0.4, 0.4, 0.4));
	// floor.set_model(glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(1,0,0)));

	std::vector<gl::component> chairs(5);
	for (gl::component& c : chairs)
	{
		c.set_shader(colour_shader);
		c.load_wavefront("/home/matt/cube.obj");
		c.set_colour(glm::vec3(0.0, 0.7, 0.0));
	}

	chairs[0].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f))
	);

	chairs[1].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f))
	);

	chairs[2].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, -3.0f))
	);

	chairs[3].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, -2.0f))
	);

	chairs[4].set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 1.0f))
	);

	// gl::component chair;
	// chair.set_shader(colour_shader);
	// chair.load_wavefront("/home/matt/Chair.obj");
	// chair.set_colour(glm::vec3(0.0, 1.0, 0.0));
	// chair.set_model(
	//	// glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 1.0, 1.0))
	//	// * glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5))
	//	glm::rotate(glm::mat4(1.0f), (float)M_PI, glm::vec3(0,1,0))
	// );

	gl::component table;
	table.set_shader(colour_shader);
	table.load_wavefront("/home/matt/block.obj");
	table.set_colour(glm::vec3(0.8, 0.7, 0.7));
	table.set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 2.0, 1.0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(3.0, 1.0, 3.0))
		* glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(0,0,1))
	);

	gl::component cylinder;
	cylinder.set_shader(colour_shader);
	cylinder.load_wavefront("/home/matt/cylinder.obj");
	cylinder.set_colour(glm::vec3(0, 0, 1));
	cylinder.set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 3.0, -3.0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5))
	);

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Scene.
	gl::model scene;
	scene.add_component(floor);
	scene.add_components(walls.begin(), walls.end());
	scene.add_component(table);
	scene.add_component(cylinder);
	scene.add_components(chairs.begin(), chairs.end());

	// Lighting
	scene.set_ambient_light_colour(glm::vec3(0.2, 0.2, 0.2));

	scene.add_point_source(
		"one",
		glm::vec3(5, 5, 2.0),
		glm::vec3(1, 1, 1),
		0.0f
	);

	scene.add_point_source(
		"two",
		glm::vec3(5, 5, -4.0),
		glm::vec3(1, 1, 1),
		0.0f
	);

	scene.add_point_source(
		"three",
		glm::vec3(-5, 5, 4.0),
		glm::vec3(1, 1, 1),
		0.0f
	);

	scene.add_point_source(
		"four",
		glm::vec3(-5, 5, -4.0),
		glm::vec3(1, 1, 1),
		0.0f
	);

	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Key callbacks.
	glfw::add_key_callback(glfw::key_callbacks::quit_on_escape);

	std::vector<int> light_enabled = {false, false, false, false};

	auto toggle_light = [&](int num, std::string const& label) {
		if (!light_enabled[num])
			scene.set_point_source_power(label, 30.0f);
		else
			scene.set_point_source_power(label, 0.0f);

		light_enabled[num] = !light_enabled[num];
	};

	auto set_camera = [&](int num) { view = camera_views[num]; };

	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_1,     std::bind(toggle_light, 0, "one"));
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_2,     std::bind(toggle_light, 1, "two"));
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_3,     std::bind(toggle_light, 2, "three"));
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_4,     std::bind(toggle_light, 3, "four"));

	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_7,     std::bind(set_camera, 0));
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_8,     std::bind(set_camera, 1));
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_9,     std::bind(set_camera, 2));
	glfw::add_key_callback(GLFW_PRESS,   GLFW_KEY_0,     std::bind(set_camera, 3));

	window.main_loop(
		[&]()
		{
			::glViewport(0, 0, 1920, 1080);
			scene.render(view, projection);
		}
	);
}
