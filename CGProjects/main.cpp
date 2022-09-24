#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <numbers>
#include <random>
#include "Utils.h"
#include <roamer_engine/display/ShaderProgram.h>
#include <roamer_engine/display/MeshRenderer.h>

using namespace std;

//全局变量
qy::cg::ShaderProgram renderingProgram;
qy::cg::MeshRenderer* meshRenderer;
qy::cg::MeshRenderer* meshRenderer2;

constexpr int numDiv = 36;
constexpr float r1 = 0.4f, r2 = 0.7f;

//导入着色器，初始化窗口
void init(GLFWwindow* window)
{
	meshRenderer = new qy::cg::MeshRenderer();
	meshRenderer2 = new qy::cg::MeshRenderer();
	renderingProgram = qy::cg::ShaderProgram("vertShader.glsl", "fragShader.glsl");
	meshRenderer->setShader(renderingProgram);
	meshRenderer2->setShader(renderingProgram);

	auto&& mesh = meshRenderer->getMesh();
	mesh.setVertices({{0.0f, 0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}});
	mesh.setColors({{1.0f, 0.0f, 0.0f,1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}});
	mesh.setTriangles({0, 1, 2});
	auto&& mesh2 = meshRenderer2->getMesh();
	mesh2.setVertices({{0.5f, 0.5f, 0.0f}, {0.0f, -0.5f, 0.0f}, {1.0f, -0.5f, 0.0f}});
	mesh2.setColors({{1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 0.5f}});
	mesh2.setTriangles({0, 1, 2});
}
//函数绘制
void display(GLFWwindow* window, double currentTime)
{
	glClearColor(0.5f, 0.8f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	meshRenderer->render();
	meshRenderer2->render();
}

int main(void)
{
	//glfw初始化
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	//窗口版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //次版本号

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 800, "Not simple", NULL, NULL);
	glfwMakeContextCurrent(window);
	//设置缓存刷新时间
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)//glew初始化
	{
		exit(EXIT_FAILURE);
	}

	//导入着色器，创建和绑定VAO和VBO
	init(window);

	//事件循环，接收输入事件
	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime()); //绘制函数，主体
		glfwSwapBuffers(window); //交换颜色缓存
		glfwPollEvents(); // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}