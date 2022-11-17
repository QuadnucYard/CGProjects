#include "roamer_engine/display/Renderer.hpp"
#include "roamer_engine/display/Material.hpp"

namespace qy::cg {

	struct Renderer::Impl {
		ptr_vector<Material> materials;
		ptr_vector<Material> sharedMaterials;
	};
	
	DEFINE_OBJECT(Renderer);

	const ptr<Material>& Renderer::getMaterial() {
		if (!pImpl->sharedMaterials.empty() && pImpl->materials.at(0) == pImpl->sharedMaterials.at(0))
			pImpl->materials[0] = pImpl->sharedMaterials[0]->clone();
		return pImpl->materials[0];
	}

	const ptr_vector<Material>& Renderer::getMaterials() {
		for (size_t i = 0; i < pImpl->materials.size(); i++) {
			if (pImpl->materials[i] == pImpl->sharedMaterials[i])
				pImpl->materials[i] = pImpl->sharedMaterials[i]->clone();
		}
		return pImpl->materials;
	}

	const ptr_vector<Material>& Renderer::__getMaterials() const {
		return pImpl->materials;
	}

	void Renderer::setMaterial(const ptr<Material>& material) {
		if (pImpl->materials.empty()) pImpl->materials.resize(1);
		pImpl->materials.at(0) = material;
	}

	void Renderer::setMaterials(const ptr_vector<Material>& materials) {
		pImpl->materials = materials;
	}

	const ptr<Material>& Renderer::getSharedMaterial() {
		return pImpl->sharedMaterials.at(0);
	}

	const ptr_vector<Material>& Renderer::getSharedMaterials() {
		return pImpl->sharedMaterials;
	}

	void Renderer::setSharedMaterial(const ptr<Material>& material) {
		if (pImpl->sharedMaterials.empty()) pImpl->sharedMaterials.resize(1);
		pImpl->sharedMaterials.at(0) = material;
		setMaterial(material);
	}

	void Renderer::setSharedMaterials(const ptr_vector<Material>& materials) {
		pImpl->materials = pImpl->sharedMaterials = materials;
	}
}