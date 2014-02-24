#include <mrr/graphics/common.hxx>
using namespace mrr::graphics;

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "objects.hxx"

#include <iostream>

int main()
{
	glfw::init();

	glfw::window_handle window(1920, 1080, "03-60-352 Assignment 2");
	window.make_current();
	glew::init();

	glClearColor(0.0f, 0.0f, 0.0f, 0.2f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	gl::vertex_array vao;
	vao.bind();

	gl::shader_handle shader(
		"vertex-transform.glsl",
		"fragment-shader.glsl"
	);

	GLuint mvp_matrix_id = glGetUniformLocation(shader.get_program_id(), "MVP");

	glm::mat4 MVP;

	gl::buffer vertex_buffer;
	vertex_buffer.bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertex_data), square_vertex_data, GL_STATIC_DRAW);

	// Insert colour/texture code here.

	glm::mat4 projection = glm::perspective((float)M_PI/4.0f, 16.0f/9.0f, 0.1f, 100.0f);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 12),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);

	glm::mat4 cab_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.8f, 0.8f));
	glm::mat4 cab_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-2, -0.2, 0));
	glm::mat4 cab_model_original = cab_translation * cab_scale;
	glm::mat4 cab_model = cab_model_original;

	glm::mat4 trailer_scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	glm::mat4 trailer_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5, 0, 0));
	glm::mat4 trailer_model_original = trailer_translation * trailer_scale;
	glm::mat4 trailer_model = trailer_model_original;

	float default_translate = 0.01f;
	float translate_by = default_translate;
	bool auto_move = false;
	bool manual_move = false;

	float cameraZ_translate = 0;

	glfw::add_key_callback(glfw::key_callbacks::quit_on_escape);

	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Reset.
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_R,
		[&]() { trailer_model = trailer_model_original; }
	);

	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Truck movement.
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_ENTER,
		[&]() { auto_move = !auto_move; }
	);
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_SPACE,
		[&]() { translate_by = -translate_by; }
	);

	// Direction control.
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_LEFT,
		[&]() { translate_by = translate_by < 0 ? translate_by : -translate_by; }
	);
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_RIGHT,
		[&]() { translate_by = translate_by > 0 ? translate_by : -translate_by; }
	);

	// Speed modification.
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_UP,
		[&]() { translate_by += translate_by < 0 ? -default_translate : default_translate; }
	);
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_DOWN,
		[&]() { translate_by += translate_by < 0 ? default_translate : -default_translate; }
	);


	//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Camera movement.
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_W,
		[&]() { cameraZ_translate = 0.2; }
	);
	glfw::add_key_callback(
		GLFW_PRESS, GLFW_KEY_S,
		[&]() { cameraZ_translate = -0.2; }
	);

	window.set_framebuffer_size_callback(glfw::framebuffer_size_callback);

	while (!window.should_close())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = glm::translate(view, glm::vec3(0, 0, cameraZ_translate));
		cameraZ_translate = 0;

		if (auto_move || manual_move)
		{
			glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(translate_by, 0, 0));
			trailer_model = translation_matrix * trailer_model;
			cab_model = translation_matrix * cab_model;
			manual_move = false;
		}

		// Draw trailer.
		MVP = projection * view * trailer_model;
		shader.use();
		glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		vertex_buffer.bind(GL_ARRAY_BUFFER);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 12*3);

		// Draw cab.
		MVP = projection * view * cab_model;
		shader.use();
		glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, &MVP[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 12*3);

		glDisableVertexAttribArray(0);
		window.swap_buffers();
		glfwPollEvents();
	}
}
