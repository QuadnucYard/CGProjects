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
using namespace std;

constexpr auto numVAOs = 1;
constexpr auto numVBOs = 1;
constexpr auto numEBOs = 1;

//全局变量
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint ebo[numVBOs];
GLuint vColorLoc, vPositionLoc;

constexpr int numDiv = 36;
constexpr float r1 = 0.4f, r2 = 0.7f;

//导入着色器，初始化窗口
void init(GLFWwindow* window)
{
	//编译、链接着色器程序
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	float vertices[numDiv * 2 * 5] {};
	GLuint indices[numDiv * 2 * 3] {};

	default_random_engine rng;
	uniform_real_distribution<float> distrib(0.0f, 1.0f);
	normal_distribution<float> distrib2(0.0f, 0.05f);

	using namespace std::numbers;
	for (int i = 0; i < numDiv; i++) {
		vertices[i * 10 + 0] = (r2 + distrib2(rng)) * cos(pi * 2 * i / numDiv);
		vertices[i * 10 + 1] = (r2 + distrib2(rng)) * sin(pi * 2 * i / numDiv);
		vertices[i * 10 + 2] = distrib(rng);
		vertices[i * 10 + 3] = distrib(rng);
		vertices[i * 10 + 4] = distrib(rng);
		vertices[i * 10 + 5] = (r1 + distrib2(rng)) * cos(pi * 2 * i / numDiv);
		vertices[i * 10 + 6] = (r1 + distrib2(rng)) * sin(pi * 2 * i / numDiv);
		vertices[i * 10 + 7] = distrib(rng);
		vertices[i * 10 + 8] = distrib(rng);
		vertices[i * 10 + 9] = distrib(rng);
		indices[i * 6 + 0] = (i * 2 + 0) % (numDiv * 2);
		indices[i * 6 + 1] = (i * 2 + 1) % (numDiv * 2);
		indices[i * 6 + 2] = (i * 2 + 2) % (numDiv * 2);
		indices[i * 6 + 3] = (i * 2 + 3) % (numDiv * 2);
		indices[i * 6 + 4] = (i * 2 + 1) % (numDiv * 2);
		indices[i * 6 + 5] = (i * 2 + 2) % (numDiv * 2);
	}

	// Associate out shader variables with our data buffer    
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");

	// VAO
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	// VBO
	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// EBO
	glGenBuffers(numEBOs, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// VertexAttribPointer
	glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(vPositionLoc);
	glVertexAttribPointer(vColorLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(vColorLoc);

}
//函数绘制
void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);

	glClearColor(0.5f, 0.8f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//绘制模型
	glBindVertexArray(vao[0]);
	glDrawElements(GL_TRIANGLES, 6 * numDiv, GL_UNSIGNED_INT, 0);
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