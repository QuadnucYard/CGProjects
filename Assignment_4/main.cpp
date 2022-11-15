#include <roamer_engine.hpp>
#include <cmath>
#include <numbers>
#include <random>

using namespace qy::cg;

glm::quat RotU2V(glm::vec3 start, glm::vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);
	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	rotationAxis = cross(start, dest);
	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;
	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}


std::shared_ptr<Material> getMatByColor(color_t color, float shininess = 51.2f) {
	auto mat = instantiate<Material>();
	mat->setShader(Shader::fromSourceFile("light.vert", "light.frag"));
	mat->setColor("material.ambient", color);
	mat->setColor("material.diffuse", color);
	mat->setColor("material.specular", color);
	mat->setFloat("material.shininess", shininess);

	mat->setColor("light.ambient", { 0.0, 0.0, 0.0, 1.0 });
	mat->setColor("light.diffuse", { 1.0, 1.0, 1.0, 1.0 });
	mat->setColor("light.specular", { 1.0, 1.0, 1.0, 1.0 });
	mat->getShader().setVec3("light.position", { 1.0, 0.0, 0.0 });

	mat->setColor("globalAmbient", { 0.7, 0.1, 0.7, 1.0 });

	return mat;
}

class Wall {
private:
	std::shared_ptr<DisplayObject> wallObj;
public:
	Wall() {
		static int instance_cnt = 0;

		wallObj = DisplayObject::create();
		wallObj->addComponent<MeshRenderer>()->setMaterial(Materials::geom_unlit);
		auto mesh = wallObj->addComponent<MeshFilter>()->mesh();
		static size_t vertNum = 24;
		mesh->setVertices({
			{-1, -1, 1}, {-1, 1, 1}, {1, 1, 1}, {1, -1, 1},
			{1, -1, 1}, {1, 1, 1}, {1, 1, -1},{1, -1, -1},
			{1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, -1},
			{-1,-1,-1}, {-1, 1, -1}, {-1, 1, 1}, {-1,-1,1},
			{-1,1,1}, {-1,1,-1}, {1, 1,-1}, {1, 1, 1},
			{-1,-1,-1}, {-1, -1, 1}, {1,-1,1}, {1,-1,-1},
			});
		mesh->setColors({ vertNum, {1.0f, 1.0f, 1.0f, 1.0f} });
		mesh->setIndices({
			1, 0, 3, 3, 2, 1,
			5, 4, 7, 7, 6, 5,
			9, 8, 11, 11, 10, 9,
			13, 12, 15, 15, 14, 13,
			17, 16, 19, 19, 18, 17,
			21, 20, 23, 23, 22, 21
			}, MeshTopology::Triangles, 0);
		mesh->setNormals({
			{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
			{1, 0, 0}, {1, 0, 0}, {1, 0, 0},{1, 0, 0},
			{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
			{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
			{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
			{0,-1, 0}, {0, -1, 0}, {0,-1,0}, {0,-1, 0},
			});
		mesh->setUVs({
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0}
			});

		if (instance_cnt++ == 0) {
			auto&& mat = wallObj->getComponent<MeshRenderer>()->getSharedMaterial();
			auto tex = Texture::loadFromFile("assets/wall.jpg");
			mat->setMainTexture(tex);
		}
	}
	std::shared_ptr<DisplayObject> getObj() { return wallObj; }
	void scale(const glm::vec3& value) {
		auto mesh = wallObj->getComponent<MeshFilter>()->mesh();
		float kx = value.x;
		float ky = value.y;
		float kz = value.z;
		mesh->setUVs({
			{0,0},{0,ky},{kx,ky},{kx,0},
			{0,0},{0,ky},{kz,ky},{kz,0},
			{0,0},{0,ky},{kx,ky},{kx,0},
			{0,0},{0,ky},{kz,ky},{kz,0},
			{0,0},{0,kz},{kx,kz},{kx,0},
			{0,0},{0,kz},{kx,kz},{kx,0}
			});
		wallObj->transform()->scale(value);
	}
	void position(const glm::vec3& value) { wallObj->transform()->position(value); }
	void rotation(const glm::vec3& value) { wallObj->transform()->rotation(value); }
};

//最后没有用
class Tunnal {
private:
	Wall wUp, wDown, wLeft, wRight;
	float longth, width, height, thickness;
	std::shared_ptr<DisplayObject> tunnalObj;
public:
	Tunnal(float longth, float width, float height,float thickness = 0.2):
		wUp(),wDown(),wLeft(), wRight(),longth(longth),width(width),height(height),thickness(thickness)
	{
		tunnalObj = DisplayObject::create();
		wUp.scale({ width/2, thickness/2, longth/2 });
		wUp.position({ 0.0, height / 2 - thickness / 2, 0.0 });
		wDown.scale({ width / 2, thickness / 2, longth / 2 });
		wDown.position({ 0.0, -(height / 2 - thickness / 2), 0.0 });
		wLeft.scale({ thickness / 2, height / 2 - thickness, longth / 2 });
		wLeft.position({ -(width / 2 - thickness / 2), 0.0, 0.0 });
		wRight.scale({ thickness / 2, height / 2 - thickness, longth / 2 });
		wRight.position({ width / 2 - thickness / 2, 0.0, 0.0 });

		tunnalObj->transform()->addChild(wUp.getObj()->transform());
		tunnalObj->transform()->addChild(wDown.getObj()->transform());
		tunnalObj->transform()->addChild(wLeft.getObj()->transform());
		tunnalObj->transform()->addChild(wRight.getObj()->transform());
	}
	std::shared_ptr<DisplayObject> getObj() { return tunnalObj; }
	void scale(const glm::vec3& value) {tunnalObj->transform()->scale(value);	}
	void position(const glm::vec3& value) { tunnalObj->transform()->position(value); }
	void rotation(const glm::vec3& value) { tunnalObj->transform()->rotation(value); }
};

//方法来自https://github.com/victorlaurentiu/MazeGenerator_graphsUnioning
class Maze
{
private:
	std::vector<std::vector<int>> maze;
	struct vec2
	{
		int x, y;
		vec2() {}
		vec2(int x, int y) : x(x), y(y) {}
		// distance^2
		int distance2(const vec2& other) const
		{
			const int dx = (other.x - x);
			const int dy = (other.y - y);
			return dx * dx + dy * dy;
		}
		vec2& operator= (const vec2& other)
		{
			if (this == &other)
				return *this;

			x = other.x;
			y = other.y;

			return *this;
		}
		const vec2 operator+ (const vec2& other) const { return vec2{ x + other.x, y + other.y }; }
		vec2 operator/ (int devider) const { return vec2{ x / devider, y / devider }; }
		bool operator== (const vec2& other) const {	return (x == other.x) && (y == other.y);}
		bool operator!= (const vec2& other) const{return !(*this == other);}
	};
	int width, height;
public:
	Maze(int width, int height) :maze( width + 2, std::vector<int>(height + 2, 1)), width(width), height(height) {
		std::default_random_engine e;

		// used to search for neighbor nodes
		static const vec2 UP2{ 0, -2 };
		static const vec2 RIGHT2{ 2, 0 };
		static const vec2 DOWN2{ 0, 2 };
		static const vec2 LEFT2{ -2, 0 };

		vec2 dirs[] = { UP2, RIGHT2, DOWN2, LEFT2 };	// directions

		// add a Block/Wall on every even position
		for (int i = 0; i < width + 2; i++)
		{
			for (int j = 0; j < height + 2; j++)
			{
				maze[i][j] = -1;
			}
		}
		// assign each non-Block/Wall to a graph
		int id_graph = 0;
		for (int i = 2; i < width; i += 2)
		{
			for (int j = 2; j < height; j += 2)
			{
				id_graph++;
				maze[i][j] = id_graph;
			}
		}

		int nrOfGraphs = id_graph;	// last id found is the number of graphs

		// returns a reference to maze[v.x][v.y]
		auto Mz = [this](const vec2& v)->int& { return maze[v.x][v.y]; };

		vec2 saved_this_pos, saved_neigh_pos;	// used to create connection between them (delete the block between them)
		while (nrOfGraphs > 1)
		{
			// choose a random graph
			const int randX = (((rand()) % (width - 4)) / 2) * 2 + 2;	// gives a random odd number between 2 and n - 2
			const int randY = (((rand()) % (height - 4)) / 2) * 2 + 2;
			const int rand_graph_Id = maze[randX][randY];

			int nrOfNeighboringNodes = 0;	// used for randomly choosing between all neighbors so I can connect the graphs
			// iterate through all the nodes that are in the random graph
			for (int i = 2; i < width; i += 2)
			{
				for (int j = 2; j < height; j += 2)
				{
					vec2 this_pos = vec2(i, j);
					int curr_pos_graph_id = Mz(this_pos);
					if (curr_pos_graph_id == rand_graph_Id)	// if current cell is part of the random graph
					{
						for (const vec2& dir : dirs)	// for each neighbor
						{
							const vec2 neigh_pos(i + dir.x, j + dir.y);
							int neighb_graph_Id = Mz(neigh_pos);
							// if the neighbor node is from a different graph,
							// randomly choose if to connect the rand_Id_graph to this graph
							// by selecting it with a probability of 1/nrOfNeighboringNodes
							if (neighb_graph_Id != -1
								&& neighb_graph_Id != rand_graph_Id)
							{
								// found a neighboring node that is not part of this graph => increment nrOfNeighboringNodes
								nrOfNeighboringNodes++;

								// randomly choose if to connect the neighb_graph_Id to this graph
								if (rand_true(nrOfNeighboringNodes))
								{
									saved_this_pos = this_pos;
									saved_neigh_pos = neigh_pos;
								}
							}
						}
					}
				}
			}

			int id_ng = Mz(saved_neigh_pos);	// id of a the neighbor graph

			Mz((saved_this_pos + saved_neigh_pos) / 2) = rand_graph_Id;	// the block between them = id_graph

			for (int i = 2; i < width; i += 2)
			{
				for (int j = 2; j < height; j += 2)
				{
					if (maze[i][j] == id_ng)	// convert the neighbor graph into this one
					{
						maze[i][j] = rand_graph_Id;
					}
				}
			}

			nrOfGraphs--;
		}
	}
	bool rand_true(int nr){return ((float)rand() / RAND_MAX < (float)1 / nr);}
	std::vector<std::vector<int>> getMaze() { return maze; }
};

class MyApplication : public qy::cg::Application {
private:
	std::shared_ptr<qy::cg::Scene> scene;
	std::shared_ptr<qy::cg::Camera> cam;
	glm::vec3 camFront{ 0, 0, -1 };
	glm::vec3 camUp{ 0, 1, 0 };

	glm::vec3 posLast = { 0.0f, 0.0f, 0.0f };
	bool mouseLeftPressed = false;
	float sensitivity = 0.1f;
	float yaw = -90.0f;
	float pitch = 0.0f;

protected:
	void init() override {
		using namespace qy::cg;
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);*/
		scene = Scene::create();
		cam = scene->createCamera();
		//cam->setBackgroundColor({ 0.4f, 0.3f, 0.1f, 1.0f });
		cam->obj()->transform()->position({ 0, 0, 0 });
		cam->addComponent(SkyBox::loadFromFile(
			"assets/skybox/right.jpg",
			"assets/skybox/left.jpg",
			"assets/skybox/top.jpg",
			"assets/skybox/bottom.jpg",
			"assets/skybox/front.jpg",
			"assets/skybox/back.jpg"
		));
		cam->setClearFlags(CameraClearFlags::Skybox);

		int width = 21;
		int height = 21;
		Maze m(width, height);
		auto maze = m.getMaze();
		for (int i = 0; auto line : maze) {
			for (int j = 0; auto item : line) {
				if (item == -1) {
					Wall w;
					w.position({ (i-2) * 2.0, 0.0, (j-height) * 2.0 });
					scene->root()->addChild(w.getObj()->transform());
				}
				else {
					Wall w1, w2;
					w1.position({ (i - 2) * 2.0, 2.0, (j - height) * 2.0 });
					scene->root()->addChild(w1.getObj()->transform());
					w2.position({ (i - 2) * 2.0, -2.0, (j - height) * 2.0 });
					scene->root()->addChild(w2.getObj()->transform());
				}
				j++;
			}
			i++;
		}
	}

	void update() override {
		using namespace qy::cg;
		auto rot = glm::qua(glm::vec3({ 0.0, Time::time() , 0.0 }));
		//earth->transform()->rotation(rot);

		scene->dispatch_update();

		float cameraSpeed = (float)Time::deltaTime() * 5;
		auto camPos = cam->obj()->transform()->position();
		auto goFront = glm::vec3({ camFront.x, 0.0, camFront.z });
		if (Input::getKey(KeyCode::W)) camPos += cameraSpeed * goFront;
		if (Input::getKey(KeyCode::S)) camPos -= cameraSpeed * goFront;
		if (Input::getKey(KeyCode::A)) camPos -= glm::normalize(glm::cross(goFront, camUp)) * cameraSpeed;
		if (Input::getKey(KeyCode::D)) camPos += glm::normalize(glm::cross(goFront, camUp)) * cameraSpeed;
		//if (Input::getKey(KeyCode::SPACE)) camPos += camUp * cameraSpeed;
		//if (Input::getKey(KeyCode::LEFT_SHIFT)) camPos -= camUp * cameraSpeed;

		if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !mouseLeftPressed) {
			posLast = Input::mousePosition();
			mouseLeftPressed = true;
		}
		if (Input::getMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT)) {
			mouseLeftPressed = false;
		}
		if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			glm::vec3 posNow = Input::mousePosition();
			glm::vec3 posDelta = posNow - posLast;
			posLast = posNow;
			float xOffset = posDelta.x * sensitivity;
			float yOffset = -posDelta.y * sensitivity;
			yaw += xOffset;
			pitch += yOffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			camFront = glm::normalize(front);
		}
		cam->transform()->position(camPos);
		cam->transform()->rotation(RotU2V({ 0.0, 0.0, -1.0 }, camFront));
		cam->setFieldOfView(std::clamp(cam->getFieldOfView() + Input::mouseScrollDelta().y, 1.0f, 80.0f));
	}

	void display() override {
		scene->dispatch_render();
	}
};

int main() {
	MyApplication app;
	app.createWindow(1280, 720, "MyApplication");
	app.run();
	return 0;
}
