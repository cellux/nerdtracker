#ifndef NT_ACTION_H
#define NT_ACTION_H

#include <cstdint>

#include "geom.h"

namespace w {
  struct Widget;
}

struct AppState;

enum class ActionType {
  exitApp,
  setFocusedWidget,
  setHiliteRect,
  setInsPoly
};

struct Action {
  ActionType type_;
  virtual ~Action() = default;
};

namespace action {

struct ExitApp : public Action {
};

struct SetFocusedWidget : public Action {
  w::Widget *w_;
};

struct SetHiliteRect : public Action {
  Rect r_;
};

struct SetInsPoly : public Action {
  uint8_t poly_;
};

} // namespace action

Action *exitApp();
Action *setFocusedWidget(w::Widget *w);
Action *setHiliteRect(Rect r);
Action *removeHiliteRect();
Action *setInsPoly(uint8_t poly);

#endif
