#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//the program ID
	unsigned int ID;

	//constructor reads and builds the sahder
	Shader(const GLchar *vertex_path, const GLchar *fragment_path, const GLchar *geometry_path = nullptr);
	//use/activate the shader
	void Use();
	//utility uniform functions
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetVec2(const std::string &name, const glm::vec2 &value) const;
	void SetVec2(const std::string &name, float x, float y) const;
	void SetVec3(const std::string &name, const glm::vec3 &value) const;
	void SetVec3(const std::string &name, float x, float y, float z) const;
	void SetVec4(const std::string &name, const glm::vec4 &value) const;
	void SetVec4(const std::string &name, float x, float y, float z, float w) const;
	void SetMat2(const std::string &name, const glm::mat2 &mat) const;
	void SetMat3(const std::string &name, const glm::mat3 &mat) const;
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;

private:
	//utility function for checking compilation/linkin errors.
	void CheckCompileErrors(GLuint shader, std::string type);
};

#endif