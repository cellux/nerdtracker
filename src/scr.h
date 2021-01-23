#ifndef NT_SCR_H
#define NT_SCR_H

#include <string>
#include <string_view>
#include <sstream>
#include <cstdint>

#include "grid.h"
#include "geom.h"
#include "gui.h"
#include "font.h"

struct Scr : public GridUser, public Drawable {
  Font *font_ = nullptr;
  uint32_t *scr_ = nullptr;
  FontColor *col_ = nullptr;
  Rect rect_; // current rectangle
  Point cur_; // cursor position relative to current rectangle
  FontColor color_ = fc::white;
  Scr(GridInfo *gridInfo, Font *font);
  void realloc();
  void color(FontColor c);
  void rect(const Rect &r);
  void move(int x, int y);
  void write(const uint32_t cp);
  void write(const std::string_view s);
  void write(const std::string s) {
    write(std::string_view(s));
  }
  void write(const char *s) {
    write(std::string_view(s));
  }
  template <typename T> void write(const T &value) {
    std::ostringstream out;
    out << value;
    write(out.str());
  }
  template <typename T> void writeAt(int x, int y, const T &value) {
    move(x, y);
    write(value);
  }
  void clear(int x, int y, int w, int h);
  void clear(const Rect &r);
  void clear();
  void draw();
};

#endif
