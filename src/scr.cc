#include <algorithm>

#include "scr.h"
#include "util.h"
#include "ext/decode_utf8.h"

Scr::Scr(GridInfo *gridInfo, Font *font)
  : GridUser(gridInfo), font_(font) {}

void Scr::realloc() {
  int sw = gridInfo_->screenSize_.x;
  int sh = gridInfo_->screenSize_.y;
  if (scr_) {
    delete[] scr_;
    scr_ = nullptr;
  }
  scr_ = new uint32_t[sw * sh];
  if (col_) {
    delete[] col_;
    col_= nullptr;
  }
  col_ = new FontColor[sw * sh];
  color(fc::white);
  rect(Rect{0,0,sw,sh});
  clear();
}

void Scr::color(FontColor c) {
  color_ = c;
}

void Scr::rect(const Rect &r) {
  int sw = gridInfo_->screenSize_.x;
  int sh = gridInfo_->screenSize_.y;
  if (r.x >= 0 && r.y >= 0 && r.w <= sw && r.h <= sh) {
    rect_ = r;
    move(0, 0);
  }
}

void Scr::move(int x, int y) {
  cur_.x = std::clamp(x, 0, rect_.w-1);
  cur_.y = std::clamp(y, 0, rect_.h-1);
}

void Scr::write(const uint32_t cp) {
  int sw = gridInfo_->screenSize_.x;
  if (cur_.x < rect_.w && cur_.y < rect_.h) {
    int offset = (rect_.y + cur_.y) * sw + rect_.x + cur_.x;
    scr_[offset] = cp;
    col_[offset] = color_;
    cur_.x++;
  }
}

void Scr::write(const std::string_view s) {
  int sw = gridInfo_->screenSize_.x;
  uint32_t cp;
  uint32_t utf8state = 0;
  int offset = (rect_.y + cur_.y) * sw + rect_.x + cur_.x;
  for (uint8_t c : s) {
    if (cur_.x >= rect_.w) {
      break;
    }
    if (decode_utf8(&utf8state, &cp, c) == UTF8_ACCEPT) {
      if (!font_->cpOK(cp)) {
        cp = '?';
      }
      scr_[offset] = cp;
      col_[offset] = color_;
      offset++;
      cur_.x++;
    }
  }
}

void Scr::clear(int x, int y, int w, int h) {
  int sw = gridInfo_->screenSize_.x;
  int offset = y * sw + x;
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      scr_[offset++] = ' ';
    }
    offset += sw - w;
  }
}

void Scr::clear(const Rect &r) {
  clear(r.x, r.y, r.w, r.h);
}

void Scr::clear() {
  int sw = gridInfo_->screenSize_.x;
  int sh = gridInfo_->screenSize_.y;
  for (int i = 0; i < sw * sh; i++) {
    scr_[i] = ' ';
  }
}

void Scr::draw() {
  int sw = gridInfo_->screenSize_.x;
  int sh = gridInfo_->screenSize_.y;
  uint32_t *cp = scr_;
  FontColor *col = col_;
  for (int y = 0; y < sh; y++) {
    font_->drawCps(0, y, cp, col, sw);
    cp += sw;
    col += sw;
  }
}
