#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void RenderTriangle();
void RenderCube();
const std::string ReadFile(const std::string &file);
void CheckCompileStatus(GLuint shader);
void CheckLinkStatus(GLuint program);

const GLuint WIDTH = 800, HEIGHT = 600;
const GLfloat FAR_PLANE = 100.0f, NEAR_PLANE = 0.1f;

glm::mat4 rotViewSpace(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

glm::mat4 translation(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -5.0f, 1.0f
);

glm::mat4 model(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);


float previousTime = 0.0f;
float currentTime = 0.0f;
float deltaTime = 0.0f;
glm::vec3 camPos(0.0f, 0.0f, -5.0f);
glm::vec3 front(0.0f, 0.0f, 1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 right(1.0f, 0.0f, 0.0f);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "learnopengl.com", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	glDisable(GL_CULL_FACE);

	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, NEAR_PLANE, FAR_PLANE);
	//Shaders
	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string shader = ReadFile("resources/shaders/triangle.vs");
	const char *shaderData = shader.c_str();
	glShaderSource(vertexShader, 1, &shaderData, NULL);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shader = ReadFile("resources/shaders/triangle.fs");
	shaderData = shader.c_str();
	glShaderSource(fragmentShader, 1, &shaderData, NULL);

	shader.clear();
	glCompileShader(vertexShader);
	CheckCompileStatus(vertexShader);

	glCompileShader(fragmentShader);
	CheckCompileStatus(fragmentShader);

	GLuint program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	CheckLinkStatus(program);

	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		glfwPollEvents();

		//Render
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//glm::mat4 view = rotViewSpace * translation;
		glm::mat4 view = glm::lookAt(camPos, camPos + front, up);

		//Set uniforms
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);

		RenderTriangle();
		
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
float speed = 200.0f;
/*
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
	{
		glm::vec4 camPos = translation[3];
		camPos.x += speed * deltaTime * rotViewSpace[0][2];
		camPos.y += speed * deltaTime * rotViewSpace[1][2];
		camPos.z += speed * deltaTime * rotViewSpace[2][2];
		translation[3] = camPos;
		
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
	{
		glm::vec4 camPos = translation[3];
		camPos.x -= speed * deltaTime * rotViewSpace[0][2];
		camPos.y -= speed * deltaTime * rotViewSpace[1][2];
		camPos.z -= speed * deltaTime * rotViewSpace[2][2];
		translation[3] = camPos;
	}
	
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
	{
		glm::vec4 camPos = translation[3];
		camPos.x += speed * deltaTime * rotViewSpace[0][0];
		camPos.y += speed * deltaTime * rotViewSpace[1][0];
		camPos.z += speed * deltaTime * rotViewSpace[2][0];
		translation[3] = camPos;
	}
	
	
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
	{
		glm::vec4 camPos = translation[3];
		camPos.x -= speed * deltaTime * rotViewSpace[0][0];
		camPos.y -= speed * deltaTime * rotViewSpace[1][0];
		camPos.z -= speed * deltaTime * rotViewSpace[2][0];
		translation[3] = camPos;
	}
	
}
*/
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
	{
		camPos += (front * speed * deltaTime);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
	{
		camPos -= (front * speed * deltaTime);
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
	{
		camPos -= (right * speed * deltaTime);
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
	{
		camPos += (right * speed * deltaTime);
	}

	//---------------
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		std::cout << "cam pos=(" << camPos.x << "," << camPos.y << "," << camPos.z << ")" << std::endl;
		std::cout << "front=(" << front.x << "," << front.y << "," << front.z << ")" "up=(" << up.x << "," << up.y << "," << up.z << ") right=(" << right.x << "," << right.y << "," << right.z << ")" << std::endl;
	}
}

float sensitivity = 0.05f;
float prevXPos = WIDTH / 2;
float currXPos = WIDTH / 2;
float prevYPos = HEIGHT / 2;
float currYPos = HEIGHT / 2;
float deltaXPos;
float deltaYPos;
/*
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Horizontal movement
	currXPos = xpos;
	deltaXPos += (currXPos - prevXPos);
	prevXPos = currXPos;

	float angleYaw = deltaXPos * sensitivity;
	if (angleYaw > 90.0f)
		angleYaw = 90.0f;
	if (angleYaw < -90.0f)
		angleYaw = -90.0f;

	std::cout << "yaw=" << angleYaw << std::endl;
	
	float xx = glm::cos((float)glm::radians(angleYaw));
	float xz = -glm::sin((float)glm::radians(angleYaw));
	glm::vec4 xYaw = glm::vec4(xx, 0.0f, xz, 0.0f);

	glm::vec4 yYaw = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	float zx = glm::sin((float)glm::radians(angleYaw));
	float zz = glm::cos((float)glm::radians(angleYaw));
	glm::vec4 zYaw = glm::vec4(zx, 0.0f, zz, 0.0f);

	glm::mat4 rotYaw = glm::mat4(xYaw, yYaw, zYaw, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Vertical movement
	currYPos = ypos;
	deltaYPos += (currYPos - prevYPos);
	prevYPos = currYPos;

	float anglePitch = deltaYPos * sensitivity;
	if (anglePitch > 90.0f)
		anglePitch = 90.0f;
	if (anglePitch < -90.0f)
		anglePitch = -90.0f;

	std::cout << "pitch=" << anglePitch << std::endl;

	glm::vec4 xPitch = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	
	float yy = glm::cos((float)glm::radians(anglePitch));
	float yz = glm::sin((float)glm::radians(anglePitch));
	glm::vec4 yPitch = glm::vec4(0.0f, yy, yz, 0.0f);

	float zy = -glm::sin((float)glm::radians(anglePitch));
	zz = glm::cos((float)glm::radians(anglePitch));
	glm::vec4 zPitch = glm::vec4(0.0f, zy, zz, 0.0f);

	glm::mat4 rotPitch = glm::mat4(xPitch, yPitch, zPitch, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	rotViewSpace = rotYaw * rotPitch; //revisar, va mal cuando se combina pitch + yaw
}
*/

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Horizontal movement
	currXPos = xpos;
	deltaXPos += (currXPos - prevXPos);
	prevXPos = currXPos;

	float angleYaw = -deltaXPos * sensitivity;
	if (angleYaw > 90.0f)
		angleYaw = 90.0f;
	if (angleYaw < -90.0f)
		angleYaw = -90.0f;
	std::cout << "yaw=" << angleYaw << std::endl;
	
	//Vertical movement
	currYPos = ypos;
	deltaYPos += (currYPos - prevYPos);
	prevYPos = currYPos;

	float anglePitch = -deltaYPos * sensitivity;
	if (anglePitch > 90.0f)
		anglePitch = 90.0f;
	if (anglePitch < -90.0f)
		anglePitch = -90.0f;
	std::cout << "pitch=" << anglePitch << std::endl;
	
	front.x = glm::cos(glm::radians(anglePitch)) * glm::sin(glm::radians(angleYaw));
	front.y = glm::sin(glm::radians(anglePitch));
	front.z = glm::cos(glm::radians(anglePitch)) * glm::cos(glm::radians(angleYaw));
	
	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
	up = glm::cross(front, right);
}


