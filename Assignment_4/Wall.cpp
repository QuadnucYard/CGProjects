#include"Wall.hpp" 
#include<roamer_engine/display/Materials.hpp>

namespace qy::cg {
	Wall::Wall() {
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

	void Wall::scale(const glm::vec3& value) {
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
}