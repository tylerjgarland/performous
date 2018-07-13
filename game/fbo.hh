#pragma once

#include <boost/noncopyable.hpp>
#include "surface.hh"
#include "video_driver.hh"

/// Frame Buffer Object class
class FBO: boost::noncopyable {
  public:
	/// Generate the FBO and attach a fresh texture to it
	FBO(unsigned w, unsigned h): m_w(w), m_h(h) {
		update();
		// Create FBO
		glGenFramebuffers(1, &m_fbo);
		// Bind texture as COLOR_ATTACHMENT0
		bind();
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.id(), 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth.id(), 0);
		unbind();
	}
	/// Handle clean-up
	~FBO() {
		if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
	}
	/// Returns a reference to the attached texture
	OpenGLTexture<GL_TEXTURE_2D>& getTexture() {
		return m_texture;
	}
	/// Bind the FBO into use
	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}
	/// Unbind any FBO
	static void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void resize(unsigned w, unsigned h) {
		m_w = w;
		m_h = h;
		update();
	}
	unsigned width() const { return m_w; }
	unsigned height() const { return m_h; }
	void update() {
		{
			UseTexture tex(m_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_w, m_h, 0, GL_RGBA, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		}
		{
			UseTexture tex(m_depth);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_w, m_h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		}
	}
  private:
  	unsigned m_w;
  	unsigned m_h;
	GLuint m_fbo;
	OpenGLTexture<GL_TEXTURE_2D> m_texture;
	OpenGLTexture<GL_TEXTURE_2D> m_depth;
};

/// RAII FBO binder
struct UseFBO {
	UseFBO(FBO& fbo) { fbo.bind(); }
	~UseFBO() { FBO::unbind(); }
};
