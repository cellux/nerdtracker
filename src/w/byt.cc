#include "byt.h"
#include "glfw.h"

namespace w {

void Byt::writeNyb(uint8_t nyb) {
  if (nyb < 0x0a) {
    nyb += '0';
  }
  else {
    nyb += 'a' - 0x0a;
  }
  uint32_t cp = nyb;
  scr_->write(cp);
}

void Byt::draw() {
  uint8_t b = vfn_(state_);
  writeNyb(b >> 4);
  writeNyb(b & 0x0f);
}

Action *Byt::setNyb(unsigned int value) {
  uint8_t b = vfn_(state_);
  if (cursorOffset_ == 0) {
    return afn_((b & 0x0f) | (value << 4));
  }
  else {
    return afn_((b & 0xf0) | value);
  }
}

bool Byt::onKeyDown(int key, int scancode, int mods) {
  switch (key) {
  case GLFW_KEY_LEFT:
  case GLFW_KEY_RIGHT:
    cursorOffset_ = 1 - cursorOffset_;
    dispatch(setHiliteRect(buildHiliteRect()));
    break;
  default:
    return false;
  }
  return true;
}

bool Byt::onCP(unsigned int codepoint) {
  Action *a = nullptr;
  if (codepoint >= '0' && codepoint <= '9') {
    a = setNyb(codepoint-'0');
  }
  if (codepoint >= 'a' && codepoint <= 'f') {
    a = setNyb(codepoint-'a'+0x0a);
  }
  if (codepoint >= 'A' && codepoint <= 'F') {
    a = setNyb(codepoint-'A'+0x0a);
  }
  if (a) {
    dispatch(a);
    cursorOffset_ = 1 - cursorOffset_;
    dispatch(setHiliteRect(buildHiliteRect()));
    return true;
  }
  return false;
}

} // namespace w
