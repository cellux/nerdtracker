#ifndef NT_WIDGET_WIDGET_H
#define NT_WIDGET_WIDGET_H

#include <functional>

#include "scr.h"
#include "geom.h"
#include "action.h"
#include "appstate.h"

namespace w {

struct Widget {
  using WidgetVisitor = std::function<void(Widget*)>;
  using ActionDispatcher = std::function<void(Action*)>;
  Widget *parent_ { nullptr };
  Scr *scr_;
  AppState *state_;
  ActionDispatcher dispatch_;
  Rect rect_;
  virtual void exec(WidgetVisitor f) { f(this); }
  void dispatch(Action *action) {
    dispatch_(action);
  }
  virtual void draw() {};
  virtual Point size() {
    return Point{0, 0};
  }
  virtual void layout(const Rect r) { rect_ = r; }
  virtual void onFocus() {};
  virtual void onBlur() {};
  virtual bool onCP(unsigned int codepoint) {
    return false;
  }
  virtual bool onKeyDown(int key, int scancode, int mods) {
    return false;
  }
  virtual bool onKeyUp(int key, int scancode, int mods) {
    return false;
  }
};

} // namespace w

#endif
