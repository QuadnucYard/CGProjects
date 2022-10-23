#include "roamer_engine/display/Mesh.hpp"
#include <cassert>

namespace qy::cg {

	class Mesh::MeshData {
	private:
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec4 tangent;
			glm::vec4 color;
			glm::vec2 uv;
		};

		std::vector<Vertex> __vertices;
		std::vector<GLuint> __indices;
		std::vector<MeshTopology> __topologies;
		std::vector<size_t> __indexSizes;

	public:
		size_t verticeCount() const { return __vertices.size(); }

		size_t submeshCount() const { return __indexSizes.size(); }

		void getVertices(std::vector<glm::vec3>& outVertices) const {
			copyAttributeInto<glm::vec3>(outVertices, offsetof(Vertex, position));
		}

		void getNormals(std::vector<glm::vec3>& outNormals) const {
			copyAttributeInto<glm::vec3>(outNormals, offsetof(Vertex, normal));
		}

		void getTangents(std::vector<glm::vec4>& outTangents) const {
			copyAttributeInto<glm::vec4>(outTangents, offsetof(Vertex, tangent));
		}

		void getColors(std::vector<glm::vec4>& outColors) const {
			copyAttributeInto<glm::vec4>(outColors, offsetof(Vertex, color));
		}

		void getUVs(std::vector<glm::vec2>& outUVs) const {
			copyAttributeInto<glm::vec2>(outUVs, offsetof(Vertex, uv));
		}

		void setVertices(const std::vector<glm::vec3>& inVertices) {
			copyAttributeFrom<glm::vec3>(inVertices, offsetof(Vertex, position));
		}

		void setNormals(const std::vector<glm::vec3>& inNormals) {
			copyAttributeFrom<glm::vec3>(inNormals, offsetof(Vertex, normal));
		}

		void setTangents(const std::vector<glm::vec4>& inTangents) {
			copyAttributeFrom<glm::vec4>(inTangents, offsetof(Vertex, tangent));
		}

		void setColors(const std::vector<glm::vec4>& inColors) {
			copyAttributeFrom<glm::vec4>(inColors, offsetof(Vertex, color));
		}

		void setUVs(const std::vector<glm::vec2>& inUVs) {
			copyAttributeFrom<glm::vec2>(inUVs, offsetof(Vertex, uv));
		}

	private:
		template <class T>
		void copyAttributeInto(std::vector<T>& buffer, size_t offset) const {
			assert(buffer.size() >= __vertices.size());
			auto data = __vertices.data();
			for (size_t i = 0, s = buffer.size(); i < s; i++) {
				buffer[i] = *reinterpret_cast<const T*>(reinterpret_cast<const char*>(data + i) + offset);
			}
		}

		template <class T>
		void copyAttributeFrom(const std::vector<T>& buffer, size_t offset) {
			__vertices.resize(buffer.size());
			auto data = const_cast<Vertex*>(__vertices.data());
			for (size_t i = 0, s = buffer.size(); i < s; i++) {
				*reinterpret_cast<T*>(reinterpret_cast<char*>(data + i) + offset) = buffer[i];
			}
		}

