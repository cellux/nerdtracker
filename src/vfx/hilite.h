#ifndef NT_VFX_HILITE_H
#define NT_VFX_HILITE_H

#include "ext/glm/ext/vector_float2.hpp"

#include "grid.h"
#include "geom.h"
#include "glu.h"
#include "gui.h"
#include "fc.h"

namespace vfx {

struct HiLite : public GridUser, public Drawable {
  Rect rect_ {0,0,0,0};
  FontColor color_;
  float alpha_ {0.5};
  glm::vec2 vertexData_[4];
  struct {
    int u_color;
    int u_alpha;
  } loc_;
  HiLite(GridInfo *gridInfo);
  void draw();
  void rect(Rect r) { rect_ = r; }
  void color(FontColor c) { color_ = c; }
  void alpha(float a) { alpha_ = a; }
};

}

#endif
