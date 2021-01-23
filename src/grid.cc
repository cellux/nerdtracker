#include "grid.h"

#include "ext/glm/ext/matrix_float4x4.hpp"
#include "ext/glm/ext/matrix_transform.hpp"
#include "ext/glm/gtc/type_ptr.hpp"

GridUser::GridUser(GridInfo *gridInfo) : gridInfo_(gridInfo) {}

GridUser::GridUser(GridInfo *gridInfo,
                   const std::string_view vertexShaderSource,
                   const std::string_view fragmentShaderSource)
    : gridInfo_(gridInfo), vertexShader_(GL_VERTEX_SHADER, vertexShaderSource),
      fragmentShader_(GL_FRAGMENT_SHADER, fragmentShaderSource)
{
  shaderProgram_.attach(vertexShader_);
  shaderProgram_.attach(fragmentShader_);
  shaderProgram_.link();
  a_position_ = shaderProgram_.getAttribLocation("a_position");
  u_transform_ = shaderProgram_.getUniformLocation("u_transform");
}

void GridUser::useProgram(glm::vec2 scale, glm::vec2 trans) {
  shaderProgram_.use();
  float sx = (2.0 / gridInfo_->displaySize_.x);
  float sy = (2.0 / gridInfo_->displaySize_.y);
  // scale pixel coordinates to OpenGL range [-1,1]
  scale.x *= sx;
  scale.y *= sy;
  glm::mat4 matScale = glm::scale(glm::mat4(1.f), glm::vec3(scale.x, scale.y, 1));
  float cw = gridInfo_->charSize_.x;
  float ch = gridInfo_->charSize_.y;
  int ox = gridInfo_->displayOffset_.x;
  int oy = gridInfo_->displayOffset_.y;
  trans.x = -1.0 + sx * (ox + trans.x * cw);
  trans.y = 1.0 - sy * (oy + trans.y * ch);
  glm::mat4 matTranslate = glm::translate(glm::mat4(1.f), glm::vec3(trans.x, trans.y, 0));
  glm::mat4 matTransform = matTranslate * matScale;
  glUniformMatrix4fv(u_transform_, 1, GL_FALSE, glm::value_ptr(matTransform));
}
