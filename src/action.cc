#include "action.h"
#include "util.h"

Action *exitApp() {
  auto a = new action::ExitApp;
  a->type_ = ActionType::exitApp;
  return a;
}

Action *setFocusedWidget(w::Widget *w) {
  auto a = new action::SetFocusedWidget;
  a->type_ = ActionType::setFocusedWidget;
  a->w_ = w;
  return a;
}

Action *setHiliteRect(Rect r) {
  auto a = new action::SetHiliteRect;
  a->type_ = ActionType::setHiliteRect;
  a->r_ = r;
  return a;
}

Action *removeHiliteRect() {
  return setHiliteRect(Rect{0,0,0,0});
}

Action *setInsPoly(uint8_t poly) {
  auto a = new action::SetInsPoly;
  a->type_ = ActionType::setInsPoly;
  a->poly_ = poly;
  return a;
}
