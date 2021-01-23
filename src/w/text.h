#ifndef NT_WIDGET_TEXT_H
#define NT_WIDGET_TEXT_H

#include <string>
#include <string_view>

#include "w/widget.h"

namespace w {

struct Text : public Widget {
  std::string text_;
  Text(std::string_view text)
    : text_(text) {}
  void draw() {
    scr_->write(text_);
  }
  Point size() {
    int w = (int) text_.size();
    return Point{w, 1};
  }
};

} // namespace w

#endif
