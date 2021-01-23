#ifndef NT_GRID_H
#define NT_GRID_H

#include <string_view>

#include "ext/glm/ext/vector_float2.hpp"

#include "geom.h"
#include "glu.h"
#include "gui.h"

struct GridInfo {
  Point displaySize_; // size of the display (px)
  Point screenSize_; // size of the display (ch)
  TPoint<float> charSize_; // size of a single character (px)
  Point displayOffset_; // distance of screen from display edge (px)
};

struct GridUser {
  GridInfo *gridInfo_;
  glu::Shader vertexShader_;
  glu::Shader fragmentShader_ ;
  glu::Program shaderProgram_;
  int a_position_;
  int u_transform_;
  GridUser(GridInfo *gridInfo);
  GridUser(
    GridInfo *gridInfo,
    const std::string_view vertexShader,
    const std::string_view fragmentShader);
  void useProgram(glm::vec2 scale, glm::vec2 trans);
};

#endif
