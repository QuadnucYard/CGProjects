#include "roamer_engine/display/SkyBox.hpp"
#include "roamer_engine/display/Materials.hpp"
#include "roamer_engine/display/Shader.hpp"
#include <SOIL2/SOIL2.h>
#include <array>

namespace qy::cg {

	struct SkyBox::Impl {
		ptr<Material> material;
		GLuint skyboxVAO;
		GLuint skyboxVBO;
		GLuint cubemapTexture;
		//GLuint ebo;

		Impl(): cubemapTexture {} {
			static const  float skyboxVertices[] = {
				// positions          
				-1.0f, 1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,

				-1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,

				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,

				-1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, 1.0f
			};
			/*static const GLuint skyboxIndices[] {

			};*/
		
			glGenVertexArrays(1, &skyboxVAO);
			glGenBuffers(1, &skyboxVBO);
			glBindVertexArray(skyboxVAO);
			glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		}
	};

	DEFINE_OBJECT(SkyBox);

	ptr<SkyBox> SkyBox::loadFromFile(const fs::path& right, const fs::path& left, const fs::path& up, const fs::path& down, const fs::path& front, const fs::path& back) {
		auto sb = instantiate<SkyBox>();
		std::array<fs::path, 6>faces {right, left, up, down, front, back};

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for (size_t i = 0; i < faces.size(); i++) {
			int width, height, nrChannels;
			unsigned char* data = SOIL_load_image(faces[i].string().c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				glTexImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i),
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
			} else {
				throw std::runtime_error("Cubemap texture failed to load at path: " + faces[i].string());
			}
			SOIL_free_image_data(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		sb->pImpl->material = Materials::Skybox;
		sb->pImpl->cubemapTexture = textureID;

		return sb;
	}

	void SkyBox::__render(const glm::mat4& view, const glm::mat4& proj) {
		// draw skybox as last
		auto&& skyboxShader = pImpl->material->getShader();
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		skyboxShader.setInt("skybox", 0);
		skyboxShader.setMat4("view", glm::mat3(view)); // remove translation from the view matrix
		skyboxShader.setMat4("proj", proj);
		// skybox cube
		glBindVertexArray(pImpl->skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pImpl->cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
	}
}