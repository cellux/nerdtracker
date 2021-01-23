#ifndef NT_WIDGET_HBOX_H
#define NT_WIDGET_HBOX_H

#include <vector>

#include "w/group.h"
#include "util.h"

namespace w {

struct HBox : public Group {
  Point size() {
    int maxH = 0;
    for (auto w : children_) {
      int height = w->size().y;
      if (height > maxH) {
        maxH = height;
      }
    }
    return Point{0,maxH};
  }
  void layout(const Rect r) {
    Widget::layout(r);
    int totalW = 0;
    int nZero = 0;
    for (auto w : children_) {
      int width = w->size().x;
      if (width) {
        totalW += width;
      } else {
        nZero++;
      }
    }
    int zeroW = nZero ? (r.w - totalW) / nZero : 0;
    Rect rw = r;
    for (auto w : children_) {
      auto size = w->size();
      rw.w = size.x ? size.x : zeroW;
      w->layout(rw);
      rw.x += rw.w;
    }
  }
};

} // namespace w

#endif