GLuint VAO = 0;
GLuint VBO = 0;
void RenderCube()
{
	if (!VAO)
	{
		GLfloat vertex_buffer_data[] = {
			-1.0f,-1.0f,-1.0f, // triángulo 1 : comienza
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triángulo 1 : termina
			1.0f, 1.0f,-1.0f, // triángulo 2 : comienza
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f, // triángulo 2 : termina
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3 * sizeof(float), GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RenderTriangle()
{
	if (VAO == 0)
	{
		GLfloat vertex_buffer_data[] = {
			-1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

const std::string ReadFile(const std::string &file)
{
	std::ifstream ifs(file);
	if (!ifs.good())
		std::cout << "Error reading shader file: " << file << std::endl;

	std::stringstream buffer;
	buffer << ifs.rdbuf();
	ifs.close();

	return buffer.str();
}

void CheckCompileStatus(GLuint shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint infoBuffSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoBuffSize);

		char *info = new char[infoBuffSize];
		glGetShaderInfoLog(shader, infoBuffSize, NULL, info);
		
		GLint shaderType;
		glGetShaderiv(shader, GL_SHADER_TYPE, &shaderType);
		std::cout << info << std::endl;
		delete[] info;
	}	
}

void CheckLinkStatus(GLuint program)
{
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint infoBuffSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoBuffSize);

		char *info = new char[infoBuffSize];
		glGetProgramInfoLog(program, infoBuffSize, NULL, info);

		GLint shaderType;
		glGetProgramiv(program, GL_SHADER_TYPE, &shaderType);
		std::cout << info << std::endl;

		delete[] info;
	}
}