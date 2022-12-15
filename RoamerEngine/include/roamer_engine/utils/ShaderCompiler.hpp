#pragma once
#include <filesystem>

namespace qy::cg {

	namespace fs = std::filesystem;

	class ShaderCompiler {

	public:
		std::string compile(const fs::path& path) const;

	private:
		std::vector<std::string> getAllLines(const fs::path& path, bool isRoot = false) const;

	public:
		inline static std::vector<fs::path> includePath {fs::current_path().parent_path() / "RoamerEngine" / "shaders"};
	};
}
