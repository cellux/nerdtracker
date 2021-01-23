#include "vfx/hilite.h"
#include "glfw.h"

#include "ext/glm/gtc/type_ptr.hpp"

namespace vfx {

static const char *vertexShaderSource =
  "precision highp float;\n"
  "attribute vec2 a_position;\n"
  "uniform mat4 u_transform;\n"
  "void main(void) {\n"
  "  gl_Position = u_transform * vec4(a_position, 0.0, 1.0);\n"
  "}\n";

static const char *fragmentShaderSource = 
  "precision highp float;\n"
  "uniform vec3 u_color;\n"
  "uniform float u_alpha;\n"
  "void main(void) {\n"
  "  gl_FragColor = vec4(u_color, u_alpha);\n"
  "}\n";

HiLite::HiLite(GridInfo *gridInfo)
    : GridUser(gridInfo, vertexShaderSource, fragmentShaderSource)
{
  loc_.u_color = shaderProgram_.getUniformLocation("u_color");
  loc_.u_alpha = shaderProgram_.getUniformLocation("u_alpha");
}

void HiLite::draw() {
  int x = rect_.x;
  int y = rect_.y;
  int w = rect_.w;
  int h = rect_.h;
  vertexData_[0] = glm::vec2(0, -h);
  vertexData_[1] = glm::vec2(0, 0);
  vertexData_[2] = glm::vec2(w, -h);
  vertexData_[3] = glm::vec2(w, 0);
  glm::vec2 scale{gridInfo_->charSize_.x, gridInfo_->charSize_.y};
  glm::vec2 trans{x, y};
  useProgram(scale, trans);
  glUniform3fv(loc_.u_color, 1, glm::value_ptr(color_));
  glUniform1f(loc_.u_alpha, alpha_);
  glu::VAP a_position(a_position_, 2, GL_FLOAT, GL_FALSE, 0, vertexData_);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisable(GL_BLEND);
}

}
