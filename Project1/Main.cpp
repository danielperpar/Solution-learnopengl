#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

#include <cerrno>
#include <cstring>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
const GLchar* LoadShaderFile(const char *file);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
/*
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
*/
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

	//triangle vertices
	/*float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};*/

	//square made out of two triangles
	//float vertices[] = {
	//	0.5f,  0.5f, 0.0f,  // top right
	//	0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
	//};
	//unsigned int indices[] = {  // note that we start from 0!
	//	0, 1, 3,   // first triangle
	//	1, 2, 3    // second triangle
	//};

	//two triangles
	float vertices1[] = {
		0.0f, 0.0f, 0.0f,  //center
		-0.5f, 0.5f, 0.0f,  //top left
		-0.5f, 0.0f, 0.0f,  //bottom left		
	};

	float vertices2[] = {
		0.0f, 0.0f, 0.0f,  //center
		0.5f, -0.5f, 0.0f,  //bottom right
		0.5f, 0.0f, 0.0f  //top right
	};

	unsigned int indices[] = {
		0, 1, 2	
	};

	

	// creating vertex array object and buffers
	unsigned int VAOs[2], VBOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);

	glBindVertexArray(VAOs[0]);	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// creating vertex shader and compilating it
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *shader_code = LoadShaderFile("triangle.vert");
	
	if (shader_code == nullptr)
	{
		std::cout << "error in vertex shader loading" << std::endl;
		return -1;
	}

	glShaderSource(vertex_shader, 1, &shader_code, NULL);
	delete shader_code;

	glCompileShader(vertex_shader);
	
	int success;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetShaderInfoLog(vertex_shader, 100, NULL, info_log);
		std::cout << "error vertex shader compilation: " << info_log << std::endl;
		return -1;
	}
	//creating fragment shader and compilating it
	unsigned int fragment_shaders[2];
	fragment_shaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	fragment_shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);

	shader_code = LoadShaderFile("triangle.frag");
	
	if (shader_code == nullptr)
	{
		std::cout << "error in fragment shader loading" << std::endl;
		return -1;
	}

	glShaderSource(fragment_shaders[0], 1, &shader_code, NULL);
	delete shader_code;

	shader_code = LoadShaderFile("triangle_yellow.frag");

	if (shader_code == nullptr)
	{
		std::cout << "error in fragment shader loading" << std::endl;
		return -1;
	}

	glShaderSource(fragment_shaders[1], 1, &shader_code, NULL);
	delete shader_code;

	glCompileShader(fragment_shaders[0]);
	glGetShaderiv(fragment_shaders[0], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetShaderInfoLog(vertex_shader, 100, NULL, info_log);
		std::cout << "error fragment shader compilation: " << info_log << std::endl;
		return -1;
	}

	glCompileShader(fragment_shaders[1]);
	glGetShaderiv(fragment_shaders[1], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetShaderInfoLog(vertex_shader, 100, NULL, info_log);
		std::cout << "error fragment shader compilation: " << info_log << std::endl;
		return -1;
	}

	//creating program
	unsigned int shader_programs[2];
	shader_programs[0] = glCreateProgram();
	shader_programs[1] = glCreateProgram();

	glAttachShader(shader_programs[0], vertex_shader);
	glAttachShader(shader_programs[0], fragment_shaders[0]);
	glLinkProgram(shader_programs[0]);

	glGetProgramiv(shader_programs[0], GL_LINK_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetProgramInfoLog(shader_programs[0], 512, NULL, info_log);
		std::cout << "error linking program1: " << info_log << std::endl;
		return -1;
	}

	glAttachShader(shader_programs[1], vertex_shader);
	glAttachShader(shader_programs[1], fragment_shaders[1]);
	glLinkProgram(shader_programs[1]);

	glGetProgramiv(shader_programs[1], GL_LINK_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetProgramInfoLog(shader_programs[1], 512, NULL, info_log);
		std::cout << "error linking program2: " << info_log << std::endl;
		return -1;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shaders[0]);
	glDeleteShader(fragment_shaders[1]);

	//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input 
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_programs[0]);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
	
		glUseProgram(shader_programs[1]);
		glBindVertexArray(VAOs[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(2, EBOs);

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

const GLchar* LoadShaderFile(const char *file)
{
	FILE *fp;
	int err = fopen_s(&fp, file, "rb");
	if (err)
	{
		char err_buffer[100];
		std::cout << "error opening shader file : ";
		strerror_s(err_buffer, 100, errno);
		std::cout << err_buffer << std::endl;

		return nullptr;
	}

	fseek(fp, 0, SEEK_END);
	int count = ftell(fp);
	char *buffer = new GLchar[count + 1];
	fseek(fp, 0, SEEK_SET);
	fread(buffer, 1, count, fp);
	buffer[count] = '\0';
	fclose(fp);

	return buffer;
}



