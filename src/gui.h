#ifndef NT_GUI_H
#define NT_GUI_H

#include <vector>

#include "glfw.h"
#include "geom.h"
#include "evt.h"

struct Drawable {
  virtual void draw() = 0;
};

struct Gui {
  GLFWwindow *win_;
  std::vector<Drawable*> drw_;

  Gui();
  ~Gui();
  void setEvt(Evt *evt);
  void draw();
  Point getScreenSize();
  void add(Drawable *drw) {
    drw_.push_back(drw);
  }
};

#endif
