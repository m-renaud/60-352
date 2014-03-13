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

	glm::vec3 camera_location(0, 0, 30);

	glm::mat4 view = glm::lookAt(
		camera_location,      // Camera location.
		glm::vec3(0, 0, 0),   // Looks at the origin.
		glm::vec3(0, 1, 0)    // Head is facing up.
	);

	auto colour_shader = gl::colour_shader();
	auto texture_shader = gl::texture_shader();

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


	window.main_loop(
		[&]()
		{
		}
	);
}
