#include "roamer_engine/utils/ShaderCompiler.hpp"
#include <fstream>
#include <regex>

namespace qy::cg {

	std::string ShaderCompiler::compile(const fs::path& path) const {
		/* 暂时不考虑报错行的问题 */
		auto lines = getAllLines(path, true);
		std::string ret;
		for (auto&& s : lines) {
			ret += s;
			ret += '\n';
		}
		return ret;
	}

	std::vector<std::string> ShaderCompiler::getAllLines(const fs::path& path, bool isRoot) const {
		std::ifstream shaderFile(path);
		std::string line;
		std::vector<std::string> lines;
		static std::regex includePattern(R"%(\s*#include\s*"(.+)"\s*)%");
		static std::regex expandPattern(R"%(\s*//!(.*))%");
		while (std::getline(shaderFile, line)) {
			if (std::smatch m; std::regex_match(line, m, includePattern)) {
				std::string fname = m[1];
				fs::path fullpath = fs::absolute(path.parent_path()) / fname;
				if (!fs::exists(fullpath)) {
					for (auto&& p : includePath) {
						fullpath = p / fname;
						if (fs::exists(fullpath)) break;
					}
				}
				if (!fs::exists(fullpath)) {
					throw std::runtime_error("Include path fault!");
				}
				lines.append_range(getAllLines(fullpath));
			} else if (isRoot || !line.starts_with("//?")) {
				lines.push_back(line);
			}
		}
		return lines;
	}
}