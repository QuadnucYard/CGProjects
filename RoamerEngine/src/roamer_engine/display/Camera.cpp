#include "roamer_engine/display/Camera.hpp"
#include "roamer_engine/display/Scene.hpp"

namespace qy::cg {

	struct Camera::Impl {

	};

	DEFINE_OBJECT(Camera);

	void Camera::render() {

		struct RenderItem {
			int renderOrder;
			int layerOrder;
			Renderer* renderer;
			glm::mat4 model;
		};
		std::vector<RenderItem> renderList;

		const auto& dfs = [&](const TransformPtr& parent, const glm::mat4& model) {
			const auto& s = [&](auto&& self, const TransformPtr& parent, const glm::mat4& model) -> void {
				int i = 0;
				for (auto&& child : parent) {
					auto model2 = model * child->modelMatrix();
					for (auto&& r : child->getComponents<Renderer>()) {
						renderList.emplace_back(0, i, r.get(), model2);
					}
					self(self, child, model2);
					i++;
				}
			};
			s(s, parent, model);
		};

		dfs(Scene::current()->root(), Scene::current()->root()->modelMatrix());

		std::ranges::sort(renderList, [](const RenderItem& o1, const RenderItem& o2) {
			return std::tie(o1.renderOrder, o2.layerOrder) < std::tie(o2.renderOrder, o2.layerOrder);
		});

		auto view = glm::lookAt(obj()->transform()->position(), {0, 0, 0}, {0,1,0});
		auto proj = glm::perspective(1.0f, 1.0f, 0.1f, 10.0f);
		
		for (auto&& r : renderList) {
			r.renderer->render(r.model, view, proj);
		}
	}

}