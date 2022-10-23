#pragma once
#include "../Object.hpp"
#include "Shader.hpp"

namespace qy::cg {

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
		void setFloat(const std::string& name, int value);
		bool hasColor(const std::string& name) const;
		color_t getColor(const std::string& name) const;
		void setColor(const std::string& name, const color_t& value);

		void __applyProperties() const;

	private:
		DECL_PIMPL;
	};
}