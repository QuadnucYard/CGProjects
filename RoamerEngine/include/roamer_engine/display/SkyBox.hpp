#pragma once
#include "Component.hpp"

namespace qy::cg {

	class Material;

	class SkyBox : public Component {

	public:
		DECL_OBJECT(SkyBox);

		static ptr<SkyBox> loadFromFile(const fs::path& left, const fs::path& right, const fs::path& up, const fs::path& down, const fs::path& front, const fs::path& back);

		void __render(const glm::mat4& view, const glm::mat4& proj);

	private:
		DECL_PIMPL;
	};

}