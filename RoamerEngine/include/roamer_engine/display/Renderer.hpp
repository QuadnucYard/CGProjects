#pragma once
#include "Component.hpp"

namespace qy::cg {

	class Material;

	class Renderer: public Component {

	public:
		DECL_OBJECT(Renderer);

		const ptr<Material>& getMaterial();

		const ptr_vector<Material>& getMaterials();

		const ptr_vector<Material>& __getMaterials() const;

		void setMaterial(const ptr<Material>& material);

		void setMaterials(const ptr_vector<Material>& materials);

		const ptr<Material>& getSharedMaterial();

		const ptr_vector<Material>& getSharedMaterials();

		void setSharedMaterial(const ptr<Material>& material);

		void setSharedMaterials(const ptr_vector<Material>& materials);

		virtual void __render() = 0;

	private:
		DECL_PIMPL;
	};
}