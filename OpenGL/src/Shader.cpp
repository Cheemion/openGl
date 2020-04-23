#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>  

std::string Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);
	std::string line;
	std::string result; 
	while (getline(stream, line)) {
		result.append(line);
		result.append("\n");
	}
	return result;
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	// TODO: ERROR handling
	return id;
}
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	//已经得到另一个program 可以删除下面的 shader编译后的代码 
	glDeleteShader(vs);
	glDeleteShader(fs);
	//glDetachShader(vs);
	return program;
}

Shader::Shader(const std::string& vertexFilename, const std::string& fragmentFilename): m_RenderID(0) {
	std::string vertexSource = ParseShader(vertexFilename);
	std::string fragmentSource = ParseShader(fragmentFilename);
	m_RenderID = CreateShader(vertexSource, fragmentSource);
}
Shader::~Shader() {
	glDeleteProgram(m_RenderID);
}

void Shader::Bind() const {
	glUseProgram(m_RenderID);
}
void Shader::Unbind() const {
	glUseProgram(0);
}
void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	int location = glGetUniformLocation(m_RenderID, name.c_str());
	if (location == -1) {
		std::cout << "Error When Set Uniform 4f" << std::endl;
		return;
	}
	glUniform4f(location, v0, v1, v2, v3);
}


