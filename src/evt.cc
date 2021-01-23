#include "evt.h"
#include "util.h"
#include "glfw.h"

void Evt::key(int key, int scancode, int action, int mods) {
  if (keyCB_) {
    keyCB_(key, scancode, action, mods);
    if (evtCB_) {
      evtCB_();
    }
  }
}

void Evt::chr(unsigned int codepoint) {
  if (chrCB_) {
    chrCB_(codepoint);
    if (evtCB_) {
      evtCB_();
    }
  }
}

void Evt::fbs(int width, int height) {
  if (fbsCB_) {
    fbsCB_(width, height);
    if (evtCB_) {
      evtCB_();
    }
  }
}

void Evt::run() {
  running_ = true;
  while (running_) {
    glfwWaitEvents();
  }
}

void Evt::stop() {
  running_ = false;
  wake();
}

void Evt::wake() {
  glfwPostEmptyEvent();
}
