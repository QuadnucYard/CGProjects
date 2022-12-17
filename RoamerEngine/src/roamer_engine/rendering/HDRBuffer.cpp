#include "roamer_engine/rendering/HDRBuffer.hpp"
#include "roamer_engine/display/Shaders.hpp"
#include "roamer_engine/rendering/RenderSettings.hpp"
#include "roamer_engine/Screen.hpp"

namespace qy::cg {

	void HDRBuffer::setup() {
		static int SCR_WIDTH = 0, SCR_HEIGHT = 0;
		// Check screen size
		if (Screen::width() != SCR_WIDTH || Screen::height() != SCR_HEIGHT) {
			if (hdrFBO) {
				glDeleteRenderbuffers(1, &rboDepth);
				glDeleteTextures(1, &colorBuffer);
				glDeleteFramebuffers(1, &hdrFBO);
			}
			SCR_WIDTH = Screen::width();
			SCR_HEIGHT = Screen::height();
		} else {
			return;
		}
		// Set up floating point framebuffer to render scene to
		glGenFramebuffers(1, &hdrFBO);
		// - Create floating point color buffer
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// - Create depth buffer (renderbuffer)
		
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
		// - Attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw "Framebuffer not complete!";
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		hdrShader = Shaders::HDR;
	}

	void HDRBuffer::activate() {
		setup();
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	}

	void HDRBuffer::render() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 2. Now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		hdrShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		hdrShader.setBool("hdr", RenderSettings::hdr);
		hdrShader.setFloat("exposure", RenderSettings::hdrExposure);
		hdrShader.setFloat("gamma", RenderSettings::hdrGamma);
		renderQuad();
	}

	void HDRBuffer::renderQuad() {
		static GLuint quadVAO = 0;
		static GLuint quadVBO;
		if (quadVAO == 0) {
			GLfloat quadVertices[] = {
				// Positions        // Texture Coords
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// Setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

}