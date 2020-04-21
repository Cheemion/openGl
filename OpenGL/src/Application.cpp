#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

static std::string ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);
	std::string line;
	std::string result;
	while (getline(stream, line)) {
		result.append(line);
		result.append("\n");
	}
	return result;
}


static unsigned int CompileShader(unsigned int type, const std::string& source) {
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
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(2);

	glewInit();
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	float position[] = {
		-0.5f, -0.5f, //0
		 0.5f, -0.5f,//1
		 0.5f,  0.5f,//2
		-0.5f,  0.5//3
	};

	//必须是unsigned int 要不然跑不起
	unsigned int indices[]{
		0, 1, 2,  //第一个点
		2, 3, 0   //第二点
	};
	//生产vao
	VertexArray va;
	va.Bind();

	//生成vbo
	VertexBuffer vb(position, 4 * 2 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	//ibo
	IndexBuffer ib(indices, 6);
	ib.Bind();
	//到此vao完成组装
	va.Unbind();
	vb.Unbind();
	ib.Unbind();


	std::string vertexShader = ParseShader("res/shaders/vertex.shader");
	std::string fragmentShader = ParseShader("res/shaders/fragment.shader");
	unsigned int shader = CreateShader(vertexShader, fragmentShader);

	int location = glGetUniformLocation(shader, "u_Color");
	if (location == -1) {
		std::cout << "Error" << std::endl;
	}
	glUniform4f(location, 0.2f, 0.1f, 0.1f, 1.0f);
	
	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */


	va.Bind();
	glUseProgram(shader);
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform4f(location, r, 0.1f, 0.1f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (r > 1.0f) {
			increment = -0.05f;
		}
		else if (r < 0.0f) {
			increment = 0.05f;
		}
		r = r + increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	//glDeleteVertexArrays()
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}