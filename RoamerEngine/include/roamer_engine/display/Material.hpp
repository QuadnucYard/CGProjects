#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class Shader;
	class Texture;

	class Material: public Object {

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

		void __applyProperties() const;

	private:
		DECL_PIMPL;
	};
}

