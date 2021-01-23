#include "root.h"

namespace w {

Root::Root(Scr *scr, AppState *state, ActionDispatcher dispatch) {
  add(&help_);
  add(&seq_);
  add(&pat_);
  add(&ins_);
  setCurrentWidget(&ins_);
  exec([=](w::Widget *w) {
    w->scr_ = scr;
    w->state_ = state;
    w->dispatch_ = dispatch;
  });
  layout(scr->rect_);
  auto currentPage = static_cast<Page*>(currentWidget_);
  dispatch(setFocusedWidget(currentPage->currentWidget_));
}

void Root::draw() {
  currentWidget_->draw();
}

bool Root::onKeyDown(int key, int scancode, int mods) {
  switch (key) {
  case GLFW_KEY_F1:
    setCurrentWidget(&help_);
    break;
  case GLFW_KEY_F2:
    setCurrentWidget(&seq_);
    break;
  case GLFW_KEY_F3:
    setCurrentWidget(&pat_);
    break;
  case GLFW_KEY_F4:
    setCurrentWidget(&ins_);
    break;
  case GLFW_KEY_ESCAPE:
    dispatch(exitApp());
    return true;
  default:
    return false;
  }
  auto currentPage = static_cast<Page*>(currentWidget_);
  dispatch(setFocusedWidget(currentPage->currentWidget_));
  return true;
}

} // namespace w
