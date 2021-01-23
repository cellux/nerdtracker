#include "gui.h"
#include "util.h"

Gui::Gui() : win_(nullptr) {
  if (!glfwInit()) {
    const char *errmsg;
    glfwGetError(&errmsg);
    DIE("glfwInit() failed: %s", errmsg);
  }

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  if (!monitor) {
    DIE("no monitors found");
  }

  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
  glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);

  win_ = glfwCreateWindow(mode->width, mode->height, "NerdTracker", monitor, NULL);
  if (!win_) {
    const char *errmsg;
    glfwGetError(&errmsg);
    DIE("glfwCreateWindow() failed: %s", errmsg);
  }
  glfwMakeContextCurrent(win_);

  glfwSetInputMode(win_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

Gui::~Gui() {
  if (win_) {
    glfwDestroyWindow(win_);
    win_ = nullptr;
  }
  glfwTerminate();
}

void Gui::setEvt(Evt *evt) {
  glfwSetWindowUserPointer(win_, evt);
  glfwSetFramebufferSizeCallback(
    win_,
    [](GLFWwindow *win, int width, int height) {
      Evt *evt = static_cast<Evt*>(glfwGetWindowUserPointer(win));
      evt->fbs(width, height);
    });
  glfwSetKeyCallback(
    win_,
    [](GLFWwindow *win, int key, int scancode, int action, int mods) {
      Evt *evt = static_cast<Evt*>(glfwGetWindowUserPointer(win));
      evt->key(key, scancode, action, mods);
    });
  glfwSetCharCallback(
    win_,
    [](GLFWwindow *win, unsigned int codepoint) {
      Evt *evt = static_cast<Evt*>(glfwGetWindowUserPointer(win));
      evt->chr(codepoint);
    });
}

void Gui::draw() {
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto drw : drw_) {
    drw->draw();
  }
  glfwSwapBuffers(win_);
}

Point Gui::getScreenSize() {
  Point p;
  glfwGetFramebufferSize(win_, &p.x, &p.y);
  return p;
}
