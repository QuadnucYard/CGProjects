#include "roamer_engine/display/Sprite.hpp"
#include "roamer_engine/display/Texture2D.hpp"
#include "roamer_engine/display/Mesh.hpp"
#include <array>

namespace qy::cg {

	struct Sprite::Impl {
		float pixelsPerUnit {100};
		vec2 pivot {0.5f,0.5f};
		rectf rect {}; // 使用的贴图在原texture中的像素位置
		ptr<Texture2D> texture;
		//rectf textureRect; // 在贴图中的范围
		//vec2 textureRectOffset;

		Mesh mesh;
		std::array<vec2, 4> uv {};
		/*
		需要存哪些数据？
		在原贴图中位置即可
		剩下的可以算
		*/

		void createMesh() {
			// 一共4个顶点
			//  3  2
			//  0  1
			int width = texture->width(), height = texture->height();
			uv = {
				vec2 {rect.xMin() / width, rect.yMin() / height},
				{rect.xMax() / width, rect.yMin() / height},
				{rect.xMax() / width, rect.yMax() / height},
				{rect.xMin() / width, rect.yMax() / height},
			};
			mesh.setVertices({
				{-pivot.x / pixelsPerUnit * width, -pivot.y / pixelsPerUnit * height, 0},
				{(1 - pivot.x) / pixelsPerUnit * width, -pivot.y / pixelsPerUnit * height, 0},
				{(1 - pivot.x) / pixelsPerUnit * width, (1 - pivot.y) / pixelsPerUnit * height, 0},
				{-pivot.x / pixelsPerUnit * width, (1 - pivot.y) / pixelsPerUnit * height, 0},
				});
			mesh.setColors({4, {1.0f, 1.0f, 1.0f, 1.0f}});
			mesh.setNormals({4, {0, 0, 1}});
			mesh.setUVs({uv[0], uv[1], uv[2], uv[3]});
			mesh.setIndices({0, 1, 2, 3}, MeshTopology::TriangleFan, 0);
		}
	};

	DEFINE_OBJECT(Sprite);

	ptr<Texture2D> Sprite::texture() const {
		return pImpl->texture;
	}

	ptr<Sprite> Sprite::create(ptr<Texture2D> texture, rectf rect, vec2 pivot, float pixelsPerUnit) {
		auto sprite = instantiate<Sprite>();
		sprite->pImpl->texture = std::move(texture);
		sprite->pImpl->rect = rect;
		sprite->pImpl->pivot = pivot;
		sprite->pImpl->pixelsPerUnit = pixelsPerUnit;
		sprite->pImpl->createMesh();
		return sprite;
	}

	void Sprite::__render() {
		pImpl->mesh.__render();
	}

}