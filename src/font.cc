#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC

#include "font.h"
#include "util.h"
#include "ext/decode_utf8.h"

static const char *vertexShaderSource =
  "precision highp float;\n"
  "attribute vec2 a_position;\n"
  "attribute vec2 a_texcoord;\n"
  "attribute vec3 a_color;\n"
  "uniform mat4 u_transform;\n"
  "varying vec2 v_texcoord;\n"
  "varying vec3 v_color;\n"
  "void main(void) {\n"
  "  gl_Position = u_transform * vec4(a_position, 0.0, 1.0);\n"
  "  v_texcoord = a_texcoord;\n"
  "  v_color = a_color;\n"
  "}\n";

static const char *fragmentShaderSource =
  "precision highp float;\n"
  "uniform sampler2D u_font;\n"
  "varying vec2 v_texcoord;\n"
  "varying vec3 v_color;\n"
  "void main(void) {\n"
  "  vec4 t = texture2D(u_font, v_texcoord);\n"
  "  gl_FragColor = vec4(v_color, 1.0) * t.a;\n"
  "}\n";

Font::Font(GridInfo *gridInfo, unsigned char *ttf, int size)
    : GridUser(gridInfo, vertexShaderSource, fragmentShaderSource),
      ttf_(ttf),
      size_(size) {
  if (!stbtt_InitFont(&fontInfo_, ttf_, 0)) {
    DIE("stbtt_InitFont() failed");
  }

  stbtt_GetCodepointHMetrics(&fontInfo_, ' ', &advanceWidth_, nullptr);
  stbtt_GetFontVMetrics(&fontInfo_, &ascent_, &descent_, &lineGap_);

  advanceWidth_ *= 1.0;

  codePointBoxes_ = new box[numCodePoints_];
  for (uint32_t cp = minCodePoint_; cp <= maxCodePoint_; cp++) {
    int index = cp - minCodePoint_;
    box *b = &codePointBoxes_[index];
    stbtt_GetCodepointBox(&fontInfo_, cp, 
                          &b->x0, &b->y0, &b->x1, &b->y1);
  }

  vertexData_ = new glm::vec2[maxLineLength_ * 6];
  texCoords_ = new glm::vec2[maxLineLength_ * 6];
  colorData_ = new FontColor[maxLineLength_ * 6];

  loc_.a_texcoord = shaderProgram_.getAttribLocation("a_texcoord");
  loc_.a_color = shaderProgram_.getAttribLocation("a_color");
  loc_.u_font = shaderProgram_.getUniformLocation("u_font");
}

Font::~Font() {
  if (vertexData_) {
    delete[] vertexData_;
    vertexData_ = nullptr;
  }
  if (texCoords_) {
    delete[] texCoords_;
    texCoords_ = nullptr;
  }
  if (colorData_) {
    delete[] colorData_;
    colorData_ = nullptr;
  }
  if (packedChars_) {
    delete[] packedChars_;
    packedChars_ = nullptr;
  }
  if (codePointBoxes_) {
    delete[] codePointBoxes_;
    codePointBoxes_ = nullptr;
  }
  if (tex_) {
    glDeleteTextures(1, &tex_);
    tex_ = 0;
  }
}

