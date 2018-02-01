#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//circle: 10 points + center
	const unsigned int num_points = 5;
	const unsigned int num_vertices = num_points * 3;
	const unsigned int num_triangles = num_points - 1;
	const unsigned int num_indices = num_triangles * 3;
	

	float vertices[num_vertices];
	unsigned int indices[num_indices];
	
	const float pi = 3.141592;
	float angle_inc = 2 * pi/(float)num_triangles;
	float radius = 1.0f;

	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = 0.0f;

	std::cout << vertices[0] << ", ";
	std::cout << vertices[1] << ", ";
	std::cout << vertices[2] << ", ";

	for (int i = 1; i <= num_triangles; i++)
	{
		for (int j = 3 * i; j < 3 * i + 3; j++)
		{
			if (j % (3 * i) == 0)
			{
				vertices[j] = radius * glm::cos((i - 1) * angle_inc);
				std::cout << vertices[j] << ", ";
			}
			else if (j % (3 * i) == 1)
			{
				vertices[j] = radius * glm::sin((i - 1) * angle_inc);
				std::cout << vertices[j] << ", ";
			}
			else if (j % (3 * i) == 2)
			{
				vertices[j] = 0.0f;
				std::cout << vertices[j] << ", ";
			}
		}
	}

	std::cout << std::endl;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	std::cout << indices[0] << ", ";
	std::cout << indices[1] << ", ";
	std::cout << indices[2] << ", ";

	for (int i = 1; i < num_triangles; i++)
	{
		for (int j = 3 * i; j < 3 * i + 3; j++)
		{		
			if (i != num_triangles - 1)
			{
				if (j % (3 * i) == 0)	
					indices[j] = 0;
				else 
					indices[j] = i + j % (3 * i) ;		
			}		
			else
			{
				if (j % (3 * i) == 0)
				{
					indices[j] = 0;
				}
				else if (j % (3 * i) == 1)
				{
					indices[j] = i + 1;
				}
				else if (j % (3 * i) == 2)
				{
					indices[j] = 1;
				}
			}

			std::cout << indices[j] << ", ";
		}

		
	}

	

	// creating vertex array object and buffers
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);	

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);


	const char *vertex_path = "triangle.vert";
	const char *fragment_path = "triangle.frag";

	Shader shader(vertex_path, fragment_path);
	shader.Use();
	
	
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input 
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, num_triangles, GL_UNSIGNED_INT, (void*)0);
		
		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