		friend class Mesh;
	};


	struct Mesh::Impl {
		MeshData data;
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		bool dirty;

		Impl(): dirty(true) {
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
		}
		~Impl() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
		}

	};

	DEFINE_OBJECT(Mesh);

	std::vector<glm::vec3> Mesh::getVertices() const {
		std::vector<glm::vec3> outVertices(pImpl->data.verticeCount());
		pImpl->data.getVertices(outVertices);
		return outVertices;
	}

	void Mesh::setVertices(const std::vector<glm::vec3>& vertices) {
		pImpl->data.setVertices(vertices);
		pImpl->dirty = true;
	}

	std::vector<glm::vec4> Mesh::getColors() const {
		std::vector<glm::vec4> outColors(pImpl->data.verticeCount());
		pImpl->data.getColors(outColors);
		return outColors;
	}

	void Mesh::setColors(const std::vector<glm::vec4>& colors) {
		pImpl->data.setColors(colors);
		pImpl->dirty = true;
	}

	std::vector<glm::vec3> Mesh::getNormals() const
	{
		std::vector<glm::vec3> outNormals(pImpl->data.verticeCount());
		pImpl->data.getNormals(outNormals);
		return outNormals;
	}

	void Mesh::setNormals(const std::vector<glm::vec3>& normals)
	{
		pImpl->data.setNormals(normals);
		pImpl->dirty = true;
	}

	std::vector<glm::vec4> Mesh::getTangents() const
	{
		std::vector<glm::vec4> outTangents(pImpl->data.verticeCount());
		pImpl->data.getTangents(outTangents);
		return outTangents;
	}

	void Mesh::setTangents(const std::vector<glm::vec4>& tangents)
	{
		pImpl->data.setTangents(tangents);
		pImpl->dirty = true;
	}

	std::vector<glm::vec2> Mesh::getUVs() const
	{
		std::vector<glm::vec2> outUVs(pImpl->data.verticeCount());
		pImpl->data.getUVs(outUVs);
		return outUVs;
	}

	void Mesh::setUVs(const std::vector<glm::vec2>& uvs)
	{
		pImpl->data.setUVs(uvs);
		pImpl->dirty = true;
	}

	size_t Mesh::getSubMeshCount() const {
		return pImpl->data.submeshCount();
	}

	void Mesh::setSubMeshCount(size_t count) {
		pImpl->data.__indexSizes.resize(count);
		pImpl->data.__topologies.resize(count);
	}

	size_t Mesh::getIndexCount() const {
		return pImpl->data.__topologies.size();
	}

	size_t Mesh::getIndexStart(size_t submesh) const {
		return std::accumulate(pImpl->data.__indexSizes.begin(), pImpl->data.__indexSizes.begin() + submesh, size_t {});
	}

	std::vector<GLuint> Mesh::getIndices(size_t submesh) const {
		size_t offset = getIndexStart(submesh);
		std::vector<GLuint> outIndices(pImpl->data.__indices.begin() + offset, pImpl->data.__indices.begin() + offset + pImpl->data.__indexSizes[submesh]);
		return outIndices;
	}

	void Mesh::setIndices(const std::vector<GLuint>& indices, MeshTopology topology, size_t submesh) {
		if (submesh >= getSubMeshCount()) setSubMeshCount(submesh + 1);
		size_t offset = getIndexStart(submesh);
		auto&& ind = pImpl->data.__indices;
		ind.erase(ind.begin() + offset, ind.begin() + offset + pImpl->data.__indexSizes[submesh]);
		ind.insert(ind.begin() + offset, indices.begin(), indices.end());
		pImpl->data.__topologies[submesh] = topology;
		pImpl->data.__indexSizes[submesh] = indices.size();
		pImpl->dirty = true;
	}

	std::vector<GLuint> Mesh::getTriangles() const {
		return pImpl->data.__indices;
	}

	void Mesh::setTriangles(const std::vector<GLuint>& triangles) {
		pImpl->data.__indices = triangles;
		pImpl->data.__indexSizes = {triangles.size()};
		pImpl->data.__topologies = {MeshTopology::Triangles};
		pImpl->dirty = true;
	}

	void Mesh::uploadMeshData() {
		glBindVertexArray(pImpl->vao);

		auto&& v = pImpl->data.__vertices;
		auto&& vd = reinterpret_cast<const char*>(v.data());
		auto stride = sizeof(MeshData::Vertex);
		glBindBuffer(GL_ARRAY_BUFFER, pImpl->vbo);
		glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(MeshData::Vertex), v.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(MeshData::Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(MeshData::Vertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(MeshData::Vertex, tangent));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(MeshData::Vertex, color));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(MeshData::Vertex, uv));
		glEnableVertexAttribArray(4);
		
		auto&& e = pImpl->data.__indices;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pImpl->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, e.size() * sizeof(GLuint), e.data(), GL_STATIC_DRAW);

		pImpl->dirty = false;
	}

	void Mesh::__render() {
		if (pImpl->dirty) {
			uploadMeshData();
		}
		glBindVertexArray(pImpl->vao);
		for (size_t i = 0, s = 0; i < getSubMeshCount(); s += pImpl->data.__indexSizes[i++]) {
			glDrawElements(
				static_cast<GLenum>(pImpl->data.__topologies[i]),
				static_cast<GLsizei>(pImpl->data.__indexSizes[i]),
				GL_UNSIGNED_INT,
				(void*)s
			);
		}
	}

}