void Font::realloc() {
  float ch = size_;
  scaleFactor_ = stbtt_ScaleForPixelHeight(&fontInfo_, ch);
  float cw = advanceWidth_ * scaleFactor_;
  gridInfo_->charSize_ = { cw, ch };

  int dw = gridInfo_->displaySize_.x;
  int dh = gridInfo_->displaySize_.y;
  int sw = dw / cw;
  int sh = dh / ch;

  gridInfo_->screenSize_ = { sw, sh };

  gridInfo_->displayOffset_ = {
    int((dw - cw * sw) / 2),
    int((dh - ch * sh) / 2)
  };

  int padding = 1;
  int i;
  for (i=8; i<16; i++) {
    textureSize_ = 1 << i;
    int numCharsX = textureSize_ / (cw + padding);
    int numCharsY = textureSize_ / (ch + padding);
    if (numCharsX * numCharsY >= numCodePoints_) {
      break;
    }
  }
  if (i == 16) {
    DIE("Cannot find a suitable texture size for the font atlas");
  }

  unsigned int oversampling = 2;
  textureSize_ *= oversampling;

  if (tex_) {
    glDeleteTextures(1, &tex_);
    tex_ = 0;
  }
  glGenTextures(1, &tex_);
  unsigned char *pixels = new unsigned char[textureSize_ * textureSize_];
  stbtt_pack_context spc;
  stbtt_PackBegin(&spc, pixels, textureSize_, textureSize_, 0, padding, nullptr);
  if (packedChars_) {
    delete[] packedChars_;
    packedChars_ = nullptr;
  }
  packedChars_ = new stbtt_packedchar[numCodePoints_];
  stbtt_PackSetOversampling(&spc, oversampling, oversampling);
  stbtt_PackFontRange(&spc, ttf_, 0, ch, minCodePoint_, numCodePoints_, packedChars_);
  stbtt_PackEnd(&spc);
  glBindTexture(GL_TEXTURE_2D, tex_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, textureSize_, textureSize_, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  delete[] pixels;
}

void Font::drawCps(int x, int y, const uint32_t *cps, const FontColor *col, int cnt) {
  int numElements = 0;
  int index = 0;
  glm::vec2 origin { 0, -ascent_ };
  float xpos = 0, ypos = 0;
  stbtt_aligned_quad q;
  for (int i = 0; i < cnt; i++) {
    uint32_t cp = cps[i];
    if (cp != ' ') {
      int charIndex = cp - minCodePoint_;
      FontColor c = col[i];

      glm::vec2 *vertexData = &vertexData_[index];
      box *b = &codePointBoxes_[charIndex];
      vertexData[0] = origin + glm::vec2(b->x0, b->y0);
      vertexData[1] = origin + glm::vec2(b->x0, b->y1);
      vertexData[2] = origin + glm::vec2(b->x1, b->y0);
      vertexData[3] = origin + glm::vec2(b->x1, b->y0);
      vertexData[4] = origin + glm::vec2(b->x0, b->y1);
      vertexData[5] = origin + glm::vec2(b->x1, b->y1);

      glm::vec2 *texCoords = &texCoords_[index];
      stbtt_GetPackedQuad(packedChars_,
                          textureSize_, textureSize_,
                          charIndex,
                          &xpos, &ypos, 
                          &q, 0);
      texCoords[0] = glm::vec2(q.s0, q.t1);
      texCoords[1] = glm::vec2(q.s0, q.t0);
      texCoords[2] = glm::vec2(q.s1, q.t1);
      texCoords[3] = glm::vec2(q.s1, q.t1);
      texCoords[4] = glm::vec2(q.s0, q.t0);
      texCoords[5] = glm::vec2(q.s1, q.t0);

      FontColor *colorData = &colorData_[index];
      colorData[0] = c;
      colorData[1] = c;
      colorData[2] = c;
      colorData[3] = c;
      colorData[4] = c;
      colorData[5] = c;

      index += 6;
      numElements++;
    }
    origin.x += advanceWidth_;
  }

  glm::vec2 scale{scaleFactor_, scaleFactor_};
  glm::vec2 trans{x, y};
  useProgram(scale, trans);
  glBindTexture(GL_TEXTURE_2D, tex_);
  GLint activeTexture;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
  glUniform1i(loc_.u_font, activeTexture - GL_TEXTURE0);
  glu::VAP a_position(a_position_, 2, GL_FLOAT, GL_FALSE, 0, vertexData_);
  glu::VAP a_texcoord(loc_.a_texcoord, 2, GL_FLOAT, GL_FALSE, 0, texCoords_);
  glu::VAP a_color(loc_.a_color, 3, GL_FLOAT, GL_FALSE, 0, colorData_);
  glDrawArrays(GL_TRIANGLES, 0, 6 * numElements);
}
