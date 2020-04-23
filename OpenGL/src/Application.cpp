#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


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



	Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	shader.Bind();
	//setUniform是绑定到当前的program中which by calling glUseProgram.
	shader.setUniform4f("u_Color", 0.2f, 0.6f, 0.4f, 1.0f);
	shader.Unbind();
	



	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */


	va.Bind();
	shader.Bind();
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		shader.setUniform4f("u_Color", r, 0.3f, 0.4f, 1.0f);
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
	glfwTerminate();
	return 0;
}