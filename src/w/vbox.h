#ifndef NT_WIDGET_VBOX_H
#define NT_WIDGET_VBOX_H

#include <vector>

#include "w/group.h"

namespace w {

struct VBox : public Group {
  Point size() {
    int maxW = 0;
    for (auto w : children_) {
      int width = w->size().x;
      if (width > maxW) {
        maxW = width;
      }
    }
    return Point{maxW,0};
  }
  void layout(const Rect r) {
    Widget::layout(r);
    int totalH = 0;
    int nZero = 0;
    for (auto w : children_) {
      int height = w->size().y;
      if (height) {
        totalH += height;
      } else {
        nZero++;
      }
    }
    int zeroH = nZero ? (r.h - totalH) / nZero : 0;
    Rect rw = r;
    for (auto w : children_) {
      auto size = w->size();
      rw.h = size.y ? size.y : zeroH;
      w->layout(rw);
      rw.y += rw.h;
    }
  }
};

} // namespace w

#endif
