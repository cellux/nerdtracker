#ifndef NT_FONT_H
#define NT_FONT_H

#include <vector>
#include <map>
#include <string_view>
#include <cstdint>

#include "ext/glm/ext/vector_float2.hpp"

#include "grid.h"
#include "glu.h"
#include "fc.h"

#include "ext/stb_rect_pack.h"
#include "ext/stb_truetype.h"

struct Font : public GridUser {
  using box = struct {
    int x0;
    int y0;
    int x1;
    int y1;
  };

  Font(GridInfo *gridInfo, unsigned char *ttf, int size);
  ~Font();

  void setSize(int pixelHeight) {
    size_ = pixelHeight;
  }
  void realloc();
  void drawCps(int x, int y, const uint32_t *cps, const FontColor *col, int cnt);
  bool cpOK(uint32_t cp) {
    return cp >= minCodePoint_ && cp <= maxCodePoint_;
  }

  unsigned char *ttf_;
  int size_;
  stbtt_fontinfo fontInfo_;
  uint32_t minCodePoint_ = 0x0020;
  uint32_t maxCodePoint_ = 0x01FF;
  const int numCodePoints_ = maxCodePoint_ - minCodePoint_ + 1;
  const int maxLineLength_ = 1024;
  float scaleFactor_;
  int advanceWidth_;
  int ascent_;
  int descent_;
  int lineGap_;
  stbtt_packedchar *packedChars_ = nullptr;
  box *codePointBoxes_ = nullptr;
  glm::vec2 *vertexData_ = nullptr;
  glm::vec2 *texCoords_ = nullptr;
  FontColor *colorData_ = nullptr;
  int textureSize_;
  GLuint tex_ = 0;
  struct {
    int a_texcoord;
    int a_color;
    int u_font;
  } loc_;
};

#endif
