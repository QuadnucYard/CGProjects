#pragma once
#include "../Object.hpp"
#include <unordered_map>

namespace qy::cg {

	class Shader;
	class Texture;

	class Material: public Object {

		template <class T>
		using prop_dict = std::unordered_map<std::string, T>;

	public:
		DECL_OBJECT(Material);

		ptr<Material> clone() const;

		const Shader& getShader();
		void setShader(const Shader& shader);

		bool hasInt(const std::string& name) const;
		int getInt(const std::string& name) const;
		void setInt(const std::string& name, int value);
		bool hasFloat(const std::string& name) const;
		float getFloat(const std::string& name) const;
		void setFloat(const std::string& name, float value);
		bool hasColor(const std::string& name) const;
		color_t getColor(const std::string& name) const;
		void setColor(const std::string& name, const color_t& value);
		const ptr<Texture>& getTexture(const std::string& name);
		void setTexture(const std::string& name, const ptr<Texture>& value);

		color_t getColor() const;
		void setColor(const color_t& value);
		const ptr<Texture>& getMainTexture();
		void setMainTexture(const ptr<Texture>& value);

		const prop_dict<int>& getInts() const;
		const prop_dict<float>& getFloats() const;
		const prop_dict<color_t>& getColors() const;
		const prop_dict<ptr<Texture>>& getTextures() const;

		void __applyProperties() const;

	private:
		DECL_PIMPL;
	};
}

