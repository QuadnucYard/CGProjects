#include "roamer_engine/display/ModelLoader.hpp"
#include "roamer_engine/display/MeshFilter.hpp"
#include "roamer_engine/display/MeshRenderer.hpp"
#include "roamer_engine/display/DisplayObject.hpp"
#include "roamer_engine/display/Materials.hpp"
#include "roamer_engine/display/Texture.hpp"
#include <tiny_obj_loader.h>
#include <iostream>

namespace qy::cg {

	ptr<DisplayObject> ModelLoader::loadObj(const fs::path& path) {
		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(path.string())) {
			throw std::runtime_error("TinyObjReader: " + reader.Error());
		}

		if (!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		auto obj = DisplayObject::create();
		obj->addComponent<MeshRenderer>()->setSharedMaterial(Materials::Lit);
		auto&& mesh = obj->addComponent<MeshFilter>()->sharedMesh();
		// 默认各属性的索引是一样的
		mesh->setVertices({
			reinterpret_cast<const glm::vec3*>(attrib.vertices.data()), 
			reinterpret_cast<const glm::vec3*>(attrib.vertices.data() + attrib.vertices.size())
			});
		mesh->setNormals({
			reinterpret_cast<const glm::vec3*>(attrib.normals.data()),
			reinterpret_cast<const glm::vec3*>(attrib.normals.data() + attrib.normals.size())
			});
		mesh->setUVs({
			reinterpret_cast<const glm::vec2*>(attrib.texcoords.data()),
			reinterpret_cast<const glm::vec2*>(attrib.texcoords.data() + attrib.texcoords.size())
			});

		for (size_t s = 0; s < shapes.size(); s++) {
			std::vector<GLuint> triangles;
			for (tinyobj::index_t idx : shapes[s].mesh.indices) {
				triangles.push_back(idx.vertex_index);
			}
			mesh->setTriangles(triangles, s);
		}
	
		return obj;
	}

	ptr<DisplayObject> ModelLoader::loadObjWithTexture(const fs::path& objPath, const fs::path& texturePath) {
		auto obj = loadObj(objPath);
		auto mat = obj->getComponent<MeshRenderer>()->getSharedMaterial()->clone();
		mat->setMainTexture(Texture::loadFromFile(texturePath));
		obj->getComponent<MeshRenderer>()->setSharedMaterial(mat);
		return obj;
	}


}