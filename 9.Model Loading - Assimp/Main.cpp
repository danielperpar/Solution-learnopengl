#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "stb_image.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

float delta_time = 0.0f;
float last_frame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool first_mouse = true;
float lastX = SCREEN_WIDTH/2, lastY = SCREEN_HEIGHT/2;

//cube vertices
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
	glm::vec3(1.0f, 0.3f, 0.3f),
	glm::vec3(0.3f, 1.0f, 0.3f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 0.0f)
};

unsigned int num_point_lights = 2;

glm::vec3 light_direction(-0.2f, -1.0f, -0.3f);

glm::vec3 dirLightColor(0.2f, 0.2f, 0.2f);

glm::vec4 clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

float pl_amb = 1.0f;

std::string model_path = "resources/objects/nanosuit/nanosuit.obj";

const float PI = glm::pi<float>();

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// creating vertex array object and buffers for the lamp
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	const char *vertex_path = "resources/shaders/lamp.vert";
	const char* fragment_path = "resources/shaders/lamp.frag";

	Shader lamp_shader(vertex_path, fragment_path);

	vertex_path = "resources/shaders/1.model_loading.vert";
	fragment_path = "resources/shaders/1.model_loading.frag";

	Shader model_shader(vertex_path, fragment_path);

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 normal_matrix;

	Model nanosuit_model(model_path);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		//input 
		processInput(window);

		//rendering commands here
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		model_shader.Use();
		
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		model_shader.SetMat4("projection", projection);
		model_shader.SetMat4("view", view);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model_shader.SetMat4("model", model);

		normal_matrix = glm::inverse(model);
		normal_matrix = glm::transpose(normal_matrix);
		model_shader.SetMat4("normalMatrix", normal_matrix);
		
		model_shader.SetFloat("material.shininess", 32.0f);
		model_shader.SetVec3("viewPos", camera.Position);
		
		glm::mat4 lamp_model;
		lamp_model = glm::rotate(lamp_model, PI * glm::sin((float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));

		for (unsigned int i = 0; i < num_point_lights; i++)
		{
			std::string position = std::string("pointLight") + std::to_string(i+1) + std::string(".position");
			std::string ambient = std::string("pointLight") + std::to_string(i+1) + std::string(".ambient");
			std::string diffuse = std::string("pointLight") + std::to_string(i+1) + std::string(".diffuse");
			std::string specular = std::string("pointLight") + std::to_string(i+1) + std::string(".specular");
			std::string constant = std::string("pointLight") + std::to_string(i+1) + std::string(".constant");
			std::string linear = std::string("pointLight") + std::to_string(i+1) + std::string(".linear");
			std::string quadratic = std::string("pointLight") + std::to_string(i+1) + std::string(".quadratic");

			glm::mat4 lm = glm::translate(lamp_model, pointLightPositions[i]);
			glm::vec3 new_position = lamp_model * glm::vec4(pointLightPositions[i], 1.0f);

			model_shader.SetVec3(position.data(), new_position);
			model_shader.SetVec3(ambient.data(), pointLightColors[i]);
			model_shader.SetVec3(diffuse.data(), pointLightColors[i]);
			model_shader.SetVec3(specular.data(), pointLightColors[i]);
			model_shader.SetFloat(constant.data(), 1.0f);
			model_shader.SetFloat(linear.data(), 0.09f);
			model_shader.SetFloat(quadratic.data(), 0.032f);
		}

		nanosuit_model.Draw(model_shader);
		
		//Draw lamps
		lamp_shader.Use();
		for (unsigned int i = 0; i < num_point_lights; i++)
		{
			glm::mat4 lm;
			lm = glm::translate(lamp_model, pointLightPositions[i]);
			lm = glm::scale(lm, glm::vec3(0.2f, 0.2f, 0.2f));
			lamp_shader.SetMat4("model", lm);
			lamp_shader.SetMat4("view", view);
			lamp_shader.SetMat4("projection", projection);
			lamp_shader.SetVec3("lightColor", pointLightColors[i]);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (first_mouse)
	{
		lastX = xpos;
		lastY = ypos;
		first_mouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window)
{
	
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, delta_time);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, delta_time);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, delta_time);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, delta_time);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		//Desert environment
		clear_color = glm::vec4(0.8f, 0.5f, 0.3f, 1.0f);
		
		pl_amb = 1.0f;

		pointLightColors[0] = glm::vec3(1.0f, 0.0f, 0.0f);
		pointLightColors[1] = glm::vec3(0.0f, 1.0f, 0.0f);
		pointLightColors[2] = glm::vec3(0.0f, 0.0f, 1.0f);
		pointLightColors[3] = glm::vec3(1.0f, 0.0f, 0.0f);

		dirLightColor = glm::vec3(0.2f, 0.2f, 0.2f);
			
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		//Factory environment
		clear_color = glm::vec4(0.09f, 0.09f, 0.09f, 1.0f);
		
		pl_amb = 1.0f;

		pointLightColors[0] = glm::vec3(0.2f, 0.2f, 0.7f);
		pointLightColors[1] = glm::vec3(0.2f, 0.2f, 0.7f);
		pointLightColors[2] = glm::vec3(0.2f, 0.2f, 0.7f);
		pointLightColors[3] = glm::vec3(0.2f, 0.2f, 0.7f);

		dirLightColor = glm::vec3(0.05f, 0.05f, 0.05f);
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		//Horror environment
		clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		pl_amb = 0.5f;

		pointLightColors[0] = glm::vec3(0.1f, 0.0f, 0.0f);
		pointLightColors[1] = glm::vec3(0.0f, 0.3f, 0.0f);
		pointLightColors[2] = glm::vec3(0.0f, 0.0f, 0.1f);
		pointLightColors[3] = glm::vec3(0.3f, 0.0f, 0.0f);

		dirLightColor = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		//Biochemical lab environment
		clear_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		pl_amb = 1.0f;

		pointLightColors[0] = glm::vec3(0.5f, 1.0f, 0.5f);
		pointLightColors[1] = glm::vec3(0.5f, 1.0f, 0.5f);
		pointLightColors[2] = glm::vec3(0.5f, 1.0f, 0.5f);
		pointLightColors[3] = glm::vec3(0.5f, 1.0f, 0.5f);

		dirLightColor = glm::vec3(0.4f, 0.4f, 0.4f);
	}

}


