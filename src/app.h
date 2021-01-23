#ifndef NT_APP_H
#define NT_APP_H

#include <memory>
#include <functional>

#include "aud.h"
#include "tracker.h"
#include "gui.h"
#include "grid.h"
#include "font.h"
#include "evt.h"
#include "scr.h"
#include "w/widget.h"
#include "w/root.h"
#include "vfx/hilite.h"
#include "action.h"
#include "appstate.h"
#include "geom.h"

struct App {
  Aud aud_;
  Tracker t_;
  Gui gui_;
  GridInfo gridInfo_;
  Font font_;
  Evt evt_;
  Scr scr_;
  vfx::HiLite hilite_;
  AppState state_;
  w::Root root_;

  App();
  void setScreenSize(Point size);
  void dispatch(Action *action);
  void invokeEH(std::function<bool(w::Widget*)> eh);
  void setup();
  void update();
};

#endif
