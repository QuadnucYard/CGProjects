#include "roamer_engine/display/Material.hpp"
#include "roamer_engine/display/Shader.hpp"
#include <unordered_map>

namespace qy::cg {

	struct Material::Impl {
		Shader shader;

		template <class T>
		using prop_dict = std::unordered_map<std::string, T>;

		prop_dict<int> intProps;
		prop_dict<float> floatProps;
		prop_dict<color_t> colorProps;

		void applyProperties() const {
			shader.use();
			for (auto&& [k, v] : intProps) shader.setInt(k, v);
			for (auto&& [k, v] : floatProps) shader.setFloat(k, v);
			for (auto&& [k, v] : colorProps) shader.setVec4(k, v);
		}
	};

	DEFINE_OBJECT(Material);

	ptr<Material> Material::clone() const {
		auto cloned = instantiate<Material>();
		cloned->pImpl->shader = pImpl->shader;
		return cloned;
	}

	const Shader& Material::getShader() { return pImpl->shader; }

	void Material::setShader(const Shader& shader) { pImpl->shader = shader; }

	bool Material::hasInt(const std::string& name) const { return pImpl->intProps.contains(name); }

	int Material::getInt(const std::string& name) const { return pImpl->intProps.at(name); }

	void Material::setInt(const std::string& name, int value) { pImpl->intProps.insert({name, value}); }

	bool Material::hasFloat(const std::string& name) const { return pImpl->floatProps.contains(name); }

	float Material::getFloat(const std::string& name) const { return pImpl->floatProps.at(name); }

	void Material::setFloat(const std::string& name, int value) { pImpl->floatProps.insert({name, value}); }

	bool Material::hasColor(const std::string& name) const { return pImpl->colorProps.contains(name); }

	color_t Material::getColor(const std::string& name) const { return pImpl->colorProps.at(name); }

	void Material::setColor(const std::string& name, const color_t& value) { pImpl->colorProps.insert({name, value}); }

	void Material::__applyProperties() const {
		pImpl->applyProperties();
	}
}