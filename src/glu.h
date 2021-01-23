#ifndef NT_GLU_H
#define NT_GLU_H

#include <vector>
#include <string>
#include <string_view>

#include "glfw.h"
#include "util.h"

namespace glu {

static std::string getShaderInfoLog(GLuint shader) {
  GLint length;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  GLchar log[length];
  GLsizei logLen;
  glGetShaderInfoLog(shader, length, &logLen, log);
  return std::string(log, logLen);
}

static std::string getProgramInfoLog(GLuint program) {
  GLint length;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  GLchar log[length];
  GLsizei logLen;
  glGetProgramInfoLog(program, length, &logLen, log);
  return std::string(log, logLen);
}

void checkError();

struct Shader {
  GLuint shader_ = 0;
  Shader() = default;
  Shader(GLenum type, const std::string_view src) {
    shader_ = glCreateShader(type);
    const GLchar *data = src.data();
    GLint length = src.length();
    glShaderSource(shader_, 1, &data, &length);
    glCompileShader(shader_);
    GLint status;
    glGetShaderiv(shader_, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      DIE("glCompileShader() failed for %s shader: %s",
          type == GL_VERTEX_SHADER ? "vertex" : "fragment",
          getShaderInfoLog(shader_).c_str());
    }
  }
  void free() {
    if (shader_) {
      glDeleteShader(shader_);
      shader_ = 0;
    }
  }
};

struct Program {
  GLuint program_ = 0;
  std::vector<Shader> shaders_;
  Program() { program_ = glCreateProgram(); }
  ~Program() {
    for (auto shader : shaders_) {
      shader.free();
    }
    if (program_) {
      glDeleteProgram(program_);
      program_ = 0;
    }
  }
  void attach(Shader shader) {
    shaders_.push_back(shader);
    glAttachShader(program_, shader.shader_);
  }
  void link() {
    glLinkProgram(program_);
    GLint status;
    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
      DIE("glLinkProgram() failed: %s", getProgramInfoLog(program_).c_str());
    }
  }
  GLint getAttribLocation(const GLchar *name) {
    return glGetAttribLocation(program_, name);
  }
  GLint getUniformLocation(const GLchar *name) {
    return glGetUniformLocation(program_, name);
  }
  void use() {
    glUseProgram(program_);
  }
};

struct VAP {
  GLuint index_;
  VAP(GLuint index, GLint size, GLenum type, GLboolean normalized,
      GLsizei stride, const GLvoid *pointer)
      : index_(index) {
    glEnableVertexAttribArray(index_);
    glVertexAttribPointer(index_, size, type, normalized, stride, pointer);
  }
  ~VAP() {
    glDisableVertexAttribArray(index_);
  }
};

}

#endif
