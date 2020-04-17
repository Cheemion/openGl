#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
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

int draw_triangle(void)
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

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewInit();
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;
	
	float data[6] = {
		-0.5f, -0.5f,
		0.0f,0.5f,
		0.5f,-0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	std::string vertexShader = ParseShader("res/shaders/vertex.shader");
	std::string fragmentShader = ParseShader("res/shaders/fragment.shader");

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	//not used that buffer anymore
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		
		glDrawArrays(GL_TRIANGLES, 0, 3);
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