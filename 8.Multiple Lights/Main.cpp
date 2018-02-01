#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "stb_image.h"

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

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 0.0f)
};

glm::vec3 light_direction(-0.2f, -1.0f, -0.3f);

glm::vec3 dirLightColor(0.2f, 0.2f, 0.2f);

glm::vec4 clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

float pl_amb = 1.0f;

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

	//loading texture
	unsigned int diffuse_map = loadTexture("textures/container2.png");
	unsigned int specular_map = loadTexture("textures/container2_specular.png");

	// creating vertex array object and buffers
	unsigned int VAO[2], VBO;
	glGenVertexArrays(2, VAO);
	glGenBuffers(1, &VBO);

	//cube object
	glBindVertexArray(VAO[0]);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	//normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//lamp
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);


	//const char *vertex_path = "multiple_lights_solution.vert";
	//const char *fragment_path = "multiple_lights_solution.frag";
	const char *vertex_path = "multiple_lights.vert";
	const char *fragment_path = "multiple_lights.frag";

	Shader model_shader(vertex_path, fragment_path);

	vertex_path = "lamp_vshader.vert";
	fragment_path = "lamp_fshader.frag";

	Shader lamp_shader(vertex_path, fragment_path);

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 normal_matrix;

	const float PI = glm::pi<float>();

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input 
		processInput(window);

		//rendering commands here
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		model_shader.Use();

		view = camera.GetViewMatrix();
		model_shader.SetMat4("view", view);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.0f);
		model_shader.SetMat4("projection", projection);
		model_shader.SetVec3("viewPos", camera.Position);
		
		model_shader.SetInt("material.diffuse", 0);
		model_shader.SetInt("material.specular", 1);
		model_shader.SetFloat("material.shininess", 64.0f);
			
		
		//directonal light
		model_shader.SetVec3("dirLight.direction", light_direction);
		model_shader.SetVec3("dirLight.ambient", dirLightColor);
		model_shader.SetVec3("dirLight.diffuse", dirLightColor);
		model_shader.SetVec3("dirLight.specular", dirLightColor);
		
		//point light 1 red
		model_shader.SetVec3("pointLights[0].position", pointLightPositions[0]);
		model_shader.SetVec3("pointLights[0].ambient", pl_amb * pointLightColors[0]);
		model_shader.SetVec3("pointLights[0].diffuse", pointLightColors[0]);
		model_shader.SetVec3("pointLights[0].specular", pointLightColors[0]);
		model_shader.SetFloat("pointLights[0].constant", 1.0f);
		model_shader.SetFloat("pointLights[0].linear", 0.09f);
		model_shader.SetFloat("pointLights[0].quadratic", 0.032f);

		//point light 2 green
		model_shader.SetVec3("pointLights[1].position", pointLightPositions[1]);
		model_shader.SetVec3("pointLights[1].ambient", pl_amb * pointLightColors[1]);
		model_shader.SetVec3("pointLights[1].diffuse", pointLightColors[1]);
		model_shader.SetVec3("pointLights[1].specular", pointLightColors[1]);
		model_shader.SetFloat("pointLights[1].constant", 1.0f);
		model_shader.SetFloat("pointLights[1].linear", 0.09f);
		model_shader.SetFloat("pointLights[1].quadratic", 0.032f);

		//point light 3 blue
		model_shader.SetVec3("pointLights[2].position", pointLightPositions[2]);
		model_shader.SetVec3("pointLights[2].ambient", pl_amb * pointLightColors[2]);
		model_shader.SetVec3("pointLights[2].diffuse", pointLightColors[2]);
		model_shader.SetVec3("pointLights[2].specular", pointLightColors[2]);
		model_shader.SetFloat("pointLights[2].constant", 1.0f);
		model_shader.SetFloat("pointLights[2].linear", 0.09f);
		model_shader.SetFloat("pointLights[2].quadratic", 0.032f);

		//point light 4 red
		model_shader.SetVec3("pointLights[3].position", pointLightPositions[3]);
		model_shader.SetVec3("pointLights[3].ambient", pl_amb * pointLightColors[3]);
		model_shader.SetVec3("pointLights[3].diffuse", pointLightColors[3]);
		model_shader.SetVec3("pointLights[3].specular", pointLightColors[3]);
		model_shader.SetFloat("pointLights[3].constant", 1.0f);
		model_shader.SetFloat("pointLights[3].linear", 0.09f);
		model_shader.SetFloat("pointLights[3].quadratic", 0.032f);

		//spot light
		model_shader.SetVec3("spotLight.position", camera.Position);
		model_shader.SetVec3("spotLight.direction", camera.Front);
		model_shader.SetVec3("spotLight.ambient", 0.5f, 0.5f, 0.5f);
		model_shader.SetVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
		model_shader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);		
		model_shader.SetFloat("spotLight.constant", 1.0f);
		model_shader.SetFloat("spotLight.linear", 0.09f);
		model_shader.SetFloat("spotLight.quadratic", 0.032f);
		model_shader.SetFloat("spotLight.cutoff", glm::cos(glm::radians(10.0f)));
		model_shader.SetFloat("spotLight.outerCutoff", glm::cos(glm::radians(12.f)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_map);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular_map);

		glBindVertexArray(VAO[0]);

		for (int i = 0; i < 10; i++)
		{		
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model_shader.SetMat4("model", model);
		
			normal_matrix = glm::transpose(glm::inverse(model));
			model_shader.SetMat4("normalMatrix", normal_matrix);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		
		lamp_shader.Use();
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 lamp_model;
			lamp_model = glm::translate(lamp_model, pointLightPositions[i]);
			lamp_model = glm::scale(lamp_model, glm::vec3(0.2f, 0.2f, 0.2f));
			lamp_shader.SetMat4("model", lamp_model);
			lamp_shader.SetMat4("view", view);
			lamp_shader.SetMat4("projection", projection);
			lamp_shader.SetVec3("lightColor", pointLightColors[i]);

			glBindVertexArray(VAO[1]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
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
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;

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

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


