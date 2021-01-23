#ifndef NT_EVT_H
#define NT_EVT_H

#include <functional>

struct Evt {
  using KeyCB = std::function<void(int key, int scancode, int action, int mods)>;
  using ChrCB = std::function<void(unsigned int codepoint)>;
  using FbsCB = std::function<void(int width, int height)>;
  using EvtCB = std::function<void()>;

  KeyCB keyCB_ = nullptr;
  ChrCB chrCB_ = nullptr;
  FbsCB fbsCB_ = nullptr;
  EvtCB evtCB_ = nullptr;
  bool running_ = false;

  void key(int key, int scancode, int action, int mods);
  void chr(unsigned int codepoint);
  void fbs(int width, int height);

  void run();
  void stop();
  void wake();

  void keyCB(KeyCB cb) { keyCB_ = cb; }
  void chrCB(ChrCB cb) { chrCB_ = cb; }
  void fbsCB(FbsCB cb) { fbsCB_ = cb; }
  void evtCB(EvtCB cb) { evtCB_ = cb; }
};

#endif
