#include <cmath>
#include <algorithm>

#include "action.h"
#include "app.h"
#include "util.h"
#include "glu.h"
#include "fc.h"

#include "fonts/DroidSansMono.ttf.h"
#define DEFAULT_FONT_SIZE 20

App::App()
    : t_(aud_.sr(), aud_.nchannels(), aud_.nf()),
      font_(&gridInfo_, DroidSansMonoTTF, DEFAULT_FONT_SIZE),
      scr_(&gridInfo_, &font_),
      hilite_(&gridInfo_),
      root_(&scr_, &state_, [&](Action *a){ dispatch(a); })
{
  setup();
  gui_.setEvt(&evt_);
  setScreenSize(gui_.getScreenSize());
  gui_.add(&scr_);
  gui_.add(&hilite_);
  evt_.fbsCB([&](int width, int height) {
    setScreenSize(Point{width, height});
  });
  evt_.chrCB([&](unsigned int codepoint) {
    invokeEH([=](w::Widget *w) { return w->onCP(codepoint); });
  });
  evt_.keyCB([&](int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) {
      invokeEH([=](w::Widget *w) {
        return w->onKeyUp(key, scancode, mods);
      });
    }
    else {
      invokeEH([=](w::Widget *w) {
        return w->onKeyDown(key, scancode, mods);
      });
    }
  });
  update();
  evt_.evtCB([&]() {
    update();
    glu::checkError();
  });
  evt_.run();
}

void App::setScreenSize(Point size) {
  glViewport(0, 0, size.x, size.y);
  gridInfo_.displaySize_ = size;
  font_.realloc();
  scr_.realloc();
}

void App::dispatch(Action *action) {
  if (!action)
    return;
  switch (action->type_) {
  case ActionType::setHiliteRect:
    hilite_.rect(((action::SetHiliteRect*) action)->r_);
    break;
  case ActionType::setFocusedWidget:
    if (state_.focusedWidget_) {
      state_.focusedWidget_->onBlur();
    }
    state_.focusedWidget_ = ((action::SetFocusedWidget *)action)->w_;
    if (state_.focusedWidget_) {
      state_.focusedWidget_->onFocus();
    }
    break;
  case ActionType::exitApp:
    evt_.stop();
    break;
  case ActionType::setInsPoly:
    state_.ins_->poly_ = ((action::SetInsPoly*) action)->poly_;
    break;
  }
  delete action;
}

// invoke event handler
void App::invokeEH(std::function<bool(w::Widget* w)> eh) {
  auto target = state_.focusedWidget_;
  bool handled = false;
  while (target && !handled) {
    handled = eh(target);
    target = target->parent_;
  }
}

void App::setup() {
  state_.ins_ = t_.getOrCreateIns(0);
  hilite_.color(fc::grape[2]);
}

void App::update() {
  Rect r{0, 0, gridInfo_.screenSize_.x, gridInfo_.screenSize_.y};
  scr_.rect(r);
  scr_.clear();
  root_.layout(r);
  root_.draw();
  gui_.draw();
}
