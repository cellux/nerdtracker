#ifndef NT_WIDGET_BYT_H
#define NT_WIDGET_BYT_H

#include <cstdint>
#include <functional>

#include "w/widget.h"

namespace w {

struct Byt : public Widget {
  using VFn = std::function<uint8_t(AppState*)>;
  using AFn = std::function<Action*(uint8_t)>;
  VFn vfn_;
  AFn afn_;
  int cursorOffset_{0};
  Byt(VFn vfn, AFn afn) : vfn_(vfn), afn_(afn) {}
  void writeNyb(uint8_t nyb);
  void draw();
  Point size() { return Point{2,1}; }
  Action *setNyb(unsigned int value);
  Rect buildHiliteRect() {
    return Rect{rect_.x+cursorOffset_, rect_.y, 1, 1};
  }
  void onFocus() {
    dispatch(setHiliteRect(buildHiliteRect()));
  }
  void onBlur() { dispatch(removeHiliteRect()); }
  bool onKeyDown(int key, int scancode, int mods);
  bool onCP(unsigned int codepoint);
};

} // namespace w

#endif
