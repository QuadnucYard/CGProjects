#include <roamer_engine.h>
#include <cmath>
#include <numbers>
#include <random>

class MyApplication: public qy::cg::Application {

private:
	static constexpr int numDiv = 36;
	static constexpr float r1 = 0.4f, r2 = 0.7f;

	qy::cg::ShaderProgram renderingProgram;
	qy::cg::MeshRenderer* meshRenderer;
	qy::cg::MeshRenderer* meshRenderer2;

protected:
	void init() override {
		meshRenderer = new qy::cg::MeshRenderer();
		meshRenderer2 = new qy::cg::MeshRenderer();
		renderingProgram = qy::cg::ShaderProgram("vertShader.glsl", "fragShader.glsl");
		meshRenderer->setShader(renderingProgram);
		meshRenderer2->setShader(renderingProgram);

		auto&& mesh = meshRenderer->getMesh();
		mesh.setVertices({{0.0f, 0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}});
		mesh.setColors({{1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}});
		mesh.setTriangles({0, 1, 2});
		auto&& mesh2 = meshRenderer2->getMesh();
		mesh2.setVertices({{0.5f, 0.5f, 0.0f}, {0.0f, -0.5f, 0.0f}, {1.0f, -0.5f, 0.0f}});
		mesh2.setColors({{1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 0.5f}});
		mesh2.setTriangles({0, 1, 2});
	}

	void display() override {
		glClearColor(0.5f, 0.8f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		meshRenderer->render();
		meshRenderer2->render();
	}
};

int main() {
	MyApplication app;
	app.createWindow(800, 800, "MyApplication");
	app.run();
	return 0;
}