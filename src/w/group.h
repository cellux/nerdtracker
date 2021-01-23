#ifndef NT_WIDGET_GROUP_H
#define NT_WIDGET_GROUP_H

#include <vector>

#include "w/widget.h"

namespace w {

struct Group : public Widget {
  std::vector<Widget*> children_;
  Widget *currentWidget_ { nullptr };
  void add(Widget *w) {
    w->parent_ = this;
    children_.push_back(w);
  }
  void draw() {
    for (auto w : children_) {
      scr_->rect(w->rect_);
      w->draw();
    }
  }
  void layout(const Rect r) {
    Widget::layout(r);
    for (auto w : children_) {
      w->layout(r);
    }
  }
  void exec(WidgetVisitor f) {
    Widget::exec(f);
    for (auto w : children_) {
      w->exec(f);
    }
  }
  void setCurrentWidget(Widget *w) {
    currentWidget_ = w;
  }
};

} // namespace w

#endif
