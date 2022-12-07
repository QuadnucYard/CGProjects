#include"Wall.hpp" 
#include<roamer_engine/display/Materials.hpp>
#include<numbers>

Wall::Wall() {
	static ptr<Material> wallMaterial;

	wallObj = DisplayObject::create();
	// wallObj->addComponent<MeshRenderer>()->setMaterial(Materials::Lit);
	auto mesh = wallObj->addComponent<MeshFilter>()->mesh();
	
	static int divNum = 40;
	static int vertNum = (divNum + 1) * (divNum + 1) * 6;

	std::vector<glm::vec3> vertices(vertNum);
	std::vector<glm::vec3> normals(vertNum);
	std::vector<glm::vec4> tangents(vertNum);
	std::vector<glm::vec2> uvs(vertNum);
	std::vector<GLuint> triangles(divNum * divNum * 3 * 2 * 6);
	std::vector<glm::vec4> colors(vertNum, {1.0f, 1.0f, 1.0f, 1.0f});

	const float pi = std::numbers::pi_v<float>;
	int idx = 0, idx_ = 0;
	for (int k = 0; k < 6; k++) {
		for (int i = 0; i <= divNum; i++) {
			for (int j = 0; j <= divNum; j++) {
				float u = (float)i / divNum, v = (float)j / divNum;	//[0,1]
				float p = u * 2 - 1, q = v * 2 - 1;	//[-1,1]
				uvs[idx] = {u, v};
				switch (k)
				{
				case 0: 
					vertices[idx] = {p, q, 1};
					normals[idx] = {0, 0, 1};
					tangents[idx] = {1, 0, 0, 1};
					break;
				case 1:	
					vertices[idx] = {p, q, -1};
					normals[idx] = {0, 0, -1};
					tangents[idx] = {1, 0, 0, 1};
					break;
				case 2:
					vertices[idx] = {p, 1, q};
					normals[idx] = {0, 1, 0};
					tangents[idx] = {1, 0, 0, 1};
					break;
				case 3:
					vertices[idx] = {p, -1, q};
					normals[idx] = {0, -1, 0};
					tangents[idx] = {1, 0, 0, 1};
					break;
				case 4:
					vertices[idx] = {1, p, q};
					normals[idx] = {1, 0, 0};
					tangents[idx] = {0, 1, 0, 1};
					break;
				case 5:
					vertices[idx] = {-1, p, q};
					normals[idx] = {-1, 0, 0};
					tangents[idx] = {0, 1, 0, 1};
					break;
				default:
					break;
				}
				idx++;

				if (i < divNum && j < divNum) {
					int vList[4];
					switch (k)
					{
					case 0: case 3: case 4:
						vList[0] = k * (divNum + 1) * (divNum + 1) + i * (divNum + 1) + j;
						vList[1] = k * (divNum + 1) * (divNum + 1) + (i + 1) * (divNum + 1) + j;
						vList[2] = k * (divNum + 1) * (divNum + 1) + (i + 1) * (divNum + 1) + j + 1;
						vList[3] = k * (divNum + 1) * (divNum + 1) + i * (divNum + 1) + j + 1;
						break;	   
					case 1: case 2: case 5:
						vList[1] = k * (divNum + 1) * (divNum + 1) + i * (divNum + 1) + j;
						vList[0] = k * (divNum + 1) * (divNum + 1) + (i + 1) * (divNum + 1) + j;
						vList[3] = k * (divNum + 1) * (divNum + 1) + (i + 1) * (divNum + 1) + j + 1;
						vList[2] = k * (divNum + 1) * (divNum + 1) + i * (divNum + 1) + j + 1;
						break;
					default:
						break;
					}
					triangles[idx_++] = vList[0];
					triangles[idx_++] = vList[1];
					triangles[idx_++] = vList[2];
					triangles[idx_++] = vList[0];
					triangles[idx_++] = vList[2];
					triangles[idx_++] = vList[3];
				}

			}
		}
	}

	mesh->setVertices(vertices);
	mesh->setTangents(tangents);
	mesh->setUVs(uvs);
	mesh->setTriangles(triangles);
	mesh->setColors(colors);
	mesh->setNormals(normals);

	if (!wallMaterial) {
		auto path = std::filesystem::current_path() /"assets" / "shaders";
		auto normalMapShader = Shader::fromSourceFile(path / "wall-map.vert", path / "wall-map.frag");
		wallMaterial = instantiate<Material>();
		wallMaterial->setShader(normalMapShader);
		wallMaterial->setColor("material.ambient", { 1.0f, 1.0f, 1.0f, 1.0f });
		wallMaterial->setColor("material.diffuse", { 1.0f, 1.0f, 1.0f, 1.0f });
		wallMaterial->setColor("material.specular", { 0.00f, 0.0f, 0.0f, 1.0f });
		wallMaterial->setFloat("material.shininess", 0.2);
		wallMaterial->setMainTexture(Texture2D::load("assets/Rock034_2K_Color.png"));
		wallMaterial->setTexture("_NormalTex", Texture2D::load("assets/Rock034_2K_NormalGL.png"));
		wallMaterial->setTexture("_SpecTex", Texture2D::load("assets/Rock034_2K_Roughness.png"));
		wallMaterial->setTexture("_DisplaceTex", Texture2D::load("assets/Rock034_2K_Displacement_8bits.png"));
	}
	wallObj->addComponent<MeshRenderer>()->setSharedMaterials({wallMaterial});
	wallObj->transform()->scale({1.05, 1.05, 1.05});
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