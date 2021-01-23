#include "glu.h"

static const char *errorStr(GLenum err) {
  switch (err) {
  case GL_INVALID_ENUM:
    return "enum argument out of range";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "framebuffer is incomplete";
  case GL_INVALID_VALUE:
    return "numeric argument out of range";
  case GL_INVALID_OPERATION:
    return "operation illegal in current state";
  case GL_OUT_OF_MEMORY:
    return "not enough memory left to execute command";
  default:
    return "unknown error";
  }
}

namespace glu {

void checkError() {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    LOG("detected GL error: %s", errorStr(err));
  }
}

} // namespace glu
