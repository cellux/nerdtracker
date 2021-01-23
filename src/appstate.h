#ifndef NT_STATE_H
#define NT_STATE_H

#include <cstdint>

namespace w {
  struct Widget;
}

struct Ins;

struct AppState {
  w::Widget *focusedWidget_ { nullptr };
  Ins *ins_ { nullptr };
};

uint8_t getInsPoly(AppState*);

#endif
