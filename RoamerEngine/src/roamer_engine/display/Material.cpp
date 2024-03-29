﻿#include "roamer_engine/display/Material.hpp"
#include "roamer_engine/display/Shader.hpp"
#include "roamer_engine/display/Texture.hpp"

namespace qy::cg {

	struct Material::Impl {
		Shader shader;

		prop_dict<int> intProps;
		prop_dict<float> floatProps;
		prop_dict<color_t> colorProps;
		prop_dict<ptr<Texture>> textureProps;

		Impl() {
			textureProps["_MainTex"] = nullptr;
			colorProps["_Color"] = {1.0f, 1.0f, 1.0f, 1.0f};
			colorProps["material.ambient"] = {1.0f, 1.0f, 1.0f, 1.0f};
			colorProps["material.diffuse"] = {1.0f, 1.0f, 1.0f, 1.0f};
			colorProps["material.specular"] = {0.0f, 0.0f, 0.0f, 0.0f};
			floatProps["material.shininess"] = 1;
		}

		void applyProperties() const {
			for (auto&& [k, v] : intProps) shader.setInt(k, v);
			for (auto&& [k, v] : floatProps) shader.setFloat(k, v);
			for (auto&& [k, v] : colorProps) shader.setVec4(k, v);

			for (int i = 0; auto && [k, v] : textureProps) {
				shader.setInt(k, i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, v ? v->id() : 0);
				i++;
			}
		}
	};

	DEFINE_OBJECT(Material);

	ptr<Material> Material::clone() const {
		auto cloned = instantiate<Material>();
		*cloned->pImpl = *pImpl;
		return cloned;
	}

	const Shader& Material::getShader() { return pImpl->shader; }

	void Material::setShader(const Shader& shader) { pImpl->shader = shader; }
	// 下面应该问shader
	bool Material::hasInt(const std::string& name) const { return pImpl->intProps.contains(name); }

	int Material::getInt(const std::string& name) const { return pImpl->intProps.at(name); }

	void Material::setInt(const std::string& name, int value) { pImpl->intProps.insert_or_assign(name, value); }

	bool Material::hasFloat(const std::string& name) const { return pImpl->floatProps.contains(name); }

	float Material::getFloat(const std::string& name) const { return pImpl->floatProps.at(name); }

	void Material::setFloat(const std::string& name, float value) { pImpl->floatProps.insert_or_assign(name, value); }

	bool Material::hasColor(const std::string& name) const { return pImpl->colorProps.contains(name); }

	color_t Material::getColor(const std::string& name) const { return pImpl->colorProps.at(name); }

	void Material::setColor(const std::string& name, const color_t& value) { pImpl->colorProps.insert_or_assign(name, value); }

	const ptr<Texture>& Material::getTexture(const std::string& name) {
		return pImpl->textureProps.at(name);
	}

	void Material::setTexture(const std::string& name, const ptr<Texture>& value) { pImpl->textureProps.insert_or_assign(name, value); }

	color_t Material::getColor() const { return getColor("_Color"); }

	void Material::setColor(const color_t& value) { setColor("_Color", value); }

	const ptr<Texture>& Material::getMainTexture() { return getTexture("_MainTex"); }

	void Material::setMainTexture(const ptr<Texture>& value) { setTexture("_MainTex", value); }

	const Material::prop_dict<int>& Material::getInts() const { return pImpl->intProps; }

	const Material::prop_dict<float>& Material::getFloats() const { return pImpl->floatProps; }

	const Material::prop_dict<color_t>& Material::getColors() const { return pImpl->colorProps; }

	const Material::prop_dict<ptr<Texture>>& Material::getTextures() const { return pImpl->textureProps; }

	void Material::__applyProperties() const { pImpl->applyProperties(); }
}