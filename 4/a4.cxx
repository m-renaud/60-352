#include <mrr/graphics/common.hxx>
#include <mrr/graphics/models/cube.hxx>
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

	glm::vec3 camera_location(0, 0, 30);

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
	road.load_texture("brick.DDS");
	road.set_model(
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(8.0, 8.0, 1.0)) *
		glm::rotate(glm::mat4(1.0f), (float)M_PI/2, glm::vec3(1, 0, 0))
	);

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Scene.
	gl::model scene;

	scene.add_component(road);

	// Lighting.
	scene.set_ambient_light_colour(glm::vec3(0.1, 0.1, 0.1));

	glfw::add_key_callback(glfw::key_callbacks::quit_on_escape);

	gl::viewport main_vp(0, 0, 1920, 1080);

	window.main_loop(
		[&]()
		{
			main_vp.render(scene, main_view, main_projection);
		}
	);
}
